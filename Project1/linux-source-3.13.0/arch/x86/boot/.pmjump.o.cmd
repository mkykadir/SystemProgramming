cmd_arch/x86/boot/pmjump.o := gcc -Wp,-MD,arch/x86/boot/.pmjump.o.d  -nostdinc -isystem /usr/lib/gcc/i686-linux-gnu/4.8/include  -I/home/mkytr/Desktop/Project1/linux-source-3.13.0/arch/x86/include -Iarch/x86/include/generated  -Iinclude -I/home/mkytr/Desktop/Project1/linux-source-3.13.0/arch/x86/include/uapi -Iarch/x86/include/generated/uapi -I/home/mkytr/Desktop/Project1/linux-source-3.13.0/include/uapi -Iinclude/generated/uapi -include /home/mkytr/Desktop/Project1/linux-source-3.13.0/include/linux/kconfig.h -Iubuntu/include  -D__KERNEL__   -m32 -g -Os -D_SETUP -D__KERNEL__ -DDISABLE_BRANCH_PROFILING -Wall -Wstrict-prototypes -march=i386 -mregparm=3 -include /home/mkytr/Desktop/Project1/linux-source-3.13.0/arch/x86/boot/code16gcc.h -fno-strict-aliasing -fomit-frame-pointer -fno-pic -mno-mmx -mno-sse  -ffreestanding  -fno-toplevel-reorder  -fno-stack-protector  -mpreferred-stack-boundary=2 -D__ASSEMBLY__   -DSVGA_MODE=NORMAL_VGA    -c -o arch/x86/boot/pmjump.o arch/x86/boot/pmjump.S

source_arch/x86/boot/pmjump.o := arch/x86/boot/pmjump.S

deps_arch/x86/boot/pmjump.o := \
  /home/mkytr/Desktop/Project1/linux-source-3.13.0/arch/x86/boot/code16gcc.h \
  /home/mkytr/Desktop/Project1/linux-source-3.13.0/arch/x86/include/asm/boot.h \
    $(wildcard include/config/physical/start.h) \
    $(wildcard include/config/physical/align.h) \
    $(wildcard include/config/x86/64.h) \
    $(wildcard include/config/kernel/bzip2.h) \
  /home/mkytr/Desktop/Project1/linux-source-3.13.0/arch/x86/include/asm/pgtable_types.h \
    $(wildcard include/config/kmemcheck.h) \
    $(wildcard include/config/mem/soft/dirty.h) \
    $(wildcard include/config/x86/pae.h) \
    $(wildcard include/config/compat/vdso.h) \
    $(wildcard include/config/x86/32.h) \
    $(wildcard include/config/proc/fs.h) \
  /home/mkytr/Desktop/Project1/linux-source-3.13.0/include/uapi/linux/const.h \
  /home/mkytr/Desktop/Project1/linux-source-3.13.0/arch/x86/include/asm/page_types.h \
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
  /home/mkytr/Desktop/Project1/linux-source-3.13.0/arch/x86/include/asm/page_32_types.h \
    $(wildcard include/config/highmem4g.h) \
    $(wildcard include/config/highmem64g.h) \
    $(wildcard include/config/page/offset.h) \
  /home/mkytr/Desktop/Project1/linux-source-3.13.0/arch/x86/include/asm/pgtable_32_types.h \
    $(wildcard include/config/highmem.h) \
  /home/mkytr/Desktop/Project1/linux-source-3.13.0/arch/x86/include/asm/pgtable-2level_types.h \
  /home/mkytr/Desktop/Project1/linux-source-3.13.0/arch/x86/include/uapi/asm/boot.h \
  /home/mkytr/Desktop/Project1/linux-source-3.13.0/arch/x86/include/asm/processor-flags.h \
    $(wildcard include/config/vm86.h) \
  /home/mkytr/Desktop/Project1/linux-source-3.13.0/arch/x86/include/uapi/asm/processor-flags.h \
  /home/mkytr/Desktop/Project1/linux-source-3.13.0/arch/x86/include/asm/segment.h \
    $(wildcard include/config/smp.h) \
    $(wildcard include/config/cc/stackprotector.h) \
    $(wildcard include/config/paravirt.h) \
    $(wildcard include/config/tracing.h) \
    $(wildcard include/config/x86/32/lazy/gs.h) \
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
    $(wildcard include/config/x86/alignment/16.h) \

arch/x86/boot/pmjump.o: $(deps_arch/x86/boot/pmjump.o)

$(deps_arch/x86/boot/pmjump.o):
