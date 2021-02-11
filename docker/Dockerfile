# Image used to build the source code

FROM ubuntu:latest AS arduino-builder

ENV DEBIAN_FRONTEND noninteractive
RUN apt-get update \
    && apt-get install -y \
    curl \
    build-essential \
    cmake \
    && rm -rf /var/lib/apt/lists/*

# Install arduino build tools
ENV BINDIR=/usr/local/bin
RUN mkdir -p /.arduino15 && chmod 777 /.arduino15 \
    && mkdir -p /Arduino && chmod 777 /Arduino \
    && mkdir -p $BINDIR \
    && curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | sh

WORKDIR /build

CMD [ "arduino-cli" "version" ]

### Image used to build the sphinx documentation

FROM sphinxdoc/sphinx:latest AS docs-builder

RUN python3 -m pip install sphinx-rtd-theme

WORKDIR /build
