#include <proc.h>
#include <elf.h>
#include "fs.h"


#ifdef __LP64__
# define Elf_Ehdr Elf64_Ehdr
# define Elf_Phdr Elf64_Phdr
#else
# define Elf_Ehdr Elf32_Ehdr
# define Elf_Phdr Elf32_Phdr
#endif

#if defined(__ISA_AM_NATIVE__)
# define EXPECT_TYPE EM_X86_64
#elif defined(__ISA_RISCV64__)
# define EXPECT_TYPE EM_RISCV  // see /usr/include/elf.h to get the right type 
#else
# error Unsupported ISA
#endif

#define RAMDISK_OFFSET 0

extern size_t ramdisk_read(void *buf, size_t offset, size_t len);
extern size_t get_ramdisk_size();
extern int    fs_open(const char *pathname, int flags, int mode);
extern size_t fs_read(int fd, void *buf, size_t len);
extern int    fs_close(int fd);
// extern size_t fs_read_all(int fd,void *buf);
extern size_t fs_lseek(int fd, size_t offset, int whence);

/*加载的过程就是把可执行文件中的代码和数据放置在正确的内存位置, 然后跳转到程序入口
可执行文件从filename引入
找出每一个需要加载的segment的Offset, VirtAddr, FileSiz和MemSiz这些参数
*/
//尝试将loader载入的过程打印出来
static uintptr_t loader(PCB *pcb, const char *filename) {

  int fd = fs_open(filename,0,0);
  assert(fd>=0);


  Elf_Ehdr eh;
  //fd -> buf 
  // printf("in loader read to %p\n",&eh);
  fs_read(fd,&eh,sizeof(Elf_Ehdr));
  // printf("phdr_offset: 0x%x\nehdr.ephnum: %d\n", eh.e_phoff, eh.e_phnum);
  // const uint32_t ELF_MAGIC = 0x464c457f;
  // assert(*(uint32_t *)buf==ELF_MAGIC);
  Elf_Phdr ph;
  int i;
  for(i=0;i<eh.e_phnum;i++) {
    //phentsize是每一个表项的大小  all entries are the same size
    //找到program header table的位置读取第i个表项
    //先读取每一个phdr表项
    fs_lseek(fd,eh.e_phoff+i*eh.e_phentsize,SEEK_SET);
    fs_read(fd,&ph,sizeof(Elf_Phdr));
    if(ph.p_type==PT_LOAD) {
    //p_offset给出本段内容在文件中的位置，即段内容的开始位置相对于文件开头的偏移量
      fs_lseek(fd,ph.p_offset,SEEK_SET);
    //给出本段内容在文件中的大小
      fs_read(fd,(void*)ph.p_vaddr,ph.p_filesz);
    //填充.bss全0
      memset((void *)ph.p_vaddr+ph.p_filesz, 0, ph.p_memsz-ph.p_filesz);
    }
  
  }

  fs_close(fd);
  uintptr_t entry = eh.e_entry;
  return entry;
}

void naive_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);
  //printf("entry 0x%x\n",entry);
  Log("Jump to entry = %p", entry);
  ((void(*)())entry) ();
}

