#pragma once

//#include "proc.h"

#define T_DIR     1   // Directory
#define T_FILE    2   // File
#define T_DEVICE  3   // Device

struct stat {
  int dev;     // File system's disk device
  uint ino;    // Inode number
  short type;  // Type of file
  short nlink; // Number of links to file
  uint64 size; // Size of file in bytes
};


struct process_info{
  int state;
  int parent_id;
  uint64 memory;
  uint files;
  uint ticks0;
  uint running_ticks;
  uint switch_times;
  char name [16];
};
