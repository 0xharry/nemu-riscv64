#参考https://gcc.gnu.org/onlinedocs/gcc/RISC-V-Options.html
CROSS_COMPILE := riscv64-linux-gnu-
#-fno-pic生成位置无关代码 rv64g???
COMMON_FLAGS  := -fno-pic -march=rv64g -mcmodel=medany
CFLAGS        += $(COMMON_FLAGS) -static
ASFLAGS       += $(COMMON_FLAGS) -O0
LDFLAGS       += -melf64lriscv
