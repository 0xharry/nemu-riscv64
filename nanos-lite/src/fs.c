#include <fs.h>
#include "/home/harry/ics2020/abstract-machine/am/include/am.h"

extern size_t ramdisk_read(void *buf, size_t offset, size_t len);
extern size_t ramdisk_write(const void *buf, size_t offset, size_t len);
extern size_t get_ramdisk_size();
extern size_t serial_write(const void *buf, size_t offset, size_t len);
typedef size_t (*ReadFn) (void *buf, size_t offset, size_t len);
typedef size_t (*WriteFn) (const void *buf, size_t offset, size_t len);

typedef struct {
  char *name;           // fixed in nanos's initialization
  size_t size;          // fixed in nanos's initialization
  size_t disk_offset;   // fixed in nanos's initialization
  ReadFn read;
  WriteFn write;
  size_t file_offset;   // offset for each program file, start at 0;
} Finfo;

enum {FD_STDIN, FD_STDOUT, FD_STDERR, FD_FB};

size_t invalid_read(void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

size_t invalid_write(const void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

/* This is the information about all files in disk. */
static Finfo file_table[] __attribute__((used)) = {
  [FD_STDIN]  = {"stdin", 0, 0, invalid_read, invalid_write},
  [FD_STDOUT] = {"stdout", 0, 0, invalid_read, serial_write},
  [FD_STDERR] = {"stderr", 0, 0, invalid_read, serial_write},
#include "files.h"
};

void init_fs() {
  // TODO: initialize the size of /dev/fb
}

#define total_file_num sizeof(file_table)/sizeof(Finfo)
size_t fs_lseek(int fd, size_t offset, int whence) {
  switch (whence){
  case SEEK_SET:
    // assert(file_table[fd].size >= offset);
    file_table[fd].file_offset = offset;
    break;

  case SEEK_CUR:
    file_table[fd].file_offset += offset;
    // assert(file_table[fd].file_offset <= file_table[fd].size);
    break;

  case SEEK_END:
    file_table[fd].file_offset = file_table[fd].size + offset;
    break;

  default: assert(0);
  }

  return file_table[fd].file_offset;
}

int fs_open(const char *pathname, int flags, int mode) {
  for(int fd=0; fd<total_file_num; ++fd)
    if(strcmp(pathname, file_table[fd].name)==0) {
      file_table[fd].file_offset = 0;
      if(fd > 2) {
        file_table[fd].read  = ramdisk_read;
        file_table[fd].write = ramdisk_write;
      }
// printf("----------------\n[fs_open]\n file %d: \"%s\"\n size: %p\n----------------\n", fd, pathname, file_table[fd].size);
      return fd;
    }

  assert(0);
}

size_t fs_read(int fd, void *buf, size_t len) {
  if(fd<=2) return 0; // ignore stdin, stdout and stderr

// printf("----------------\n[fs_read] fd %d\n from (%u + offset %d)\n size=%p\n----------------\n",
// fd, file_table[fd].disk_offset, file_table[fd].file_offset, len);

  file_table[fd].read(buf, file_table[fd].disk_offset + file_table[fd].file_offset, len);
  fs_lseek(fd, len, SEEK_CUR);
  return len;
}

size_t fs_write(int fd, const void *buf, size_t len) {
  file_table[fd].write(buf, file_table[fd].disk_offset + file_table[fd].file_offset, len);
  return fs_lseek(fd, len, SEEK_CUR);
}

int fs_close(int fd) {
  return 0;
}
