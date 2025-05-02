FROM ubuntu:25.04

# Install necessary dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    ninja-build \
    cmake \
    gdb \
    cppcheck \
    valgrind \
    sudo \
    protobuf-compiler \
    protobuf-compiler-grpc \
    libgtest-dev \
    libgmock-dev \
    libcpprest-dev \
    libboost-all-dev \
    libgrpc-dev \
    libgrpc++-dev \
    libssl-dev


RUN echo 'ubuntu' | passwd --stdin ubuntu 
RUN usermod -aG sudo ubuntu

USER ubuntu