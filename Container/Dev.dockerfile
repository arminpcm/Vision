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
RUN wget https://github.com/bazelbuild/bazel/releases/download/6.0.0/bazel_6.0.0-linux-x86_64.deb
RUN dpkg -i bazel_6.0.0-linux-x86_64.deb \
    && rm bazel_6.0.0-linux-x86_64.deb

RUN apt-get update \
 && apt-get install -y sudo

# Install web development tools
RUN apt-get update && apt-get install -y \
    git \
    libgl1-mesa-glx \
    libglib2.0-0 \
    nginx \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

# Install Python packages
RUN pip install --upgrade pip
RUN pip install Flask gunicorn

# Install specific version of Node.js and npm for frontend dependencies
RUN apt-get update && apt-get install -y ca-certificates curl gnupg
RUN curl -fsSL https://deb.nodesource.com/gpgkey/nodesource-repo.gpg.key | gpg --dearmor -o /etc/apt/keyrings/nodesource.gpg
RUN echo "deb [signed-by=/etc/apt/keyrings/nodesource.gpg] https://deb.nodesource.com/node_20.x nodistro main" | tee /etc/apt/sources.list.d/nodesource.list
RUN apt-get update && sudo apt-get install nodejs -y

# Install Three.js library for 3D visualization
WORKDIR /Vision
RUN npm install three

# Expose the port where the app will run
EXPOSE 5173

# Create a user named "docker" without a password
RUN useradd -m docker
# Allow the "docker" user to use sudo without a password
RUN echo 'docker ALL=(ALL) NOPASSWD:ALL' >> /etc/sudoers

# Install what's needed for dev
RUN apt-get update && apt-get install -y doxygen

USER docker
CMD ["/bin/bash"]
WORKDIR /Vision
