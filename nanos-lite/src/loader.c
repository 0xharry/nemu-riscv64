#include <proc.h>
#include <elf.h>
// #include "/home/harry/ics2020/nemu/include/memory/paddr.h"
#ifdef __LP64__
# define Elf_Ehdr Elf64_Ehdr
# define Elf_Phdr Elf64_Phdr
#else
# define Elf_Ehdr Elf32_Ehdr
# define Elf_Phdr Elf32_Phdr
#endif

extern size_t ramdisk_read(void *buf, size_t offset, size_t len);
extern size_t ramdisk_write(const void *buf, size_t offset, size_t len);
extern size_t get_ramdisk_size();

static char* elf_buffer[sizeof(Elf_Ehdr)];
static char* pgm_buffer[sizeof(Elf_Phdr)];
static Elf_Ehdr* elf_header;
static Elf_Phdr* pgm_header;
// static const int zero_buf;
static uintptr_t loader(PCB *pcb, const char *filename) {
// read ELF header, start at ${ramdisk_start} also 0
  ramdisk_read(elf_buffer, 0, sizeof(Elf_Ehdr));
  elf_header = (Elf_Ehdr*)elf_buffer;
  assert(*(uint32_t *)elf_header->e_ident == 0x464c457f);

// read program headers, start at elf_header->e_phoff
  ramdisk_read(pgm_buffer, elf_header->e_phoff, sizeof(Elf_Phdr));
  pgm_header = (Elf_Phdr*)pgm_buffer;

  // analyze each program header
  for(int i=0; i<elf_header->e_phnum; ++i, pgm_header+=sizeof(Elf_Phdr)){
    // .bss set to zero
    Log("p_offset=%ld\npgm_header=%ld",pgm_header->p_offset,pgm_header);
    assert((unsigned long)pgm_header->p_offset==(unsigned long)pgm_header);
    // ramdisk_write((const void*)&zero_buf, pgm_header->p_offset,
    //                 pgm_header->p_memsz - pgm_header->p_filesz);
    // // write into memery
    // for(int j=0; j<pgm_header->p_memsz; j+=pgm_header->p_align)
    //   paddr_write(pgm_header->p_vaddr+j,
    //               ramdisk_read(pgm_buffer,pgm_header+j,pgm_header->p_align),
    //               pgm_header->p_align);
  }
  return elf_header->e_entry;
}

void naive_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);
  Log("Jump to entry = %p", entry);
  ((void(*)())entry) ();
}

