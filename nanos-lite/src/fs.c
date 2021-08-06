#include <fs.h>
#include "/home/harry/ics2020/abstract-machine/am/include/am.h"
#define putnstr(buf, n) \
    for(size_t i=0; i<n; ++i) putch(*(char*)(buf+i))

extern size_t ramdisk_read(void *buf, size_t offset, size_t len);
extern size_t ramdisk_write(const void *buf, size_t offset, size_t len);
extern size_t get_ramdisk_size();

typedef size_t (*ReadFn) (void *buf, size_t offset, size_t len);
typedef size_t (*WriteFn) (const void *buf, size_t offset, size_t len);

typedef struct {
  char *name;           // fixed in nanos
  size_t size;          // fixed in nanos
  size_t disk_offset;   // fixed in nanos
  ReadFn read;
  WriteFn write;
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
  [FD_STDOUT] = {"stdout", 0, 0, invalid_read, invalid_write},
  [FD_STDERR] = {"stderr", 0, 0, invalid_read, invalid_write},
#include "files.h"
};

void init_fs() {
  // TODO: initialize the size of /dev/fb
}

#define total_file_num sizeof(file_table)/sizeof(Finfo)
static int offset_cur[total_file_num];
size_t fs_lseek(int fd, size_t offset, int whence) {
  switch (whence){
  case SEEK_SET:
    assert(file_table[fd].size <= offset);
    offset_cur[fd] = offset;
    return offset_cur[fd];

  case SEEK_CUR:
    offset_cur[fd] += offset;
    assert(offset_cur[fd] <= file_table[fd].size);
    return offset_cur[fd];

  case SEEK_END:
  default: assert(0);
  }
}

int fs_open(const char *pathname, int flags, int mode) {
  for(int fd=0; fd<total_file_num; ++fd)
    if(strcmp(pathname, file_table[fd].name)==0)
      return fd;

  assert(0);
}

size_t fs_read(int fd, void *buf, size_t len) {
  if(fd<=2) return 0; // ignore stdin, stdout and stderr
  assert(len <= file_table[fd].size);
  ramdisk_read(buf, file_table[fd].disk_offset + offset_cur[fd], len);
  return fs_lseek(fd, len, SEEK_CUR);
}

size_t fs_write(int fd, const void *buf, size_t len) {
  if(fd==1 || fd==2) {
    putnstr(buf, len);
    return len;
  }
  if(fd==0) return 0;
  assert(len <= file_table[fd].size);
  ramdisk_write(buf, file_table[fd].disk_offset + offset_cur[fd], len);
  return fs_lseek(fd, len, SEEK_CUR);
}

int fs_close(int fd) {
  return 0;
}
