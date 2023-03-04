#include <stdbool.h>
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "filesys/filesys.h"
#include "filesys/file.h"
#include "devices/input.h"
#include "threads/init.h"
#include "threads/vaddr.h"
#include "userprog/process.h"
#include "userprog/pagedir.h"
#include "lib/string.h"
#include <stdlib.h>


#ifndef USERPROG_SYSCALL_H
#define USERPROG_SYSCALL_H


void syscall_init (void);
void halt(void);
bool create (const char *file, unsigned initial_size);
int open (const char *file);
void close (int fd);
int read (int fd, void *buffer, unsigned size);
int write (int fd, const void *buffer, unsigned size);
int wait(tid_t id);
void exit (int status);
tid_t exec (const char *cmd_line);
bool is_correct_address(void *ad);
bool is_correct_string(char *s);
bool is_correct_buffer(void *buf , unsigned size);

#endif /* userprog/syscall.h */
