#
# build:
# docker build --build-arg CACHEBUST=$(date +%F) --build-arg USER=$USER -t cc:22.04 .
#
# run:
# docker run -ti -v /home:/home -w $PWD cc:22.04
#
FROM ubuntu:22.04

ENV DEBIAN_FRONTEND noninteractive

ARG CACHEBUST
RUN apt-get update

RUN apt-get install -y bash bash-completion
RUN apt-get install -y sudo vim gawk curl wget

# build tools
RUN apt-get install -y make quilt git

# compilers
RUN apt-get install -y gcc clang
RUN apt-get install -y gcc-aarch64-linux-gnu
RUN apt-get install -y gcc-arm-linux-gnueabihf
RUN apt-get install -y gcc-riscv64-linux-gnu

# misc tools
RUN apt-get install -y inotify-tools strace ripgrep
RUN apt-get install -y mc tree htop

RUN echo "dash dash/sh boolean false" | debconf-set-selections
RUN DEBIAN_FRONTEND=noninteractive dpkg-reconfigure dash

ARG USER=user

RUN groupadd -g 1000 ${USER}
RUN useradd -g ${USER} -G sudo -u 1000 ${USER}
RUN echo "${USER} ALL=(ALL) NOPASSWD: ALL" > /etc/sudoers.d/user

WORKDIR /home/${USER}
USER ${USER}
ENV USER ${USER}

CMD /bin/bash
