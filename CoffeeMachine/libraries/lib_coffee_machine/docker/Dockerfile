# Image used to build and test the source code

FROM ubuntu:latest AS builder

ENV DEBIAN_FRONTEND noninteractive
RUN apt-get update \
    && apt-get install -y \
    curl \
    build-essential \
    cmake \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /build
