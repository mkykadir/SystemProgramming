cmd_arch/x86/lib/rwlock.o := gcc -Wp,-MD,arch/x86/lib/.rwlock.o.d  -nostdinc -isystem /usr/lib/gcc/i686-linux-gnu/4.8/include  -I/home/mkytr/Desktop/Project1/linux-source-3.13.0/arch/x86/include -Iarch/x86/include/generated  -Iinclude -I/home/mkytr/Desktop/Project1/linux-source-3.13.0/arch/x86/include/uapi -Iarch/x86/include/generated/uapi -I/home/mkytr/Desktop/Project1/linux-source-3.13.0/include/uapi -Iinclude/generated/uapi -include /home/mkytr/Desktop/Project1/linux-source-3.13.0/include/linux/kconfig.h -Iubuntu/include  -D__KERNEL__  -D__ASSEMBLY__ -m32 -DCONFIG_AS_CFI=1 -DCONFIG_AS_CFI_SIGNAL_FRAME=1 -DCONFIG_AS_CFI_SECTIONS=1  -DCONFIG_AS_AVX=1 -DCONFIG_AS_AVX2=1          -c -o arch/x86/lib/rwlock.o arch/x86/lib/rwlock.S

source_arch/x86/lib/rwlock.o := arch/x86/lib/rwlock.S

deps_arch/x86/lib/rwlock.o := \
    $(wildcard include/config/x86/32.h) \
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
    $(wildcard include/config/x86/64.h) \
    $(wildcard include/config/x86/alignment/16.h) \
  /home/mkytr/Desktop/Project1/linux-source-3.13.0/arch/x86/include/asm/alternative-asm.h \
    $(wildcard include/config/smp.h) \
  /home/mkytr/Desktop/Project1/linux-source-3.13.0/arch/x86/include/asm/asm.h \
  /home/mkytr/Desktop/Project1/linux-source-3.13.0/arch/x86/include/asm/frame.h \
    $(wildcard include/config/frame/pointer.h) \
  /home/mkytr/Desktop/Project1/linux-source-3.13.0/arch/x86/include/asm/dwarf2.h \
    $(wildcard include/config/as/cfi.h) \
    $(wildcard include/config/as/cfi/signal/frame.h) \
    $(wildcard include/config/as/cfi/sections.h) \
  /home/mkytr/Desktop/Project1/linux-source-3.13.0/arch/x86/include/asm/rwlock.h \
    $(wildcard include/config/nr/cpus.h) \

arch/x86/lib/rwlock.o: $(deps_arch/x86/lib/rwlock.o)

$(deps_arch/x86/lib/rwlock.o):
