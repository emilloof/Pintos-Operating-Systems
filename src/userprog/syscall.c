#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "filesys/filesys.h"
#include "filesys/file.h"
#include "devices/input.h"


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
  return filesys_create(file, initial_size);
}

int 
open (const char *file)
{
  int fd; 
  struct thread  *current_thread = thread_current();
  for(int i = 2; i < MAX_OPEN_FILES; i++){
    if(current_thread->file_list[i] == NULL){
      current_thread->file_list[i] = i;
      struct file* opended = filesys_open(file);
      if(opended != NULL){
        current_thread->file_list[i] = opended;
        return i;
      }
      return -1;      
    }
  }
  return -1;
}

void 
close (int fd)
{
  struct thread *current_thread = thread_current();
  file_close(current_thread->file_list[fd]);
  current_thread->file_list[fd] == NULL;
}

int
read (int fd, void *buffer, unsigned size)
{
  if(fd == 0){
    char *bufferCopy = (char *) buffer;
    for(int i = 0; i < size; i++){
      uint8_t key = input_getc();
      bufferCopy[i] = key;
    }
    return size;
  }
  else if(2 <= fd && fd <= MAX_OPEN_FILES){
    struct thread *current_thread = thread_current();
    struct file *opened_file = current_thread->file_list[fd];
    return file_read(opened_file, buffer, size);
  }
  else{
    return -1;
  }

}

int 
write (int fd, const void *buffer, unsigned size)
{ 
  
  if(fd == 1){
    //putbuf(buffer, size);
    //return (int) size;

    size_t size_left = (size_t)size;
    if ((int)size_left == 0) {
      return (int)size;
    }
    else if ((int)size_left < 200){
        putbuf(buffer, size_left);
        size_left = 0;
    }
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
  /*
    size_t i;
  if(fd==1){
    size_t size_left = (size_t)size;
    for (i = 0; i < (size_t) size; i++) {
      if ((int)size_left == 0) {
        return (int)size;
      }
      else if ((int)size_left < 200){
        putbuf(buffer, size_left);
        size_left = 0;
      }
      else{
        // at  least  aslong as size is not bigger than a few hundred bytes.
        // (It is reasonable to breakup larger buffers.) CHOOSE 200
      //  putbuf((char*)buffer, 200);
        putbuf(buffer, 200);
        size_left -= 200;
      }
    }
  }
  else if (2 <= fd && fd < 130) {
    //find current thread.
    struct thread *current_thread = thread_current();
    //find file from threads file-list. 0&1 taken accord.to assign.
    struct file *file = current_thread->file_list[fd];

    int written_bytes = file_write (file, buffer, (off_t)size);

    if (written_bytes != 0 ) {
      return written_bytes;
    }
    else {
      return -1;
    }
  }
  return -1;
*/
}

void 
exit (int status)
{
  thread_exit();  
}

static void
syscall_handler (struct intr_frame *f UNUSED) 
{
  //printf ("system call!\n");
 
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
      int fd = sp;
      void *buffer = *(void**)sp+4;
      unsigned size = (unsigned)sp+8;
      f->eax = read(fd, buffer, size);
      break;
    }
    case SYS_WRITE:
    {
      int fd = *((int*)(sp));
      sp += 4;
      void *buffer = *((void**)sp);
      sp += 4;
      unsigned size = *((unsigned*)(sp));
      f->eax = write(fd, buffer, size);
      break;
    }
    case SYS_EXIT:
    {
      exit(id);
      break;
    }
  }
}