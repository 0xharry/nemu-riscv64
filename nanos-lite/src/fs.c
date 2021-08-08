#include <fs.h>

typedef size_t (*ReadFn) (void *buf, size_t offset, size_t len);
typedef size_t (*WriteFn) (const void *buf, size_t offset, size_t len);

extern size_t serial_write(const void *buf, size_t offset, size_t len);
// extern size_t events_read(void *buf, size_t offset, size_t len);
// extern size_t dispinfo_read(void *buf, size_t offset, size_t len);

// extern size_t fb_write(const void *buf, size_t offset, size_t len); 
// extern size_t dispinfo_write(const void *buf,size_t offset, size_t len) ;
typedef struct {
  char *name;           // fixed in nanos's initialization
  size_t size;          // fixed in nanos's initialization
  size_t disk_offset;   // fixed in nanos's initialization
  ReadFn read;
  WriteFn write;
  size_t file_offset;   // offset for each program file, start at 0;
} Finfo;

enum {FD_STDIN, FD_STDOUT, FD_STDERR, FD_DEV, FD_DISP, FD_FB};

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
  [FD_STDIN]  = {"stdin",      0, 0,invalid_read,  invalid_write},
  [FD_STDOUT] = {"stdout",     0, 0,invalid_read,  serial_write},
  [FD_STDERR] = {"stderr",     0, 0,invalid_read,  serial_write},
#include "files.h"
};

void init_fs() {
  // TODO: initialize the size of /dev/fb
  int w = io_read(AM_GPU_CONFIG).width;
  int h = io_read(AM_GPU_CONFIG).height;
  file_table[FD_FB].size = w*h*4;
}

int    fs_open(const char *pathname, int flags, int mode);
size_t fs_read(int fd, void *buf, size_t len);
size_t fs_write(int fd, const void *buf, size_t len);
size_t fs_lseek(int fd, size_t offset, int whence);
int    fs_close(int fd);

extern size_t ramdisk_read(void *buf, size_t offset, size_t len);
extern size_t ramdisk_write(const void *buf, size_t offset, size_t len) ;
char buf[4096];



#define total_file_num sizeof(file_table)/sizeof(Finfo)
//打开文件并返回对应的文件描述符
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

//从文件描述符中进行读取
//偏移量不能越界
size_t fs_read(int fd, void *buf, size_t len) {
  if(fd<=2) return 0; // ignore stdin, stdout and stderr

// printf("----------------\n[fs_read] fd %d\n from (%u + offset %d)\n size=%p\n----------------\n",
// fd, file_table[fd].disk_offset, file_table[fd].file_offset, len);

  file_table[fd].read(buf, file_table[fd].disk_offset + file_table[fd].file_offset, len);
  return fs_lseek(fd, len, SEEK_CUR);
}


// size_t fs_read_all(int fd,void *buf) {
//   return fs_read(fd, buf, file_table[fd].size);
// }

int fs_close(int fd) {
  return 0;
}


// buf -> fd
size_t fs_write(int fd, const void *buf, size_t len) {
  file_table[fd].write(buf, file_table[fd].disk_offset + file_table[fd].file_offset, len);
  return fs_lseek(fd, len, SEEK_CUR);
}
//根据whence修改offset
//为每一个已经打开的文件引入偏移量属性file_offset, 
//来记录目前文件操作的位置. 每次对文件读写了多少个字节, 偏移量就前进多少.
size_t fs_lseek(int fd, size_t offset, int whence) {
  Finfo * ff = &file_table[fd];

  switch(whence) {
    case SEEK_SET: ff->file_offset = offset;break;
    case SEEK_CUR: ff->file_offset +=offset;break;
    case SEEK_END: ff->file_offset = ff->size + offset;break;
    default: assert(0);
  }
  return ff->file_offset;
}