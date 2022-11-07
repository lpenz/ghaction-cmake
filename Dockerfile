ARG BASE=debian:bullseye
FROM ${BASE}

# install debian packages:
ENV DEBIAN_FRONTEND=noninteractive
RUN set -e -x; \
    echo 'Acquire::Retries "3";' > /etc/apt/apt.conf.d/80-retries; \
    apt-get update; \
    apt-get install -y --no-install-recommends \
        # infra
        ca-certificates python3-yaml \
        # build
        cmake pkg-config make gcc g++ \
        # coverage report
        curl lcov \
        # clang
        clang clang-tidy clang-format \
        # C/C++ linters \
        cppcheck iwyu \
        # used by clang-format
        git \
        # cpack
        file dpkg-dev \
        # base system (su)
        util-linux; \
    # ctest -D ExperimentalMemCheck; may not work in all architectures
    apt-get install -y --no-install-recommends valgrind || true; \
    # setup su for dep installation
    sed -i '/pam_rootok.so$/aauth sufficient pam_permit.so' /etc/pam.d/su; \
    # make git trust all directories to avoid issues in CI
    git config --system --add safe.directory '*'

ADD entrypoint /usr/local/bin/entrypoint
CMD ["/usr/local/bin/entrypoint"]
