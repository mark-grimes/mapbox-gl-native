FROM ubuntu:disco-20190718

ENV DEBIAN_FRONTEND="noninteractive"
ENV ANDROID_HOME="/opt/android"

WORKDIR /src

RUN set -eu && apt-get update

# Base dependencies
RUN set -eu && apt-get install -y \
    cmake \
    g++ \
    git \
    mesa-common-dev \
    ninja-build \
    nodejs \
    npm \
    pkg-config \
    xvfb

# Cache Mapbox GL Native
RUN set -eu \
    && git clone https://github.com/mapbox/mapbox-gl-native.git -b tmpsantos-build_system \
    && cd mapbox-gl-native \
    && git submodule update --init --recursive \
    && git gc

# Cache NodeJS dependencies
RUN set -eu \
    && cd mapbox-gl-native \
    && npm install

# Linux dependencies
RUN set -eu && apt-get install -y \
    libcurl4-openssl-dev \
    libgl1-mesa-dev \
    libglfw3-dev \
    libicu-dev \
    libjpeg-turbo8-dev \
    libpng-dev \
    libuv1-dev \
    zlib1g-dev

# Qt dependencies
RUN set -eu && apt-get install -y \
    qt5-default

# Android dependencies
RUN set -eu && apt-get install -y \
    coreutils \
    curl \
    openjdk-8-jdk-headless \
    unzip

# Install Android SDK
RUN set -eu \
    && mkdir -p ${ANDROID_HOME} && cd ${ANDROID_HOME} \
    && curl -L --retry 3 https://dl.google.com/android/repository/sdk-tools-linux-4333796.zip -o tools.zip \
    && (echo "92ffee5a1d98d856634e8b71132e8a95d96c83a63fde1099be3d86df3106def9  tools.zip" | sha256sum -c) \
    && unzip -q tools.zip && rm tools.zip

RUN set -eu \
    && mkdir -p ${ANDROID_HOME} && cd ${ANDROID_HOME} \
    && curl -L --retry 3 https://dl.google.com/android/repository/android-ndk-r19-linux-x86_64.zip -o ndk.zip \
    && (echo "f02ad84cb5b6e1ff3eea9e6168037c823408c8ac  ndk.zip" | sha1sum -c) \
    && unzip -q ndk.zip && rm ndk.zip && mv android-ndk-r* ndk-bundle

RUN set -eu \
    && mkdir -p ${ANDROID_HOME} && cd ${ANDROID_HOME} \
    && yes | tools/bin/sdkmanager \
        "platform-tools" \
        "platforms;android-26" \
        "build-tools;26.0.3" \
        "platforms;android-27" \
        "build-tools;27.0.3" \
        "platforms;android-28" \
        "build-tools;28.0.3" \
        "extras;android;m2repository" \
        "patcher;v4" \
        "extras;google;m2repository" \
        "extras;m2repository;com;android;support;constraint;constraint-layout;1.0.2" \
        "cmake;3.10.2.4988404"

RUN set -eu && apt-get clean
