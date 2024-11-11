# Use Ubuntu 22.04 as the base image
FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive
ENV TZ=Asia/Jakarta

# Install necessary packages
RUN apt-get update && \
    apt-get install -y \
    x11-apps \
    sudo \
    python3 \
    python3-pip \
    libx11-dev \
    git \ 
    libx11-dev \
    mesa-utils \
    cmake \ 
    make \ 
    gcc \ 
    g++ \ 
    libgl1-mesa-dev \
    libglu1-mesa-dev \
    freeglut3-dev \
    qtbase5-dev\
    python3-matplotlib\
    python3-numpy\
    pkg-config\
    && apt-get clean

# Set environment variable to disable interactive prompts

ENV DISPLAY=host.docker.internal:0.0
# Create a new user 'docker' with password 'docker' and add to sudo group
RUN useradd -ms /bin/bash docker && \
    echo "docker:docker" | chpasswd && \
    usermod -aG sudo docker


# Switch to 'docker' user
USER docker

# Set the home directory for the user
WORKDIR /home/docker

# Default command to keep container running
CMD ["bash"]
