FROM ubuntu:18.04

ENV DEBIAN_FRONTEND=noninteractive
RUN set -x -e; \
    apt-get -y update; \
    apt-get -y install --no-install-recommends apt-transport-https ca-certificates gnupg software-properties-common wget; \
    rm -rf /var/lib/apt/lists/*

RUN set -x -e; \
    wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null | apt-key add -; \
    apt-add-repository -y -n 'https://apt.kitware.com/ubuntu/'; \
    apt-add-repository -y -n 'ppa:ubuntu-toolchain-r/test'; \
    wget -qO - https://apt.llvm.org/llvm-snapshot.gpg.key | apt-key add -; \
    apt-add-repository -y -n "deb http://apt.llvm.org/$(lsb_release -cs)/ llvm-toolchain-$(lsb_release -cs)-10 main"; \
    apt-add-repository -y -n 'ppa:mhier/libboost-latest'; \
    apt-get -y update; \
    apt-get -y install --no-install-recommends \
        # build
        cmake pkg-config make ninja-build \
        # GCC compilers
        gcc-5 gcc-6 gcc-7 gcc-8 gcc-9 gcc-10 \
        g++-5 g++-6 g++-7 g++-8 g++-9 g++-10 \
        # Clang compilers
        clang-4.0 clang-5.0 clang-6.0 clang-7 clang-8 clang-9 clang-10 \
        # Clang tools
        clang-tidy-4.0 clang-tidy-5.0 clang-tidy-6.0 clang-tidy-7 clang-tidy-8 clang-tidy-9 clang-tidy-10 \
        clang-format-4.0 clang-format-5.0 clang-format-6.0 clang-format-7 clang-format-8 clang-format-9 clang-format-10 \
        # LLVM
        llvm-4.0 llvm-5.0 llvm-6.0 llvm-7 llvm-8 llvm-9 llvm-10 \
        # Coverage report upload
        curl \
        # ctest -D ExperimentalMemCheck
        valgrind \
        # Using boost as reference for tests
        libboost1.73-dev \
        # zlib needed for some boost components
        zlib1g-dev \
        # git for listing files in changes
        git \
        ; \
    rm -rf /var/lib/apt/lists/*

COPY entrypoint.py /usr/local/bin/entrypoint
ENTRYPOINT ["/usr/bin/python3", "-u", "/usr/local/bin/entrypoint"]
