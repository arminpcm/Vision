# Use Ubuntu 22.04 as the base image
FROM ubuntu:latest

# Set environment variables
ENV DEBIAN_FRONTEND=noninteractive

# Update package lists and install necessary packages
RUN apt-get update && apt-get install -y \
    python3-minimal \
    python3-pip \
    python3-setuptools \
    build-essential \
    curl \
    unzip \
    clang \
    clang-tidy \
    && rm -rf /var/lib/apt/lists/*

# Install Bazel
# Install Bazel's prerequisites
RUN apt-get update && apt-get install -y openjdk-11-jdk wget git && rm -rf /var/lib/apt/lists/*

# Install Bazel
RUN wget https://github.com/bazelbuild/bazel/releases/download/4.2.1/bazel_4.2.1-linux-x86_64.deb
RUN dpkg -i bazel_4.2.1-linux-x86_64.deb \
    && rm bazel_4.2.1-linux-x86_64.deb

RUN apt-get update \
 && apt-get install -y sudo

# Create a user named "docker" without a password
RUN useradd -m docker
# Allow the "docker" user to use sudo without a password
RUN echo 'docker ALL=(ALL) NOPASSWD:ALL' >> /etc/sudoers

# Install what's needed for dev
RUN apt-get install -y doxygen

USER docker
CMD ["/bin/bash"]
WORKDIR /Vision
