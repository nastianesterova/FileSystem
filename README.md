# PDOS File System

A FAT-based (File Allocation Table) pseudo DOS file system implementation in C that operates entirely in shared memory, simulating disk operations without requiring actual hardware disk access.

## Overview

This project implements a simplified file system inspired by the original DOS FAT file system. It uses POSIX shared memory to simulate a disk, providing a complete set of file operations including creating, reading, writing, and listing files and directories.

### Key Features

- **FAT-based architecture** — Classic File Allocation Table structure for block management
- **Shared memory disk** — Uses `/dev/shm/MYFS` for fast, in-memory storage
- **DOS-compatible naming** — Supports 8.3 filename format (max 13 characters including extension)
- **Block-based storage** — 1024-byte blocks with support for up to 1024 blocks (1 MB total)
- **Standard file operations** — Open, read, write, close, and directory listing

## Disk Layout

```
┌─────────────────────────────┐  Block 0
│      Reserved Block         │  (Contains FS ID string)
├─────────────────────────────┤  Block 1
│        FAT Block 1          │  (512 entries)
├─────────────────────────────┤  Block 2
│        FAT Block 2          │  (512 entries)
├─────────────────────────────┤  Block 3
│    Root Directory Block     │
├─────────────────────────────┤  Block 4+
│                             │
│        Data Blocks          │
│           ...               │
│                             │
└─────────────────────────────┘  Block 1023
```

## API Reference

### Disk & File System Management

| Function | Description |
|----------|-------------|
| `void pdos_mkdisk(int sz)` | Create shared memory disk of `sz` bytes |
| `void pdos_mkfs(char *ID)` | Format disk and initialize FAT structure with identifier |

### File Operations

| Function | Description |
|----------|-------------|
| `PDOS_FILE *pdos_open(const char *fname, const char *mode)` | Open file (`"r"`, `"w"`, or `"rw"`) |
| `int pdos_fgetc(PDOS_FILE *file)` | Read single byte from file |
| `void pdos_fputc(int b, PDOS_FILE *file)` | Write single byte to file |
| `void pdos_fclose(PDOS_FILE *file)` | Close file and flush buffers |

### Directory Operations

| Function | Description |
|----------|-------------|
| `char **pdos_dir()` | List contents of root directory |
| `void pdos_mkdir(char *dir)` | Create new directory |

## Building

### Prerequisites

- GCC compiler with C11 support
- POSIX-compliant system (Linux recommended)
- `librt` (realtime extensions library)

### Compile the Library and Test Programs

```bash
# Build everything
make $(OBJ_DIR) $(LIB_DIR) $(LIB_DIR)/libPseudoFS.a all_tests

# Or build individual components
make mkdisk mkfs write_file read_from_file print_file list_dir make_dir
```

### Clean Build Artifacts

```bash
make clean
```

## Usage Examples

### 1. Create and Initialize the File System

```bash
# Create a 1MB disk in shared memory
./mkdisk

# Format the file system with default ID
./mkfs
```

### 2. Create and Write to a File

```bash
# Write 1023 bytes of A-Z pattern to file1.txt
./write_file file1.txt

# Write 1024 bytes of A-Z pattern
./write_file file1.txt 1

# Expand existing file with additional 1024 bytes
./write_file file1.txt 1 expand
```

### 3. Read File Contents

```bash
# Print file contents to stdout
./print_file file1.txt

# Verify file contents match expected A-Z pattern
./read_from_file file1.txt
```

### 4. Directory Operations

```bash
# List contents of root directory
./list_dir

# Create a new directory
./make_dir mydir
```

## Technical Specifications

| Parameter | Value |
|-----------|-------|
| Block Size | 1024 bytes |
| Maximum Blocks | 1024 |
| Total Disk Capacity | 1 MB |
| Max Filename Length | 13 characters |
| Max Path Length | 512 characters |
| FAT Entry Size | 2 bytes (short) |

### FAT Entry Values

| Value | Meaning |
|-------|---------|
| `-1` (UNUSED) | Block is free |
| `0` (ENDOFCHAIN) | End of file chain |
| `n > 0` | Next block in chain |

## Project Structure

```
FileSystem-master/
├── pdos.h              # Public API declarations
├── pdosfilesys.h       # Data structures and constants
├── pdos_mkdisk.c       # Disk creation (shared memory)
├── pdos_mkfs.c         # File system formatting
├── pdos_open.c         # File open operations
├── pdos_fgetc.c        # Read byte from file
├── pdos_fputc.c        # Write byte to file
├── pdos_fclose.c       # Close file
├── pdos_dir.c          # Directory listing
├── pdos_mkdir.c        # Create directory
├── Makefile            # Build configuration
├── test_src/           # Test programs
│   ├── mkdisk.c        # Test: create disk
│   ├── mkfs.c          # Test: format file system
│   ├── write_file.c    # Test: write to file
│   ├── read_from_file.c# Test: read and verify file
│   ├── print_file.c    # Test: print file contents
│   ├── list_dir.c      # Test: list directory
│   └── make_dir.c      # Test: create directory
└── tests_writeup.txt   # Test documentation
```

## Data Structures

### Directory Entry (`DIR_ENTRY`)

```c
typedef struct direntry {
    char    isdir;              // 1 if directory, 0 if file
    time_t  filemodtime;        // Last modification time
    int     filelength;         // File size in bytes
    short   filefirstblock;     // First block in FAT chain
    char    name[MAXNAME];      // Filename (max 13 chars)
} DIR_ENTRY;
```

### File Descriptor (`PDOS_FILE`)

```c
typedef struct FILE_DESCRIPTOR {
    char        mode[3];        // "r", "w", or "rw"
    int         pos;            // Position in current block
    short       blocknum;       // Current block number
    DISK_BLOCK  buffer;         // Buffered I/O
    unsigned char entrylistIdx; // Directory entry index
} PDOS_FILE;
```

## License

This project is provided for educational purposes.

## References

- [File Allocation Table (Wikipedia)](https://en.wikipedia.org/wiki/File_Allocation_Table)
- [POSIX Shared Memory](https://man7.org/linux/man-pages/man7/shm_overview.7.html)
