cmd_/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/apps/art/modules/dk_isr.o := mips-linux-uclibc-gcc -Wp,-MD,/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/apps/art/modules/.dk_isr.o.d  -nostdinc -isystem /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/bin-ccache/../lib/gcc/mips-linux-uclibc/4.3.3/include -Iinclude  -I/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/linux/kernels/mips-linux-2.6.31/arch/mips/include -include include/linux/autoconf.h -D__KERNEL__ -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -Werror-implicit-function-declaration -Wno-format-security -fno-delete-null-pointer-checks -O2 -mno-check-zero-division -mabi=32 -G 0 -mno-abicalls -fno-pic -pipe -msoft-float -ffreestanding -march=74kc -Wa,-march=74kc -Wa,--trap -I/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/linux/kernels/mips-linux-2.6.31/arch/mips/include/asm/mach-atheros -I/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/linux/kernels/mips-linux-2.6.31/arch/mips/include/asm/mach-generic -D"VMLINUX_LOAD_ADDRESS=0xffffffff80002000" -fno-stack-protector -fomit-frame-pointer -funit-at-a-time -pipe -mtune=74kc -Os -Wdeclaration-after-statement -Wno-pointer-sign -fno-strict-overflow -I/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/apps/art/modules/include -DPCI_BUS -D__KERNEL__ -DMODULE -DAP83 -DAP83 -DWASP  -DMODULE -mlong-calls -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(dk_isr)"  -D"KBUILD_MODNAME=KBUILD_STR(art)"  -c -o /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/apps/art/modules/dk_isr.o /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/apps/art/modules/dk_isr.c

deps_/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/apps/art/modules/dk_isr.o := \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/apps/art/modules/dk_isr.c \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/linux/kernels/mips-linux-2.6.31/arch/mips/include/asm/ptrace.h \
    $(wildcard include/config/32bit.h) \
    $(wildcard include/config/cpu/has/smartmips.h) \
    $(wildcard include/config/mips/mt/smtc.h) \
    $(wildcard include/config/cpu/cavium/octeon.h) \
  include/linux/compiler.h \
    $(wildcard include/config/trace/branch/profiling.h) \
    $(wildcard include/config/profile/all/branches.h) \
    $(wildcard include/config/enable/must/check.h) \
    $(wildcard include/config/enable/warn/deprecated.h) \
  include/linux/compiler-gcc.h \
    $(wildcard include/config/arch/supports/optimized/inlining.h) \
    $(wildcard include/config/optimize/inlining.h) \
  include/linux/compiler-gcc4.h \
  include/linux/linkage.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/linux/kernels/mips-linux-2.6.31/arch/mips/include/asm/linkage.h \
  include/linux/types.h \
    $(wildcard include/config/uid16.h) \
    $(wildcard include/config/lbdaf.h) \
    $(wildcard include/config/phys/addr/t/64bit.h) \
    $(wildcard include/config/64bit.h) \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/linux/kernels/mips-linux-2.6.31/arch/mips/include/asm/types.h \
    $(wildcard include/config/highmem.h) \
    $(wildcard include/config/64bit/phys/addr.h) \
  include/asm-generic/int-ll64.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/linux/kernels/mips-linux-2.6.31/arch/mips/include/asm/bitsperlong.h \
  include/asm-generic/bitsperlong.h \
  include/linux/posix_types.h \
  include/linux/stddef.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/linux/kernels/mips-linux-2.6.31/arch/mips/include/asm/posix_types.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/linux/kernels/mips-linux-2.6.31/arch/mips/include/asm/sgidefs.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/linux/kernels/mips-linux-2.6.31/arch/mips/include/asm/isadep.h \
    $(wildcard include/config/cpu/r3000.h) \
    $(wildcard include/config/cpu/tx39xx.h) \
  include/linux/kernel.h \
    $(wildcard include/config/preempt/voluntary.h) \
    $(wildcard include/config/debug/spinlock/sleep.h) \
    $(wildcard include/config/prove/locking.h) \
    $(wildcard include/config/printk.h) \
    $(wildcard include/config/dynamic/debug.h) \
    $(wildcard include/config/ring/buffer.h) \
    $(wildcard include/config/tracing.h) \
    $(wildcard include/config/numa.h) \
    $(wildcard include/config/ftrace/mcount/record.h) \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/bin-ccache/../lib/gcc/mips-linux-uclibc/4.3.3/include/stdarg.h \
  include/linux/bitops.h \
    $(wildcard include/config/generic/find/first/bit.h) \
    $(wildcard include/config/generic/find/last/bit.h) \
    $(wildcard include/config/generic/find/next/bit.h) \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/linux/kernels/mips-linux-2.6.31/arch/mips/include/asm/bitops.h \
    $(wildcard include/config/cpu/mipsr2.h) \
  include/linux/irqflags.h \
    $(wildcard include/config/trace/irqflags.h) \
    $(wildcard include/config/irqsoff/tracer.h) \
    $(wildcard include/config/preempt/tracer.h) \
    $(wildcard include/config/trace/irqflags/support.h) \
    $(wildcard include/config/x86.h) \
  include/linux/typecheck.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/linux/kernels/mips-linux-2.6.31/arch/mips/include/asm/irqflags.h \
    $(wildcard include/config/irq/cpu.h) \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/linux/kernels/mips-linux-2.6.31/arch/mips/include/asm/hazards.h \
    $(wildcard include/config/cpu/mipsr1.h) \
    $(wildcard include/config/mach/alchemy.h) \
    $(wildcard include/config/cpu/loongson2.h) \
    $(wildcard include/config/cpu/r10000.h) \
    $(wildcard include/config/cpu/r5500.h) \
    $(wildcard include/config/cpu/rm9000.h) \
    $(wildcard include/config/cpu/sb1.h) \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/linux/kernels/mips-linux-2.6.31/arch/mips/include/asm/cpu-features.h \
    $(wildcard include/config/smp.h) \
    $(wildcard include/config/cpu/mipsr2/irq/vi.h) \
    $(wildcard include/config/cpu/mipsr2/irq/ei.h) \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/linux/kernels/mips-linux-2.6.31/arch/mips/include/asm/cpu.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/linux/kernels/mips-linux-2.6.31/arch/mips/include/asm/cpu-info.h \
    $(wildcard include/config/mips/mt/smp.h) \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/linux/kernels/mips-linux-2.6.31/arch/mips/include/asm/cache.h \
    $(wildcard include/config/mips/l1/cache/shift.h) \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/linux/kernels/mips-linux-2.6.31/arch/mips/include/asm/mach-atheros/kmalloc.h \
    $(wildcard include/config/dma/coherent.h) \
    $(wildcard include/config/kmalloc/minalign/64byte.h) \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/linux/kernels/mips-linux-2.6.31/arch/mips/include/asm/mach-atheros/cpu-feature-overrides.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/linux/kernels/mips-linux-2.6.31/arch/mips/include/asm/barrier.h \
    $(wildcard include/config/cpu/has/sync.h) \
    $(wildcard include/config/sgi/ip28.h) \
    $(wildcard include/config/cpu/has/wb.h) \
    $(wildcard include/config/weak/ordering.h) \
    $(wildcard include/config/weak/reordering/beyond/llsc.h) \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/linux/kernels/mips-linux-2.6.31/arch/mips/include/asm/bug.h \
    $(wildcard include/config/bug.h) \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/linux/kernels/mips-linux-2.6.31/arch/mips/include/asm/break.h \
  include/asm-generic/bug.h \
    $(wildcard include/config/generic/bug.h) \
    $(wildcard include/config/generic/bug/relative/pointers.h) \
    $(wildcard include/config/debug/bugverbose.h) \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/linux/kernels/mips-linux-2.6.31/arch/mips/include/asm/byteorder.h \
  include/linux/byteorder/big_endian.h \
  include/linux/swab.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/linux/kernels/mips-linux-2.6.31/arch/mips/include/asm/swab.h \
  include/linux/byteorder/generic.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/linux/kernels/mips-linux-2.6.31/arch/mips/include/asm/war.h \
    $(wildcard include/config/cpu/r4000/workarounds.h) \
    $(wildcard include/config/cpu/r4400/workarounds.h) \
    $(wildcard include/config/cpu/daddi/workarounds.h) \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/linux/kernels/mips-linux-2.6.31/arch/mips/include/asm/mach-atheros/war.h \
  include/asm-generic/bitops/non-atomic.h \
  include/asm-generic/bitops/fls64.h \
  include/asm-generic/bitops/ffz.h \
  include/asm-generic/bitops/find.h \
  include/asm-generic/bitops/sched.h \
  include/asm-generic/bitops/hweight.h \
  include/asm-generic/bitops/ext2-non-atomic.h \
  include/asm-generic/bitops/le.h \
  include/asm-generic/bitops/ext2-atomic.h \
  include/asm-generic/bitops/minix.h \
  include/linux/log2.h \
    $(wildcard include/config/arch/has/ilog2/u32.h) \
    $(wildcard include/config/arch/has/ilog2/u64.h) \
  include/linux/ratelimit.h \
  include/linux/param.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/linux/kernels/mips-linux-2.6.31/arch/mips/include/asm/param.h \
    $(wildcard include/config/hz.h) \
  include/linux/dynamic_debug.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/linux/kernels/mips-linux-2.6.31/arch/mips/include/asm/div64.h \
  include/asm-generic/div64.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/apps/art/modules/include/dk.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/apps/art/modules/include/client.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/apps/art/modules/include/dk.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/apps/art/modules/include/dk_ioctl.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/apps/art/modules/include/dk_event.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/apps/art/modules/include/dk_event.h \

/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/apps/art/modules/dk_isr.o: $(deps_/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/apps/art/modules/dk_isr.o)

$(deps_/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/apps/art/modules/dk_isr.o):
