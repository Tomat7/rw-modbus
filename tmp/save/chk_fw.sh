#!/bin/bash
Myservices=("ufw" "iptables" "firewalld" "nftables" "ipfire")

for myservice in ${Myservices[@]}
do

    if systemctl is-active --quiet $myservice;
    then
	echo -e "Service is  running \033[0;32m $myservice \033[0m"
    else
	echo -e "Service not running \033[0;31m $myservice \033[0m"
    fi

done
