cmd_arch/x86/lib/msr-reg.o := gcc -Wp,-MD,arch/x86/lib/.msr-reg.o.d  -nostdinc -isystem /usr/lib/gcc/i686-linux-gnu/4.8/include  -I/home/mkytr/Desktop/Project1/linux-source-3.13.0/arch/x86/include -Iarch/x86/include/generated  -Iinclude -I/home/mkytr/Desktop/Project1/linux-source-3.13.0/arch/x86/include/uapi -Iarch/x86/include/generated/uapi -I/home/mkytr/Desktop/Project1/linux-source-3.13.0/include/uapi -Iinclude/generated/uapi -include /home/mkytr/Desktop/Project1/linux-source-3.13.0/include/linux/kconfig.h -Iubuntu/include  -D__KERNEL__  -D__ASSEMBLY__ -m32 -DCONFIG_AS_CFI=1 -DCONFIG_AS_CFI_SIGNAL_FRAME=1 -DCONFIG_AS_CFI_SECTIONS=1  -DCONFIG_AS_AVX=1 -DCONFIG_AS_AVX2=1          -c -o arch/x86/lib/msr-reg.o arch/x86/lib/msr-reg.S

source_arch/x86/lib/msr-reg.o := arch/x86/lib/msr-reg.S

deps_arch/x86/lib/msr-reg.o := \
    $(wildcard include/config/x86/64.h) \
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
    $(wildcard include/config/x86/alignment/16.h) \
  include/linux/errno.h \
  include/uapi/linux/errno.h \
  /home/mkytr/Desktop/Project1/linux-source-3.13.0/arch/x86/include/uapi/asm/errno.h \
  /home/mkytr/Desktop/Project1/linux-source-3.13.0/include/uapi/asm-generic/errno.h \
  /home/mkytr/Desktop/Project1/linux-source-3.13.0/include/uapi/asm-generic/errno-base.h \
  /home/mkytr/Desktop/Project1/linux-source-3.13.0/arch/x86/include/asm/dwarf2.h \
    $(wildcard include/config/as/cfi.h) \
    $(wildcard include/config/as/cfi/signal/frame.h) \
    $(wildcard include/config/as/cfi/sections.h) \
  /home/mkytr/Desktop/Project1/linux-source-3.13.0/arch/x86/include/asm/asm.h \
  /home/mkytr/Desktop/Project1/linux-source-3.13.0/arch/x86/include/asm/msr.h \
    $(wildcard include/config/paravirt.h) \
    $(wildcard include/config/smp.h) \
  /home/mkytr/Desktop/Project1/linux-source-3.13.0/arch/x86/include/uapi/asm/msr.h \
  /home/mkytr/Desktop/Project1/linux-source-3.13.0/arch/x86/include/uapi/asm/msr-index.h \

arch/x86/lib/msr-reg.o: $(deps_arch/x86/lib/msr-reg.o)

$(deps_arch/x86/lib/msr-reg.o):
