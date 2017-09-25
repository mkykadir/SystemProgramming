cmd_arch/x86/lib/misc.o := gcc -Wp,-MD,arch/x86/lib/.misc.o.d  -nostdinc -isystem /usr/lib/gcc/i686-linux-gnu/4.8/include  -I/home/mkytr/Desktop/Project1/linux-source-3.13.0/arch/x86/include -Iarch/x86/include/generated  -Iinclude -I/home/mkytr/Desktop/Project1/linux-source-3.13.0/arch/x86/include/uapi -Iarch/x86/include/generated/uapi -I/home/mkytr/Desktop/Project1/linux-source-3.13.0/include/uapi -Iinclude/generated/uapi -include /home/mkytr/Desktop/Project1/linux-source-3.13.0/include/linux/kconfig.h -Iubuntu/include  -D__KERNEL__ -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -Werror-implicit-function-declaration -Wno-format-security -fno-delete-null-pointer-checks -std=gnu89 -O2 -m32 -msoft-float -mregparm=3 -freg-struct-return -mno-mmx -mno-sse -fno-pic -mpreferred-stack-boundary=2 -march=i686 -mtune=generic -Wa,-mtune=generic32 -ffreestanding -DCONFIG_AS_CFI=1 -DCONFIG_AS_CFI_SIGNAL_FRAME=1 -DCONFIG_AS_CFI_SECTIONS=1 -DCONFIG_AS_AVX=1 -DCONFIG_AS_AVX2=1 -pipe -Wno-sign-compare -fno-asynchronous-unwind-tables -mno-sse -mno-mmx -mno-sse2 -mno-3dnow -mno-avx -Wframe-larger-than=2048 -fno-stack-protector -Wno-unused-but-set-variable -fno-omit-frame-pointer -fno-optimize-sibling-calls -fno-var-tracking-assignments -Wdeclaration-after-statement -Wno-pointer-sign -fno-strict-overflow -fconserve-stack -Werror=implicit-int -Werror=strict-prototypes -DCC_HAVE_ASM_GOTO    -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(misc)"  -D"KBUILD_MODNAME=KBUILD_STR(misc)" -c -o arch/x86/lib/misc.o arch/x86/lib/misc.c

source_arch/x86/lib/misc.o := arch/x86/lib/misc.c

deps_arch/x86/lib/misc.o := \

arch/x86/lib/misc.o: $(deps_arch/x86/lib/misc.o)

$(deps_arch/x86/lib/misc.o):
