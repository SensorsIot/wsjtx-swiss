FROM mcr.microsoft.com/devcontainers/base:debian

# Rename vscode user to dev (base image has vscode with UID 1000)
RUN usermod -l dev -d /home/dev -m vscode \
    && groupmod -n dev vscode \
    && sed -i 's/vscode/dev/g' /etc/sudoers.d/vscode || true \
    && mv /etc/sudoers.d/vscode /etc/sudoers.d/dev 2>/dev/null || true

# Install build tools and dependencies (matching GitHub Actions workflow)
RUN apt-get update && apt-get install -y --no-install-recommends \
    # Build tools
    build-essential \
    cmake \
    pkg-config \
    gfortran \
    # Qt5 development
    qtbase5-dev \
    qtmultimedia5-dev \
    libqt5serialport5-dev \
    qttools5-dev \
    qttools5-dev-tools \
    # Libraries (from workflow)
    libfftw3-dev \
    libboost-all-dev \
    libportaudio2 \
    portaudio19-dev \
    libusb-1.0-0-dev \
    # Hamlib (radio control)
    libhamlib-dev \
    libhamlib-utils \
    # Additional tools
    git \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

USER dev
