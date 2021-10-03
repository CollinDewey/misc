sudo arp-scan --macfile arp-scan.txt --localnet --interface eth0 --ignoredups --plain | sort -t . -k 3,3n -k 4,4n
