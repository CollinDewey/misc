FROM alpine:latest
RUN apk --no-cache add dnsmasq bash
EXPOSE 53 53/udp
EXPOSE 67 67/udp
COPY ./startup.sh /startup.sh
ENTRYPOINT ["/bin/bash", "startup.sh"]
