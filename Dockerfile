FROM ubuntu:18.04

ENV DEBIAN_FRONTEND=noninteractive
RUN set -x -e; \
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
        valgrind

ADD entrypoint.py /usr/local/bin/entrypoint
ENTRYPOINT ["/usr/local/bin/entrypoint"]
