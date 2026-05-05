# Pintos Operating System 💻⚙️

This repository contains the source code and implementations for the Pintos instructional operating system, developed as part of the TDDB68 (Concurrent Programming and Operating Systems) coursework. Pintos is a simple operating system framework for the 80x86 architecture that supports kernel threads, loading and running user programs, and a file system.

## 📂 Repository Structure

The project is organized into several key directories reflecting the progression of the operating system labs:

*   **`src/threads/`**: Contains the core kernel threading logic, synchronization primitives (semaphores, locks, condition variables), and scheduling algorithms.
*   **`src/userprog/`**: Holds the implementation for loading and executing user programs, handling system calls, and managing process execution.
*   **`src/vm/`**: Dedicated to virtual memory management, including page tables, memory mapping (`mmap`), and swapping.
*   **`src/filesys/`**: Contains the implementation of the Pintos file system, including file creation, reading, writing, and directory management.
*   **`src/examples/` & `src/tests/`**: Includes sample user programs (e.g., `echo`, `cat`, `ls`) and an extensive test suite used to validate the OS functionality across all assignments.
*   **`pintos-linked-list/` & `singly-linked-list/`**: Standalone implementations and tests for kernel-level linked list data structures.

## 🛠️ Tech Stack & Tools

*   **Language:** C and x86 Assembly.
*   **Build System:** Makefiles configured for different OS sub-systems (e.g., `Makefile.kernel`, `Makefile.userprog`).
*   **Emulation/Debugging:** Configured to run on Bochs and QEMU, with included GDB macros for kernel debugging (`src/utils/gdb-macros`, `src/utils/pintos-gdb`).

## 🚀 Getting Started

### Prerequisites
To build and run Pintos, you will need a Linux environment (or a suitable VM/container) equipped with:
*   GCC (cross-compiler for x86 if building on a different architecture).
*   Make.
*   Bochs or QEMU emulators.

### Building and Running
1. Clone this repository to your local machine.
2. Navigate to the directory of the assignment you wish to build (e.g., `cd src/threads`).
3. Run `make` to compile the kernel.
4. Navigate to the `build` directory that is generated.
5. Use the `pintos` utility located in `src/utils/` to run tests or execute the kernel. Example:
   ```bash
   pintos --run alarm-multiple
