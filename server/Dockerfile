FROM debian:latest

ENV DEBIAN_FRONTEND=noninteractive

ENV APT_KEY_DONT_WARN_ON_DANGEROUS_USAGE=DontWarn

RUN apt-get update -y --no-install-recommends

RUN apt-get install \
	make \
	g++ \
	valgrind \
	dialog \
	-y --no-install-recommends

ENV DEBIAN_FRONTEND=dialog