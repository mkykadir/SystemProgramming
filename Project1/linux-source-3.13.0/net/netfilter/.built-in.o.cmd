cmd_net/netfilter/built-in.o :=  ld -m elf_i386   -r -o net/netfilter/built-in.o net/netfilter/netfilter.o net/netfilter/nfnetlink.o net/netfilter/nfnetlink_log.o net/netfilter/nf_conntrack.o net/netfilter/nf_conntrack_netlink.o net/netfilter/nf_conntrack_ftp.o net/netfilter/nf_conntrack_irc.o net/netfilter/nf_conntrack_sip.o net/netfilter/x_tables.o net/netfilter/xt_tcpudp.o net/netfilter/xt_CONNSECMARK.o net/netfilter/xt_NFLOG.o net/netfilter/xt_SECMARK.o net/netfilter/xt_TCPMSS.o net/netfilter/xt_conntrack.o net/netfilter/xt_policy.o net/netfilter/xt_state.o 
