#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "filesys/filesys.h"
#include "filesys/file.h"

static void syscall_handler (struct intr_frame *);
static int MAX_OPEN_FILES = 128;



void
syscall_init (void)  
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

static void
syscall_handler (struct intr_frame *f UNUSED) 
{
  //printf ("system call!\n");
  int sp = f->esp;
  switch(sp){
    case(SYS_HALT){
      halt();
      break;
    }
    case(SYS_CREATE){
      const char* file = sp+4;
      unsigned size = sp+8;
      create(file, size);
      break; 
    }
    case(SYS_OPEN){
      const char* file = sp+4;
      open(file);
      break;
    }
    case(SYS_CLOSE){
      close(sp);
      break;
    }
    case(SYS_READ){
      int fd = sp;
      int size = sp+4;
      read(fd, buffer?, size);
    break;
    }
    case(SYS_WRITE){
      int fd = sp;
      int size = sp+4;
      write(fd, buffer?, size);
    break;
    }
    case(SYS_EXIT){
      exit();
      break;
    }
  }
  thread_exit ();
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
    if(current_thread->fd_list[i] == NULL){
      current_thread->fd_list[i] = i;
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
    (uint8_t) *bufferCopy = (uint8_t) *buffer;
    for(int i = 0; i < size; i++){
      uint8_t key = inputgetc();
      bufferCopy[i] = key;
    }
    return size;
  }
  else if(2 <= fd && fd <= 128)
    struct thread *current_thread = thread_current();
    struct file *opened_file = current_thread->file_list[fd];
    return file_read(opened_file, buffer, size);
  else{
    return -1
  }

}

int 
write (int fd, const void *buffer, unsigned size)
{ 
  if(fd == 1){
    for(int i = 0; i < size; i++){
      s
    }
    putbuf(buffer, size);
    return (int) size;
  }
  else if(2 <= fd && fd <= 128){
    struct thread *current_thread = thread_current();
    struct file *opened_file = current_thread->file_list[fd];
    return file_write(opened_file, buffer, size);
  }
  else{
    return -1;
  }
}

void 
exit (int status)
{
  thread_exit();  
}