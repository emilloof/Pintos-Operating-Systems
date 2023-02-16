#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "filesys/filesys.h"
#include "filesys/file.h"
#include "devices/input.h"
#include "threads/init.h"
#include "userprog/process.h"

/*
export PATH="${HOME}/Desktop/pintos/src/utils/:${PATH}/"
pintos --qemu -- run lab1test2
*/


static void syscall_handler (struct intr_frame *);
static int MAX_OPEN_FILES = 130;



void
syscall_init (void)  
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}



void
halt (void)
{
  power_off();
}

bool
create (const char *file, unsigned initial_size)
{
  return  filesys_create(file, initial_size);
}

int 
open (const char *file)
{
  struct thread  *current_thread = thread_current();
  for(int i = 2; i < MAX_OPEN_FILES; i++){
    if(current_thread->file_list[i] == NULL){
      struct file* opended = filesys_open(file);
      if(opended != NULL){
        current_thread->file_list[i] = opended;
        printf("%d\n", i);
        return i;
      }
      return -1;      
    }
  //}
  return -1;
}

void 
close (int fd)
{
  struct thread *current_thread = thread_current();
  if(current_thread->file_list[fd] != NULL){
    file_close(current_thread->file_list[fd]);
    printf("%d\n",fd);
    current_thread->file_list[fd] = NULL;
  }
}

int
read (int fd, void *buffer, unsigned size)
{
  struct thread *current_thread = thread_current();

  if(fd == 0){
    char *bufferCopy = (char *) buffer;
    for(unsigned i = 0; i < size; i++){
      uint8_t key = input_getc();
      bufferCopy[i] = key;
    }
    return size;
  }
  else if(2 <= fd && fd < MAX_OPEN_FILES && current_thread->file_list[fd] != NULL){
    struct file *opened_file = current_thread->file_list[fd];
    printf("%s\n", "hej");
    return file_read(opened_file, buffer, size);
  }
  return -1;
}

int 
write (int fd, const void *buffer, unsigned size)
{ 
  
  if(fd == 1){
<<<<<<< HEAD
    putbuf((char *)buffer, size);
    return (int) size;


=======
    putbuf(buffer, size);
    return (int) size;
>>>>>>> bccfa74c5b9ee680f78063f3682f495f28353cf2
  }
  else if(2 <= fd && fd < MAX_OPEN_FILES){

    struct thread *current_thread = thread_current();
    struct file *opened_file = current_thread->file_list[fd];
    if(opened_file != NULL){
       return file_write(opened_file, buffer, size);
    }
  }
  else{
    return -1;
  } 
<<<<<<< HEAD
=======
  return -1;
>>>>>>> bccfa74c5b9ee680f78063f3682f495f28353cf2
}

void 
exit (int status)
{
  thread_exit();  
}

static void
syscall_handler (struct intr_frame *f UNUSED) 
{
  printf ("system call!\n");
 
  void* sp = f->esp;
  int id = *(int*)(sp);
  sp+=4;

  switch(id){
    case SYS_HALT:
    {
      halt();
      break;
    }
    case SYS_CREATE:
     {
      const char *file = *(char**)sp;
      sp+=4;
      unsigned size = *(unsigned*)sp;
      f->eax = create(file, size);
      break; 
     }
    case SYS_OPEN:
      {
      const char *file = *(char**)sp;
      f->eax = open(file);
      break;
      }
    case SYS_CLOSE:
    {
      int fd = *(int*)sp;
      close(fd);
      break;
    } 
    case SYS_READ:
    {
      int fd = *(int*)sp;
      sp += 4;
      void *buffer = *(void**)sp;
      sp +=4;
      unsigned size = *(unsigned*)sp;
      f->eax = read(fd, buffer, size);
      break;
    }
    case SYS_WRITE:
    {
      int fd = *(int*)sp;
      sp += 4;
      void *buffer = *(void**)sp;
      sp += 4;
      unsigned size = *(unsigned*)sp;
      f->eax = write(fd, buffer, size);
      break;
    }
    case SYS_EXIT:
    {
      int fd = *((int*)(sp));
      exit(fd);
      break;
    }
  }
}

