cmd_firmware/bnx2/bnx2-mips-06-6.2.3.fw.gen.o := gcc -Wp,-MD,firmware/bnx2/.bnx2-mips-06-6.2.3.fw.gen.o.d  -nostdinc -isystem /usr/lib/gcc/i686-linux-gnu/4.8/include  -I/home/mkytr/Desktop/Project1/linux-source-3.13.0/arch/x86/include -Iarch/x86/include/generated  -Iinclude -I/home/mkytr/Desktop/Project1/linux-source-3.13.0/arch/x86/include/uapi -Iarch/x86/include/generated/uapi -I/home/mkytr/Desktop/Project1/linux-source-3.13.0/include/uapi -Iinclude/generated/uapi -include /home/mkytr/Desktop/Project1/linux-source-3.13.0/include/linux/kconfig.h -Iubuntu/include  -D__KERNEL__  -D__ASSEMBLY__ -m32 -DCONFIG_AS_CFI=1 -DCONFIG_AS_CFI_SIGNAL_FRAME=1 -DCONFIG_AS_CFI_SECTIONS=1  -DCONFIG_AS_AVX=1 -DCONFIG_AS_AVX2=1          -c -o firmware/bnx2/bnx2-mips-06-6.2.3.fw.gen.o firmware/bnx2/bnx2-mips-06-6.2.3.fw.gen.S

source_firmware/bnx2/bnx2-mips-06-6.2.3.fw.gen.o := firmware/bnx2/bnx2-mips-06-6.2.3.fw.gen.S

deps_firmware/bnx2/bnx2-mips-06-6.2.3.fw.gen.o := \

firmware/bnx2/bnx2-mips-06-6.2.3.fw.gen.o: $(deps_firmware/bnx2/bnx2-mips-06-6.2.3.fw.gen.o)

$(deps_firmware/bnx2/bnx2-mips-06-6.2.3.fw.gen.o):
