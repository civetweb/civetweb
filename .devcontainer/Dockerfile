# syntax=docker/dockerfile:1
ARG ubuntu_version=24.04

FROM ubuntu:${ubuntu_version}
RUN apt-get update && \
    apt-get install -y \
                build-essential \
                ca-certificates \
                cmake \
                git \
                nano\
                openssh-server