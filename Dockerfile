FROM debian:buster

# install debian packages:
ENV DEBIAN_FRONTEND=noninteractive
RUN set -x -e; \
    apt-get update; \
    apt-get install -y --no-install-recommends \
        # infra
        ca-certificates python3-yaml \
        # build
        cmake pkg-config make gcc g++ \
        # coverage report
        curl lcov \
        # clang
        clang clang-tidy clang-format llvm grcov \
        # C/C++ linters \
        cppcheck iwyu \
        # used by clang-format
        git \
        # cpack
        file dpkg-dev \
        # base system (su)
        util-linux

# ctest -D ExperimentalMemCheck; may not work in all architectures
RUN apt-get install -y --no-install-recommends valgrind || true

# setup su for dep installation
RUN sed -i '/pam_rootok.so$/aauth sufficient pam_permit.so' /etc/pam.d/su

ADD entrypoint /usr/local/bin/entrypoint
CMD ["/usr/local/bin/entrypoint"]
