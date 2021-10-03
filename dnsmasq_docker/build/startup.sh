#!/bin/bash

#Verify that the executable bit is set on this script

cat dhcp.csv | while IFS="	" read -r col1 col2 col3 col4
do
	if [[ $col1 != *"#"* ]] && [[ $col1 != "" ]]; then
		echo "$col1	$col2"
	fi
done > arp-scan.txt

cat dhcp.csv | while IFS="	" read -r col1 col2 col3 col4
do
	if [[ $col1 != *"#"* ]] && [[ $col1 != "" ]]; then
		echo "dhcp-host=${col1:0:2}:${col1:2:2}:${col1:4:2}:${col1:6:2}:${col1:8:2}:${col1:10:2},$col2,$col3,$col4"
	fi
done > /etc/dnsmasq.d/hosts.conf

echo "127.0.0.1	localhost
::1     ip6-localhost ip6-loopback
fe00::0 ip6-localnet
ff00::0 ip6-mcastprefix
ff02::1 ip6-allnodes
ff02::2 ip6-allrouters
" > /etc/hosts

cat dhcp.csv | while IFS="	" read -r col1 col2 col3 col4
do
	if [[ $col1 != *"#"* ]] && [[ $col1 != "" ]]; then
		echo "$col3	$col2"
	fi
done >> /etc/hosts

dnsmasq -k --dhcp-broadcast
