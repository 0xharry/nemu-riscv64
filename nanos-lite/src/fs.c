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



//打开文件并返回对应的文件描述符
int fs_open(const char *pathname, int flags, int mode) {
  //如何找到pathname? 在file_table中找
  //忽略权限位 flag
  //使用ramdisk_read 进行真正的读取
  //偏移量不要越界
  
  // Finfo * ff;
  
  //printf("get in fs_open\n");
  bool find = false;
  int sum = sizeof(file_table)/sizeof(Finfo);
  //printf("sum is %d\n",sum);
  int i,ret = 0;
  for(i=0;i<sum;i++) {
    //printf("第%d个 : %s\n",i,file_table[i].name);
    if(strcmp(pathname,file_table[i].name)==0) {
      file_table[i].file_offset = 0;
      if(i>=3){
        file_table[i].read  = ramdisk_read;
        file_table[i].write = ramdisk_write;
      }
      find = true;
      ret = i;
      break;
    }
  }  
  assert(find==true);
  return ret;
}

//从文件描述符中进行读取
//偏移量不能越界
size_t fs_read(int fd, void *buf, size_t len) {
  //printf("get in fs_read\n");
  Finfo * ff = &file_table[fd];
  //printf("ff-addr : %p, buf-addr: %p, len-addr: %p\n",ff,buf,&len);
  assert(ff);

  // 读指针越界 || 读指针地址+读取长度 越界
  //printf("ff->file_offset : %d,  ff->size : %d\n",ff->file_offset,ff->size);
  assert( ff->file_offset <= ff->size );

  // if(ff->file_offset + len > ff->size) {
  //   len = ff->size - ff->file_offset;
  //   assert( len <= ff->size );
  // }
  /* read `len' bytes starting from `offset' of ramdisk into `buf' */
  //printf("disk_offset is 0x%x\n",ff->disk_offset);
  size_t count = ff->read(buf,ff->disk_offset + ff->file_offset,len);
  fs_lseek(fd,count,SEEK_CUR); //注意更新!
  return count;
}


// size_t fs_read_all(int fd,void *buf) {
//   return fs_read(fd, buf, file_table[fd].size);
// }

int fs_close(int fd) {
  return 0;
}


// buf -> fd
size_t fs_write(int fd, const void *buf, size_t len) {
  
  Finfo * ff = &file_table[fd];
  assert(ff);
  // printf("ff-addr : %p, buf-addr: %p\n",ff,buf);
  // 写指针越界 || 读写针地址+写长度 越界
  // printf("fd is %d\n",fd);
  // printf("ff->file_offset : %d,  ff->size : %d\n",ff->file_offset,ff->size);

  // if(fd==1 || fd==2) {
  //   int i;
  //   for(i=0;i<len;i++) {
  //     putch(*(char *)(buf+i));
  //   }
  //   return len;
  // }
  // else {
    //assert( ff->file_offset <= ff->size );

  // if(ff->file_offset + len > ff->size) {
  //   len = ff->size - ff->file_offset;
  //   assert( ff->file_offset <= ff->size );
  // }
    /* write `len' bytes starting from `buf' into the `offset' of ramdisk */
    size_t count = ff->write(buf,ff->disk_offset + ff->file_offset,len);

  // int i;
  // for(i=0;i<count;i++) {
  //   putch(*(char *)(buf+i));
  // }
    fs_lseek(fd,count,SEEK_CUR);
    return count;
  // }
}

//根据whence修改offset
//为每一个已经打开的文件引入偏移量属性file_offset, 
//来记录目前文件操作的位置. 每次对文件读写了多少个字节, 偏移量就前进多少.
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