FROM ubuntu:latest

RUN apt-get update \
    && apt-get install -y curl \
    && rm -rf /var/lib/apt/lists/*

# Install arduino build tools
ENV BINDIR=/usr/local/bin
RUN mkdir -p $BINDIR \
    && curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | sh

WORKDIR /build

CMD [ "arduino-cli" "version" ]
