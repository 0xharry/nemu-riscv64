#include <device/map.h>

/* http://en.wikibooks.org/wiki/Serial_Programming/8250_UART_Programming */
// NOTE: this is compatible to 16550

#define SERIAL_PORT 0x3F8
#define SERIAL_MMIO 0xa10003F8

#define CH_OFFSET 0

static uint8_t *serial_base = NULL;


// 串口的工作：输出串口当前的内容到流
static void serial_io_handler(uint32_t offset, int len, bool is_write) {
  assert(len == 1);
  switch (offset) {
    /* We bind the serial port with the host stderr in NEMU. */
    case CH_OFFSET:
      if (is_write) putc(serial_base[0], stderr);
      else panic("do not support read");
      break;
    default: panic("do not support offset = %d", offset);
  }
}


void init_serial() {
  // 在IO空间io_space[]开辟 8字节 大小，串口本身地址为 serial_base
  serial_base = new_space(8);
  // 无论从端口还是内存访问串口，实际上都从 serial_base 读
  // 初始化端口映射IO
  add_pio_map("serial", SERIAL_PORT, serial_base, 8, serial_io_handler);
  // 初始化内存映射IO
  add_mmio_map("serial", SERIAL_MMIO, serial_base, 8, serial_io_handler);

}
