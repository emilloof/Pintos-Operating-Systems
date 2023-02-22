#include "userprog/syscall.h"

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
  if(current_thread->file_list[fd] != NULL){
    file_close(current_thread->file_list[fd]);
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
    return (int)size;
  }
  else if(2 <= fd && fd < MAX_OPEN_FILES && current_thread->file_list[fd] != NULL){
    struct file *opened_file = current_thread->file_list[fd];
    return file_read(opened_file, buffer, size);
  }
  return -1;
}

int 
write (int fd, const void *buffer, unsigned size)
{ 
  
  if(fd == 1){
    putbuf(buffer, size);
    return (int) size;
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
  return -1;
}

void 
exit (int status)
{
  thread_exit();  
}

tid_t 
exec (const char *cmd_line)
{
  return process_execute(cmd_line);
}

static void
syscall_handler (struct intr_frame *f UNUSED) 
{
 
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
    case SYS_EXEC:
    {
      const char *cmd_line = *(char**)sp;
      f->eax = exec(cmd_line);
      break;
    }
  }
}

