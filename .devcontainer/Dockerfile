# syntax=docker/dockerfile:1
ARG ubuntu_version=24.04

FROM ubuntu:${ubuntu_version}
RUN apt-get update && \
    apt-get install --no-install-recommends -y \
                build-essential \
                ca-certificates \
                cmake \
                clang \
                git \
                nano\
                openssh-server