cmd_arch/x86/kernel/entry_32.o := gcc -Wp,-MD,arch/x86/kernel/.entry_32.o.d  -nostdinc -isystem /usr/lib/gcc/i686-linux-gnu/4.8/include  -I/home/mkytr/Desktop/Project1/linux-source-3.13.0/arch/x86/include -Iarch/x86/include/generated  -Iinclude -I/home/mkytr/Desktop/Project1/linux-source-3.13.0/arch/x86/include/uapi -Iarch/x86/include/generated/uapi -I/home/mkytr/Desktop/Project1/linux-source-3.13.0/include/uapi -Iinclude/generated/uapi -include /home/mkytr/Desktop/Project1/linux-source-3.13.0/include/linux/kconfig.h -Iubuntu/include  -D__KERNEL__  -D__ASSEMBLY__ -m32 -DCONFIG_AS_CFI=1 -DCONFIG_AS_CFI_SIGNAL_FRAME=1 -DCONFIG_AS_CFI_SECTIONS=1  -DCONFIG_AS_AVX=1 -DCONFIG_AS_AVX2=1           -c -o arch/x86/kernel/entry_32.o arch/x86/kernel/entry_32.S

source_arch/x86/kernel/entry_32.o := arch/x86/kernel/entry_32.S

deps_arch/x86/kernel/entry_32.o := \
    $(wildcard include/config/x86/32/lazy/gs.h) \
    $(wildcard include/config/auditsyscall.h) \
    $(wildcard include/config/preempt.h) \
    $(wildcard include/config/trace/irqflags.h) \
    $(wildcard include/config/vm86.h) \
    $(wildcard include/config/x86/espfix32.h) \
    $(wildcard include/config/paravirt.h) \
    $(wildcard include/config/x86/l1/cache/shift.h) \
    $(wildcard include/config/tracing.h) \
    $(wildcard include/config/x86/invd/bug.h) \
    $(wildcard include/config/x86/mce.h) \
    $(wildcard include/config/xen.h) \
    $(wildcard include/config/hyperv.h) \
    $(wildcard include/config/function/tracer.h) \
    $(wildcard include/config/dynamic/ftrace.h) \
    $(wildcard include/config/function/graph/tracer.h) \
    $(wildcard include/config/kvm/guest.h) \
  include/linux/linkage.h \
  include/linux/compiler.h \
    $(wildcard include/config/sparse/rcu/pointer.h) \
    $(wildcard include/config/trace/branch/profiling.h) \
    $(wildcard include/config/profile/all/branches.h) \
    $(wildcard include/config/enable/must/check.h) \
    $(wildcard include/config/enable/warn/deprecated.h) \
    $(wildcard include/config/kprobes.h) \
  include/linux/stringify.h \
  include/linux/export.h \
    $(wildcard include/config/have/underscore/symbol/prefix.h) \
    $(wildcard include/config/modules.h) \
    $(wildcard include/config/modversions.h) \
    $(wildcard include/config/unused/symbols.h) \
  /home/mkytr/Desktop/Project1/linux-source-3.13.0/arch/x86/include/asm/linkage.h \
    $(wildcard include/config/x86/32.h) \
    $(wildcard include/config/x86/64.h) \
    $(wildcard include/config/x86/alignment/16.h) \
  include/linux/err.h \
  /home/mkytr/Desktop/Project1/linux-source-3.13.0/arch/x86/include/uapi/asm/errno.h \
  /home/mkytr/Desktop/Project1/linux-source-3.13.0/include/uapi/asm-generic/errno.h \
  /home/mkytr/Desktop/Project1/linux-source-3.13.0/include/uapi/asm-generic/errno-base.h \
  /home/mkytr/Desktop/Project1/linux-source-3.13.0/arch/x86/include/asm/thread_info.h \
    $(wildcard include/config/ia32/emulation.h) \
  /home/mkytr/Desktop/Project1/linux-source-3.13.0/arch/x86/include/asm/page.h \
  include/linux/types.h \
    $(wildcard include/config/uid16.h) \
    $(wildcard include/config/lbdaf.h) \
    $(wildcard include/config/arch/dma/addr/t/64bit.h) \
    $(wildcard include/config/phys/addr/t/64bit.h) \
    $(wildcard include/config/64bit.h) \
  include/uapi/linux/types.h \
  /home/mkytr/Desktop/Project1/linux-source-3.13.0/arch/x86/include/uapi/asm/types.h \
  /home/mkytr/Desktop/Project1/linux-source-3.13.0/include/uapi/asm-generic/types.h \
  include/asm-generic/int-ll64.h \
  include/uapi/asm-generic/int-ll64.h \
  /home/mkytr/Desktop/Project1/linux-source-3.13.0/arch/x86/include/uapi/asm/bitsperlong.h \
  include/asm-generic/bitsperlong.h \
  include/uapi/asm-generic/bitsperlong.h \
  /home/mkytr/Desktop/Project1/linux-source-3.13.0/arch/x86/include/asm/page_types.h \
    $(wildcard include/config/physical/start.h) \
    $(wildcard include/config/physical/align.h) \
  /home/mkytr/Desktop/Project1/linux-source-3.13.0/include/uapi/linux/const.h \
  /home/mkytr/Desktop/Project1/linux-source-3.13.0/arch/x86/include/asm/page_32_types.h \
    $(wildcard include/config/highmem4g.h) \
    $(wildcard include/config/highmem64g.h) \
    $(wildcard include/config/page/offset.h) \
    $(wildcard include/config/x86/pae.h) \
  /home/mkytr/Desktop/Project1/linux-source-3.13.0/arch/x86/include/asm/page_32.h \
    $(wildcard include/config/hugetlb/page.h) \
    $(wildcard include/config/debug/virtual.h) \
    $(wildcard include/config/flatmem.h) \
    $(wildcard include/config/x86/use/3dnow.h) \
    $(wildcard include/config/x86/3dnow.h) \
  include/asm-generic/memory_model.h \
    $(wildcard include/config/discontigmem.h) \
    $(wildcard include/config/sparsemem/vmemmap.h) \
    $(wildcard include/config/sparsemem.h) \
  include/asm-generic/getorder.h \
  /home/mkytr/Desktop/Project1/linux-source-3.13.0/arch/x86/include/asm/asm-offsets.h \
  include/generated/asm-offsets.h \
  /home/mkytr/Desktop/Project1/linux-source-3.13.0/arch/x86/include/asm/irqflags.h \
    $(wildcard include/config/debug/lock/alloc.h) \
  /home/mkytr/Desktop/Project1/linux-source-3.13.0/arch/x86/include/asm/processor-flags.h \
  /home/mkytr/Desktop/Project1/linux-source-3.13.0/arch/x86/include/uapi/asm/processor-flags.h \
  /home/mkytr/Desktop/Project1/linux-source-3.13.0/arch/x86/include/asm/segment.h \
    $(wildcard include/config/smp.h) \
    $(wildcard include/config/cc/stackprotector.h) \
  /home/mkytr/Desktop/Project1/linux-source-3.13.0/arch/x86/include/asm/smp.h \
    $(wildcard include/config/x86/local/apic.h) \
    $(wildcard include/config/x86/io/apic.h) \
    $(wildcard include/config/x86/32/smp.h) \
    $(wildcard include/config/x86/64/smp.h) \
    $(wildcard include/config/debug/nmi/selftest.h) \
  /home/mkytr/Desktop/Project1/linux-source-3.13.0/arch/x86/include/asm/percpu.h \
    $(wildcard include/config/x86/cmpxchg64.h) \
  /home/mkytr/Desktop/Project1/linux-source-3.13.0/arch/x86/include/asm/dwarf2.h \
    $(wildcard include/config/as/cfi.h) \
    $(wildcard include/config/as/cfi/signal/frame.h) \
    $(wildcard include/config/as/cfi/sections.h) \
  /home/mkytr/Desktop/Project1/linux-source-3.13.0/arch/x86/include/asm/ftrace.h \
    $(wildcard include/config/ftrace/syscalls.h) \
  /home/mkytr/Desktop/Project1/linux-source-3.13.0/arch/x86/include/asm/irq_vectors.h \
    $(wildcard include/config/have/kvm.h) \
  include/linux/threads.h \
    $(wildcard include/config/nr/cpus.h) \
    $(wildcard include/config/base/small.h) \
  /home/mkytr/Desktop/Project1/linux-source-3.13.0/arch/x86/include/asm/cpufeature.h \
    $(wildcard include/config/x86/debug/static/cpu/has.h) \
  /home/mkytr/Desktop/Project1/linux-source-3.13.0/arch/x86/include/asm/required-features.h \
    $(wildcard include/config/x86/minimum/cpu/family.h) \
    $(wildcard include/config/math/emulation.h) \
    $(wildcard include/config/x86/cmov.h) \
    $(wildcard include/config/x86/p6/nop.h) \
    $(wildcard include/config/matom.h) \
  /home/mkytr/Desktop/Project1/linux-source-3.13.0/arch/x86/include/asm/alternative-asm.h \
  /home/mkytr/Desktop/Project1/linux-source-3.13.0/arch/x86/include/asm/asm.h \
  /home/mkytr/Desktop/Project1/linux-source-3.13.0/arch/x86/include/asm/smap.h \
    $(wildcard include/config/x86/smap.h) \
  /home/mkytr/Desktop/Project1/linux-source-3.13.0/arch/x86/include/asm/nops.h \
    $(wildcard include/config/mk7.h) \
  /home/mkytr/Desktop/Project1/linux-source-3.13.0/include/uapi/linux/elf-em.h \
  /home/mkytr/Desktop/Project1/linux-source-3.13.0/arch/x86/include/asm/entry_arch.h \
    $(wildcard include/config/irq/work.h) \
    $(wildcard include/config/x86/thermal/vector.h) \
    $(wildcard include/config/x86/mce/threshold.h) \

arch/x86/kernel/entry_32.o: $(deps_arch/x86/kernel/entry_32.o)

$(deps_arch/x86/kernel/entry_32.o):
