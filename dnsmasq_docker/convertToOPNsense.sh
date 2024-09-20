#!/usr/bin/env bash

cat dhcp.csv | while IFS="	" read -r col1 col2 col3 col4
do
	if [[ $col1 != *"#"* ]] && [[ $col1 != "" ]]; then
		echo "$col1	$col2"
	fi
done > arp-scan.txt



echo '<?xml version="1.0"?>
<opnsense>
		<dhcpd>
				<lan>
						<enable>1</enable>
						<staticarp>1</staticarp>
						<denyunknown>1</denyunknown>
						<ddnsdomainalgorithm>hmac-md5</ddnsdomainalgorithm>
						<numberoptions>
							<item/>
						</numberoptions>
						<range>
							<from>172.16.0.2</from>
							<to>172.16.255.254</to>
						</range>
						<winsserver/>
						<dnsserver/>
						<ntpserver/>' > insert.xml

cat dhcp.csv | while IFS="	" read -r col1 col2 col3 col4
do
    if [[ $col1 != *"#"* ]] && [[ $col1 != "" ]]; then
        mac=$(echo "${col1}" | tr '[:lower:]' '[:upper:]')
        ipaddr="$col3"
        hostname="$col2"
        descr="$col4"

        echo -e "\t\t\t\t\t\t<staticmap>"
        echo -e "\t\t\t\t\t\t\t<mac>${mac:0:2}:${mac:2:2}:${mac:4:2}:${mac:6:2}:${mac:8:2}:${mac:10:2}</mac>"
        echo -e "\t\t\t\t\t\t\t<ipaddr>${ipaddr}</ipaddr>"
        echo -e "\t\t\t\t\t\t\t<hostname>${hostname}</hostname>"
        echo -e "\t\t\t\t\t\t\t<descr>${descr}</descr>"
        echo -e "\t\t\t\t\t\t\t<arp_table_static_entry>1</arp_table_static_entry>"
        echo -e "\t\t\t\t\t\t\t<winsserver/>"
        echo -e "\t\t\t\t\t\t\t<dnsserver/>"
        echo -e "\t\t\t\t\t\t\t<ntpserver/>"
        echo -e "\t\t\t\t\t\t</staticmap>"
    fi
done >> insert.xml

echo '				</lan>
		</dhcpd>
</opnsense>' >> insert.xml
