# OS

### Build

To build:
```
cd src/
make
```

### Run

To run:
```
cd build/
bochs -q
```

## Overview

The end goal of this OS is to contain these features:
+ Memory Management
  + Main Memory
  	+ [Paging](./docs/paging.md)
  	+ Segmentation
  + [Virtual Memory](./docs/virtualmemory.md)
+ Storage Management
    + File System
    + I/O system 
+ Process Management
   + Processes
      + IPC
   + Threads
   + Synchronization
   + Scheduling
+ Protection and Security
   + Protection
   + Security
