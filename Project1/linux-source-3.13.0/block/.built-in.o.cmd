cmd_block/built-in.o :=  ld -m elf_i386   -r -o block/built-in.o block/elevator.o block/blk-core.o block/blk-tag.o block/blk-sysfs.o block/blk-flush.o block/blk-settings.o block/blk-ioc.o block/blk-map.o block/blk-exec.o block/blk-merge.o block/blk-softirq.o block/blk-timeout.o block/blk-iopoll.o block/blk-lib.o block/blk-mq.o block/blk-mq-tag.o block/blk-mq-sysfs.o block/blk-mq-cpu.o block/blk-mq-cpumap.o block/ioctl.o block/genhd.o block/scsi_ioctl.o block/partition-generic.o block/partitions/built-in.o block/bsg.o block/noop-iosched.o block/deadline-iosched.o block/cfq-iosched.o 
