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

#define seg_buf_size 0x20000
static char* elf_buffer[sizeof(Elf_Ehdr)];
static char* pgm_buffer[sizeof(Elf_Phdr)];
static char* seg_buffer[seg_buf_size];
static Elf_Ehdr* elf_header;
static Elf_Phdr* pgm_header;
static const int zero_buf;
static uintptr_t loader(PCB *pcb, const char *filename) {
  // read ELF header, start at ${ramdisk_start} also 0
  ramdisk_read(elf_buffer, 0, sizeof(Elf_Ehdr));
  elf_header = (Elf_Ehdr*)elf_buffer;

#ifdef DEBUG
  // check elf magic number
  assert(*(uint32_t *)elf_header->e_ident == 0x464c457f);
#endif

  // read and analyze each program header
  for(int i=0; i<elf_header->e_phnum; ++i, pgm_header+=sizeof(Elf_Phdr)){
    ramdisk_read(pgm_buffer, elf_header->e_phoff + sizeof(Elf_Phdr)*i, sizeof(Elf_Phdr));
    pgm_header = (Elf_Phdr*)pgm_buffer;

    if(pgm_header->p_type == PT_LOAD) {
      // set segment '.bss' to zero 
      // # length  : pgm_header->p_memsz  - pgm_header->p_filesz Bytes
      // to memery
      // # start at: pgm_header->p_vaddr  + pgm_header->p_filesz
      // to elf
      // # start at: pgm_header->p_offset + pgm_header->p_filesz
      ramdisk_write(&zero_buf,
                     pgm_header->p_offset + pgm_header->p_filesz,
                     pgm_header->p_memsz  - pgm_header->p_filesz);

      // read segment and write into memery
      // # length  : pgm_header->p_memsz Bytes
      // from elf  
      // # start at: pgm_header->p_offset
      // to memery
      // # start at: pgm_header->p_vaddr
      ramdisk_read( seg_buffer, 
                    pgm_header->p_offset, 
                    pgm_header->p_memsz);
      memcpy((void*)pgm_header->p_vaddr, 
                    seg_buffer, 
                    pgm_header->p_memsz);
    }
  }
  return 0;//elf_header->e_entry;
}

void naive_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);
  Log("Jump to entry = %p", entry);
  ((void(*)())entry) ();
}
