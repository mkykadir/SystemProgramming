cmd_kernel/sched/built-in.o :=  ld -m elf_i386   -r -o kernel/sched/built-in.o kernel/sched/core.o kernel/sched/proc.o kernel/sched/clock.o kernel/sched/cputime.o kernel/sched/idle_task.o kernel/sched/fair.o kernel/sched/rt.o kernel/sched/stop_task.o kernel/sched/wait.o kernel/sched/completion.o kernel/sched/cpupri.o kernel/sched/stats.o kernel/sched/cpuacct.o 
