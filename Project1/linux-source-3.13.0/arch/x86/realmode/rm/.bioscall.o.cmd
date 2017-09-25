cmd_arch/x86/realmode/rm/bioscall.o := gcc -Wp,-MD,arch/x86/realmode/rm/.bioscall.o.d  -nostdinc -isystem /usr/lib/gcc/i686-linux-gnu/4.8/include  -I/home/mkytr/Desktop/Project1/linux-source-3.13.0/arch/x86/include -Iarch/x86/include/generated  -Iinclude -I/home/mkytr/Desktop/Project1/linux-source-3.13.0/arch/x86/include/uapi -Iarch/x86/include/generated/uapi -I/home/mkytr/Desktop/Project1/linux-source-3.13.0/include/uapi -Iinclude/generated/uapi -include /home/mkytr/Desktop/Project1/linux-source-3.13.0/include/linux/kconfig.h -Iubuntu/include  -D__KERNEL__   -I/home/mkytr/Desktop/Project1/linux-source-3.13.0/arch/x86/include -Iarch/x86/include/generated  -Iinclude -I/home/mkytr/Desktop/Project1/linux-source-3.13.0/arch/x86/include/uapi -Iarch/x86/include/generated/uapi -I/home/mkytr/Desktop/Project1/linux-source-3.13.0/include/uapi -Iinclude/generated/uapi -include /home/mkytr/Desktop/Project1/linux-source-3.13.0/include/linux/kconfig.h -Iubuntu/include  -m32 -g -Os -D_SETUP -D__KERNEL__ -D_WAKEUP -I/home/mkytr/Desktop/Project1/linux-source-3.13.0/arch/x86/boot -DDISABLE_BRANCH_PROFILING -Wall -Wstrict-prototypes -march=i386 -mregparm=3 -include /home/mkytr/Desktop/Project1/linux-source-3.13.0/arch/x86/realmode/rm/../../boot/code16gcc.h -fno-strict-aliasing -fomit-frame-pointer -fno-pic -mno-mmx -mno-sse  -ffreestanding  -fno-toplevel-reorder  -fno-stack-protector  -mpreferred-stack-boundary=2 -D__ASSEMBLY__          -c -o arch/x86/realmode/rm/bioscall.o arch/x86/realmode/rm/bioscall.S

source_arch/x86/realmode/rm/bioscall.o := arch/x86/realmode/rm/bioscall.S

deps_arch/x86/realmode/rm/bioscall.o := \
  /home/mkytr/Desktop/Project1/linux-source-3.13.0/arch/x86/realmode/rm/../../boot/code16gcc.h \
  arch/x86/realmode/rm/../../boot/bioscall.S \

arch/x86/realmode/rm/bioscall.o: $(deps_arch/x86/realmode/rm/bioscall.o)

$(deps_arch/x86/realmode/rm/bioscall.o):
