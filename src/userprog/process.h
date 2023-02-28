#ifndef USERPROG_PROCESS_H
#define USERPROG_PROCESS_H

#include "threads/thread.h"
#include "threads/synch.h"
#include <stdlib.h>

tid_t process_execute (const char *file_name);
int process_wait (tid_t);
void process_exit (void);
void process_activate (void);


struct parent_child{
  char* name;  
  int exit_status;
  int alive_count;
  struct thread *current;
  struct thread *parent;
  struct semaphore sema;
  struct lock lock;
  struct list_elem elem;
  const char* fn;
};


#endif /* userprog/process.h */
