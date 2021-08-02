#include <proc.h>
#include <elf.h>

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

static char* header_buffer[sizeof(Elf_Ehdr)];
static Elf_Ehdr* elf_header;
static size_t elf_offset;
static uintptr_t loader(PCB *pcb, const char *filename) {
// read ELF header, start at ${ramdisk_start}
  elf_offset = ramdisk_read(header_buffer, 0, sizeof(Elf_Ehdr));
  elf_header = (Elf_Ehdr*)header_buffer;
  assert(*(uint32_t *)elf_header->e_ident == 0x464c457f);
  return elf_header->e_entry;
}

void naive_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);
  Log("Jump to entry = %p", entry);
  ((void(*)())entry) ();
}

