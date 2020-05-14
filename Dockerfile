FROM ubuntu:18.04

ENV DEBIAN_FRONTEND=noninteractive
RUN set -x -e; \
    apt-get update; \
    apt-get install -y --no-install-recommends apt-transport-https ca-certificates gnupg software-properties-common wget; \
    wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null | apt-key add -; \
    apt-add-repository 'deb https://apt.kitware.com/ubuntu/ bionic main'; \
    apt-get update; \
    apt-get install -y --no-install-recommends \
        # build
        cmake pkg-config make \
        # GCC compilers
        gcc-5 g++5 gcc-6 g++-6 gcc-7 g++-7 gcc-8 g++-8 \
        # Clang compilers
        clang-4.0 clang-5.0 clang-6.0 clang-7 clang-8 clang-9 \
        # Coverage report upload
        curl \
        # ctest -D ExperimentalMemCheck
        valgrind \
        # Using boost as reference for tests
        libboost-all-dev \
        # git for listing files in changes
        git

COPY entrypoint.py /usr/local/bin/entrypoint
ENTRYPOINT ["/usr/bin/python3", "-u", "/usr/local/bin/entrypoint"]
