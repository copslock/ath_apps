cmd_/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/apps/art/modules/dk_func.o := mips-linux-uclibc-gcc -Wp,-MD,/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/apps/art/modules/.dk_func.o.d  -nostdinc -isystem /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/bin-ccache/../lib/gcc/mips-linux-uclibc/4.3.3/include -Iinclude  -I/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/linux/kernels/mips-linux-2.6.31/arch/mips/include -include include/linux/autoconf.h -D__KERNEL__ -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -Werror-implicit-function-declaration -Wno-format-security -fno-delete-null-pointer-checks -O2 -mno-check-zero-division -mabi=32 -G 0 -mno-abicalls -fno-pic -pipe -msoft-float -ffreestanding -march=74kc -Wa,-march=74kc -Wa,--trap -I/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/linux/kernels/mips-linux-2.6.31/arch/mips/include/asm/mach-atheros -I/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/linux/kernels/mips-linux-2.6.31/arch/mips/include/asm/mach-generic -D"VMLINUX_LOAD_ADDRESS=0xffffffff80002000" -fno-stack-protector -fomit-frame-pointer -funit-at-a-time -pipe -mtune=74kc -Os -Wdeclaration-after-statement -Wno-pointer-sign -fno-strict-overflow -I/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/apps/art/modules/include -DPCI_BUS -D__KERNEL__ -DMODULE -DAP83 -DAP83 -DWASP  -DMODULE -mlong-calls -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(dk_func)"  -D"KBUILD_MODNAME=KBUILD_STR(art)"  -c -o /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/apps/art/modules/dk_func.o /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/apps/art/modules/dk_func.c

deps_/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/apps/art/modules/dk_func.o := \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/apps/art/modules/dk_func.c \
  include/linux/mm.h \
    $(wildcard include/config/discontigmem.h) \
    $(wildcard include/config/sysctl.h) \
    $(wildcard include/config/mmu.h) \
    $(wildcard include/config/stack/growsup.h) \
    $(wildcard include/config/numa.h) \
    $(wildcard include/config/sparsemem.h) \
    $(wildcard include/config/sparsemem/vmemmap.h) \
    $(wildcard include/config/highmem.h) \
    $(wildcard include/config/swap.h) \
    $(wildcard include/config/shmem.h) \
    $(wildcard include/config/arch/populates/node/map.h) \
    $(wildcard include/config/have/arch/early/pfn/to/nid.h) \
    $(wildcard include/config/proc/fs.h) \
    $(wildcard include/config/ia64.h) \
    $(wildcard include/config/debug/pagealloc.h) \
    $(wildcard include/config/hibernation.h) \
  include/linux/errno.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/linux/kernels/mips-linux-2.6.31/arch/mips/include/asm/errno.h \
  include/asm-generic/errno-base.h \
  include/linux/gfp.h \
    $(wildcard include/config/kmemcheck.h) \
    $(wildcard include/config/zone/dma.h) \
    $(wildcard include/config/zone/dma32.h) \
    $(wildcard include/config/debug/vm.h) \
  include/linux/mmzone.h \
    $(wildcard include/config/force/max/zoneorder.h) \
    $(wildcard include/config/smp.h) \
    $(wildcard include/config/memory/hotplug.h) \
    $(wildcard include/config/flat/node/mem/map.h) \
    $(wildcard include/config/cgroup/mem/res/ctlr.h) \
    $(wildcard include/config/have/memory/present.h) \
    $(wildcard include/config/need/node/memmap/size.h) \
    $(wildcard include/config/need/multiple/nodes.h) \
    $(wildcard include/config/flatmem.h) \
    $(wildcard include/config/sparsemem/extreme.h) \
    $(wildcard include/config/nodes/span/other/nodes.h) \
    $(wildcard include/config/holes/in/zone.h) \
    $(wildcard include/config/arch/has/holes/memorymodel.h) \
  include/linux/spinlock.h \
    $(wildcard include/config/debug/spinlock.h) \
    $(wildcard include/config/generic/lockbreak.h) \
    $(wildcard include/config/preempt.h) \
    $(wildcard include/config/debug/lock/alloc.h) \
  include/linux/typecheck.h \
  include/linux/preempt.h \
    $(wildcard include/config/debug/preempt.h) \
    $(wildcard include/config/preempt/tracer.h) \
    $(wildcard include/config/preempt/notifiers.h) \
  include/linux/thread_info.h \
    $(wildcard include/config/compat.h) \
  include/linux/types.h \
    $(wildcard include/config/uid16.h) \
    $(wildcard include/config/lbdaf.h) \
    $(wildcard include/config/phys/addr/t/64bit.h) \
    $(wildcard include/config/64bit.h) \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/linux/kernels/mips-linux-2.6.31/arch/mips/include/asm/types.h \
    $(wildcard include/config/64bit/phys/addr.h) \
  include/asm-generic/int-ll64.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/linux/kernels/mips-linux-2.6.31/arch/mips/include/asm/bitsperlong.h \
  include/asm-generic/bitsperlong.h \
  include/linux/posix_types.h \
  include/linux/stddef.h \
  include/linux/compiler.h \
    $(wildcard include/config/trace/branch/profiling.h) \
    $(wildcard include/config/profile/all/branches.h) \
    $(wildcard include/config/enable/must/check.h) \
    $(wildcard include/config/enable/warn/deprecated.h) \
  include/linux/compiler-gcc.h \
    $(wildcard include/config/arch/supports/optimized/inlining.h) \
    $(wildcard include/config/optimize/inlining.h) \
  include/linux/compiler-gcc4.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/linux/kernels/mips-linux-2.6.31/arch/mips/include/asm/posix_types.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/linux/kernels/mips-linux-2.6.31/arch/mips/include/asm/sgidefs.h \
  include/linux/bitops.h \
    $(wildcard include/config/generic/find/first/bit.h) \
    $(wildcard include/config/generic/find/last/bit.h) \
    $(wildcard include/config/generic/find/next/bit.h) \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/linux/kernels/mips-linux-2.6.31/arch/mips/include/asm/bitops.h \
    $(wildcard include/config/cpu/mipsr2.h) \
  include/linux/irqflags.h \
    $(wildcard include/config/trace/irqflags.h) \
    $(wildcard include/config/irqsoff/tracer.h) \
    $(wildcard include/config/trace/irqflags/support.h) \
    $(wildcard include/config/x86.h) \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/linux/kernels/mips-linux-2.6.31/arch/mips/include/asm/irqflags.h \
    $(wildcard include/config/mips/mt/smtc.h) \
    $(wildcard include/config/irq/cpu.h) \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/linux/kernels/mips-linux-2.6.31/arch/mips/include/asm/hazards.h \
    $(wildcard include/config/cpu/cavium/octeon.h) \
    $(wildcard include/config/cpu/mipsr1.h) \
    $(wildcard include/config/mach/alchemy.h) \
    $(wildcard include/config/cpu/loongson2.h) \
    $(wildcard include/config/cpu/r10000.h) \
    $(wildcard include/config/cpu/r5500.h) \
    $(wildcard include/config/cpu/rm9000.h) \
    $(wildcard include/config/cpu/sb1.h) \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/linux/kernels/mips-linux-2.6.31/arch/mips/include/asm/cpu-features.h \
    $(wildcard include/config/32bit.h) \
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
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/linux/kernels/mips-linux-2.6.31/arch/mips/include/asm/thread_info.h \
    $(wildcard include/config/page/size/4kb.h) \
    $(wildcard include/config/page/size/8kb.h) \
    $(wildcard include/config/page/size/16kb.h) \
    $(wildcard include/config/page/size/32kb.h) \
    $(wildcard include/config/page/size/64kb.h) \
    $(wildcard include/config/debug/stack/usage.h) \
    $(wildcard include/config/mips32/o32.h) \
    $(wildcard include/config/mips32/n32.h) \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/linux/kernels/mips-linux-2.6.31/arch/mips/include/asm/processor.h \
    $(wildcard include/config/cavium/octeon/cvmseg/size.h) \
    $(wildcard include/config/mips/mt/fpaff.h) \
    $(wildcard include/config/cpu/has/prefetch.h) \
  include/linux/cpumask.h \
    $(wildcard include/config/disable/obsolete/cpumask/functions.h) \
    $(wildcard include/config/hotplug/cpu.h) \
    $(wildcard include/config/cpumask/offstack.h) \
    $(wildcard include/config/debug/per/cpu/maps.h) \
  include/linux/kernel.h \
    $(wildcard include/config/preempt/voluntary.h) \
    $(wildcard include/config/debug/spinlock/sleep.h) \
    $(wildcard include/config/prove/locking.h) \
    $(wildcard include/config/printk.h) \
    $(wildcard include/config/dynamic/debug.h) \
    $(wildcard include/config/ring/buffer.h) \
    $(wildcard include/config/tracing.h) \
    $(wildcard include/config/ftrace/mcount/record.h) \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/bin-ccache/../lib/gcc/mips-linux-uclibc/4.3.3/include/stdarg.h \
  include/linux/linkage.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/linux/kernels/mips-linux-2.6.31/arch/mips/include/asm/linkage.h \
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
  include/linux/threads.h \
    $(wildcard include/config/nr/cpus.h) \
    $(wildcard include/config/base/small.h) \
  include/linux/bitmap.h \
  include/linux/string.h \
    $(wildcard include/config/binary/printf.h) \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/linux/kernels/mips-linux-2.6.31/arch/mips/include/asm/string.h \
    $(wildcard include/config/cpu/r3000.h) \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/linux/kernels/mips-linux-2.6.31/arch/mips/include/asm/cachectl.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/linux/kernels/mips-linux-2.6.31/arch/mips/include/asm/mipsregs.h \
    $(wildcard include/config/cpu/vr41xx.h) \
    $(wildcard include/config/hugetlb/page.h) \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/linux/kernels/mips-linux-2.6.31/arch/mips/include/asm/prefetch.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/linux/kernels/mips-linux-2.6.31/arch/mips/include/asm/system.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/linux/kernels/mips-linux-2.6.31/arch/mips/include/asm/addrspace.h \
    $(wildcard include/config/cpu/r8000.h) \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/linux/kernels/mips-linux-2.6.31/arch/mips/include/asm/mach-generic/spaces.h \
    $(wildcard include/config/dma/noncoherent.h) \
  include/linux/const.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/linux/kernels/mips-linux-2.6.31/arch/mips/include/asm/cmpxchg.h \
  include/asm-generic/cmpxchg-local.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/linux/kernels/mips-linux-2.6.31/arch/mips/include/asm/dsp.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/linux/kernels/mips-linux-2.6.31/arch/mips/include/asm/watch.h \
    $(wildcard include/config/hardware/watchpoints.h) \
  include/linux/list.h \
    $(wildcard include/config/debug/list.h) \
  include/linux/poison.h \
  include/linux/prefetch.h \
  include/linux/stringify.h \
  include/linux/bottom_half.h \
  include/linux/spinlock_types.h \
  include/linux/spinlock_types_up.h \
  include/linux/lockdep.h \
    $(wildcard include/config/lockdep.h) \
    $(wildcard include/config/lock/stat.h) \
    $(wildcard include/config/generic/hardirqs.h) \
  include/linux/spinlock_up.h \
  include/linux/spinlock_api_up.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/linux/kernels/mips-linux-2.6.31/arch/mips/include/asm/atomic.h \
  include/asm-generic/atomic-long.h \
  include/linux/wait.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/linux/kernels/mips-linux-2.6.31/arch/mips/include/asm/current.h \
  include/linux/cache.h \
    $(wildcard include/config/arch/has/cache/line/size.h) \
  include/linux/numa.h \
    $(wildcard include/config/nodes/shift.h) \
  include/linux/init.h \
    $(wildcard include/config/modules.h) \
    $(wildcard include/config/hotplug.h) \
  include/linux/seqlock.h \
  include/linux/nodemask.h \
  include/linux/pageblock-flags.h \
    $(wildcard include/config/hugetlb/page/size/variable.h) \
  include/linux/bounds.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/linux/kernels/mips-linux-2.6.31/arch/mips/include/asm/page.h \
    $(wildcard include/config/cpu/mips32.h) \
  include/linux/pfn.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/linux/kernels/mips-linux-2.6.31/arch/mips/include/asm/io.h \
  include/asm-generic/iomap.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/linux/kernels/mips-linux-2.6.31/arch/mips/include/asm/pgtable-bits.h \
    $(wildcard include/config/cpu/tx39xx.h) \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/linux/kernels/mips-linux-2.6.31/arch/mips/include/asm/mach-generic/ioremap.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/linux/kernels/mips-linux-2.6.31/arch/mips/include/asm/mach-atheros/mangle-port.h \
    $(wildcard include/config/swap/io/space.h) \
  include/asm-generic/memory_model.h \
  include/asm-generic/getorder.h \
  include/linux/memory_hotplug.h \
    $(wildcard include/config/have/arch/nodedata/extension.h) \
    $(wildcard include/config/memory/hotremove.h) \
  include/linux/notifier.h \
  include/linux/mutex.h \
    $(wildcard include/config/debug/mutexes.h) \
  include/linux/rwsem.h \
    $(wildcard include/config/rwsem/generic/spinlock.h) \
  include/linux/rwsem-spinlock.h \
  include/linux/srcu.h \
  include/linux/topology.h \
    $(wildcard include/config/sched/smt.h) \
    $(wildcard include/config/sched/mc.h) \
  include/linux/smp.h \
    $(wildcard include/config/use/generic/smp/helpers.h) \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/linux/kernels/mips-linux-2.6.31/arch/mips/include/asm/topology.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/linux/kernels/mips-linux-2.6.31/arch/mips/include/asm/mach-generic/topology.h \
  include/asm-generic/topology.h \
  include/linux/mmdebug.h \
    $(wildcard include/config/debug/virtual.h) \
  include/linux/rbtree.h \
  include/linux/prio_tree.h \
  include/linux/debug_locks.h \
    $(wildcard include/config/debug/locking/api/selftests.h) \
  include/linux/mm_types.h \
    $(wildcard include/config/split/ptlock/cpus.h) \
    $(wildcard include/config/want/page/debug/flags.h) \
    $(wildcard include/config/mm/owner.h) \
    $(wildcard include/config/mmu/notifier.h) \
  include/linux/auxvec.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/linux/kernels/mips-linux-2.6.31/arch/mips/include/asm/auxvec.h \
  include/linux/completion.h \
  include/linux/page-debug-flags.h \
    $(wildcard include/config/page/poisoning.h) \
    $(wildcard include/config/page/debug/something/else.h) \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/linux/kernels/mips-linux-2.6.31/arch/mips/include/asm/mmu.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/linux/kernels/mips-linux-2.6.31/arch/mips/include/asm/pgtable.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/linux/kernels/mips-linux-2.6.31/arch/mips/include/asm/pgtable-32.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/linux/kernels/mips-linux-2.6.31/arch/mips/include/asm/fixmap.h \
    $(wildcard include/config/cpu/tx49xx.h) \
  include/asm-generic/pgtable-nopmd.h \
  include/asm-generic/pgtable-nopud.h \
  include/asm-generic/pgtable.h \
  include/linux/page-flags.h \
    $(wildcard include/config/pageflags/extended.h) \
    $(wildcard include/config/have/mlocked/page/bit.h) \
    $(wildcard include/config/ia64/uncached/allocator.h) \
    $(wildcard include/config/s390.h) \
  include/linux/vmstat.h \
    $(wildcard include/config/vm/event/counters.h) \
  include/linux/percpu.h \
    $(wildcard include/config/have/dynamic/per/cpu/area.h) \
    $(wildcard include/config/debug/kmemleak.h) \
  include/linux/slab.h \
    $(wildcard include/config/slab/debug.h) \
    $(wildcard include/config/debug/objects.h) \
    $(wildcard include/config/slub.h) \
    $(wildcard include/config/slob.h) \
    $(wildcard include/config/debug/slab.h) \
  include/linux/slab_def.h \
    $(wildcard include/config/kmemtrace.h) \
  include/linux/kmemtrace.h \
  include/trace/events/kmem.h \
  include/linux/tracepoint.h \
    $(wildcard include/config/tracepoints.h) \
  include/linux/rcupdate.h \
    $(wildcard include/config/classic/rcu.h) \
    $(wildcard include/config/tree/rcu.h) \
    $(wildcard include/config/preempt/rcu.h) \
  include/linux/rcuclassic.h \
    $(wildcard include/config/rcu/cpu/stall/detector.h) \
  include/trace/define_trace.h \
    $(wildcard include/config/event/tracing.h) \
  include/linux/kmalloc_sizes.h \
    $(wildcard include/config/ath/2x8.h) \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/linux/kernels/mips-linux-2.6.31/arch/mips/include/asm/percpu.h \
  include/asm-generic/percpu.h \
    $(wildcard include/config/have/setup/per/cpu/area.h) \
  include/linux/percpu-defs.h \
  include/linux/fs.h \
    $(wildcard include/config/dnotify.h) \
    $(wildcard include/config/sysfs.h) \
    $(wildcard include/config/quota.h) \
    $(wildcard include/config/fsnotify.h) \
    $(wildcard include/config/inotify.h) \
    $(wildcard include/config/security.h) \
    $(wildcard include/config/fs/posix/acl.h) \
    $(wildcard include/config/epoll.h) \
    $(wildcard include/config/debug/writecount.h) \
    $(wildcard include/config/file/locking.h) \
    $(wildcard include/config/auditsyscall.h) \
    $(wildcard include/config/block.h) \
    $(wildcard include/config/fs/xip.h) \
    $(wildcard include/config/migration.h) \
  include/linux/limits.h \
  include/linux/ioctl.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/linux/kernels/mips-linux-2.6.31/arch/mips/include/asm/ioctl.h \
  include/asm-generic/ioctl.h \
  include/linux/kdev_t.h \
  include/linux/dcache.h \
  include/linux/rculist.h \
  include/linux/path.h \
  include/linux/stat.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/linux/kernels/mips-linux-2.6.31/arch/mips/include/asm/stat.h \
  include/linux/time.h \
    $(wildcard include/config/arch/uses/gettimeoffset.h) \
  include/linux/math64.h \
  include/linux/kobject.h \
  include/linux/sysfs.h \
  include/linux/kref.h \
  include/linux/radix-tree.h \
  include/linux/pid.h \
  include/linux/capability.h \
    $(wildcard include/config/security/file/capabilities.h) \
  include/linux/semaphore.h \
  include/linux/fiemap.h \
  include/linux/quota.h \
  include/linux/dqblk_xfs.h \
  include/linux/dqblk_v1.h \
  include/linux/dqblk_v2.h \
  include/linux/dqblk_qtree.h \
  include/linux/nfs_fs_i.h \
  include/linux/nfs.h \
  include/linux/sunrpc/msg_prot.h \
  include/linux/fcntl.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/linux/kernels/mips-linux-2.6.31/arch/mips/include/asm/fcntl.h \
  include/asm-generic/fcntl.h \
  include/linux/err.h \
  include/linux/module.h \
    $(wildcard include/config/modversions.h) \
    $(wildcard include/config/unused/symbols.h) \
    $(wildcard include/config/kallsyms.h) \
    $(wildcard include/config/markers.h) \
    $(wildcard include/config/module/unload.h) \
    $(wildcard include/config/constructors.h) \
  include/linux/kmod.h \
  include/linux/elf.h \
  include/linux/elf-em.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/linux/kernels/mips-linux-2.6.31/arch/mips/include/asm/elf.h \
    $(wildcard include/config/mips32/compat.h) \
  include/linux/moduleparam.h \
    $(wildcard include/config/alpha.h) \
    $(wildcard include/config/ppc64.h) \
  include/linux/marker.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/linux/kernels/mips-linux-2.6.31/arch/mips/include/asm/local.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/linux/kernels/mips-linux-2.6.31/arch/mips/include/asm/module.h \
    $(wildcard include/config/cpu/mips32/r1.h) \
    $(wildcard include/config/cpu/mips32/r2.h) \
    $(wildcard include/config/cpu/mips64/r1.h) \
    $(wildcard include/config/cpu/mips64/r2.h) \
    $(wildcard include/config/cpu/r4300.h) \
    $(wildcard include/config/cpu/r4x00.h) \
    $(wildcard include/config/cpu/r5000.h) \
    $(wildcard include/config/cpu/r5432.h) \
    $(wildcard include/config/cpu/r6000.h) \
    $(wildcard include/config/cpu/nevada.h) \
    $(wildcard include/config/cpu/rm7000.h) \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/linux/kernels/mips-linux-2.6.31/arch/mips/include/asm/uaccess.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/apps/art/modules/include/dk.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/apps/art/modules/include/dk_ioctl.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/apps/art/modules/include/client.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/apps/art/modules/include/dk.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/apps/art/modules/include/dk_ioctl.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/apps/art/modules/include/dk_event.h \

/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/apps/art/modules/dk_func.o: $(deps_/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/apps/art/modules/dk_func.o)

$(deps_/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/apps/art/modules/dk_func.o):
