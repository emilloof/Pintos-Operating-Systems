#include "userprog/syscall.h"

static void syscall_handler (struct intr_frame *);
static int MAX_OPEN_FILES = 130;



  bool
  is_correct_address(void* ad){
    if(ad == NULL){
      return false;
    }
    if(is_user_vaddr(ad) && pagedir_get_page(thread_current()->pagedir, ad)){
      return true;
    }
    return false;
  }

  bool
  is_correct_string(char *s){
    int i=0;
    while(true){
      if(!is_correct_address(s)){
        return false;
      }
      if(*(s+i) == '\0'){
        return true;
      }
      i++;
    }
  }
  bool
  is_correct_buffer(void *buf , unsigned size){
    if(is_correct_address(buf) && is_correct_address(buf+size)){
      return true;
    }
    else{
      return false;
    }
  }



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
  if(!is_correct_string(file)){
    exit(-1);
  }
  return filesys_create(file, initial_size);
}

int 
open (const char *file)
{
  struct thread *current_thread = thread_current();
  for(int i = 2; i < MAX_OPEN_FILES; i++){
    if(current_thread->file_list[i] == NULL){
      struct file* opened = filesys_open(file);
      if(opened == NULL){
        return -1;
      }
      else{
        current_thread->file_list[i] = opened;
        return i;
      }      
    }
  }
  return -1;
}

void 
close (int fd)
{
  struct thread *current_thread = thread_current();
  if(fd<0 || fd>MAX_OPEN_FILES){
    exit(-1);
  }
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


int
wait(tid_t id){
  return process_wait(id);
}


void 
exit (int status)
{
  char* name;
  //int size = sizeof(thread_current()->name);
  //  strlcpy(name, thread_current()->name, size);
  char* saveptr;
  name= strtok_r(thread_current()->name, " ", &saveptr);
  thread_current()->parent_child->exit_status = status;
  printf("%s: exit(%d)\n", name, status);
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
  
 if(!is_correct_address(sp)){
    exit(-1);
  }
  
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

      if(!is_correct_string(file) || !is_correct_address(sp)){
        exit(-1);
      }

      f->eax = create(file, size);
      break; 
     }
    case SYS_OPEN:
      {
      
      const char *file = *(char**)sp;
      if(!is_correct_string(file)){
        exit(-1);
      }
   
      f->eax = open(file);
      break;
      }
    case SYS_CLOSE:
    {
      if(!is_correct_address(sp)){
        exit(-1);
      }
  
      int fd = *(int*)sp;
      close(fd);
      break;
    } 
    case SYS_READ:
    {
      int fd = *(int*)sp;
     if(!is_correct_address(sp)){
        exit(-1);
      }

      sp += 4;
      void *buffer = *(void**)sp;
      sp +=4;
      unsigned size = *(unsigned*)sp;
    if(!is_correct_buffer(buffer, size)){
        exit(-1);
      }

      f->eax = read(fd, buffer, size);
      break;
    }
    case SYS_WRITE:
    {
      int fd = *(int*)sp;
      if(!is_correct_address(sp)){
        exit(-1);
      }
  
      sp += 4;
      void *buffer = *(void**)sp;
      sp += 4;
      unsigned size = *(unsigned*)sp;
      if(!is_correct_buffer(buffer, size)){
        exit(-1);
      }
    
      f->eax = write(fd, buffer, size);
      break;
    }
    case SYS_EXIT:
    {
      int fd = *((int*)(sp));
      if(!is_correct_address(sp)){
        exit(-1);
      }
  
      exit(fd);
      break;
    }
    case SYS_EXEC:
    {
      const char *cmd_line = *(char**)sp;
      if(!is_correct_string(cmd_line)){
        exit(-1);
      }
    
      f->eax = exec(cmd_line);
      break;
    }
    case SYS_WAIT:
    {
      int id = *((int*)(sp));
      if(!is_correct_address(sp)){
        exit(-1);
      }
      f->eax = wait(id);
    }
  }
  }

