# Simple docker container for running dnsmasq and assigning static IPs

Modify dnsmasq.conf to your network's needs. Modify dhcp.tsv (csv with tab delimiter) using the example format.

Read more about this project on my [website](https://collindewey.net/articles/dhcp-with-dnsmasq-docker-and-arp-scan/)

convertToOPNsense.sh will make an xml config import for OPNsense. Adjust the hardcoded IP ranges as fit. 
