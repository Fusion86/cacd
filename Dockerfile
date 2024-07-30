FROM ubuntu:focal as builder

RUN apt update
RUN apt -y --no-install-recommends install \
    build-essential make libmotif-dev libxaw7-dev \
    groff ghostscript psutils \
    less \
    && apt-get clean \
    && rm -rf \
    /var/lib/apt/lists/* \
    /tmp/* \
    /var/tmp/*

WORKDIR /cacd
RUN mkdir -p bin
COPY ./ ./

ENV ICDPATH=/cacd
ENV PATH=$PATH:$ICDPATH/bin
RUN make

ENV PAGER=less
