# FogOS  
*Fall 2025 Edition*  

![FogOS](docs/fogos.gif)

---

## Overview  

This project extends **FogOS** with new user-space utilities.  

In **Project 1**, we (Shenrui and Ezequiel) added two new commands:  

- `clear` — clears the terminal screen using ANSI escape codes.  
- `tree` — displays the file system hierarchy in a tree-like structure.  

These commands are integrated into the xv6 user programs and available through the shell.  

---

## Changes Made  

### `clear` Command  
- **Purpose:** Clears the screen and moves the cursor to the top-left corner.  
- **Implementation:**  
  - Created `user/clear.c`  
  - Modified `user/sh.c` to support the command  
  - Updated `Makefile` to include the new program  

### `tree` Command  
- **Purpose:** Displays the directory structure of the file system recursively.  
- **Implementation:**  
  - Created `user/tree.c`  
  - Updated `Makefile` to include the new program  

---
