# ghaction-cmake-quality

This is a github action for projects that use cmake. It builds, tests and optionally installs the project. It can also be used to to run the tests with valgrind and clang sanitizers. Since it also generated compiled commands it can be used as pre-step for other processing (clang-tidy etc).

# Inputs
Inputs needed for action are very limited, with decent defaults. Refer to [action.yml](action.yml) for documentations.

# Examples
[Workflow for verifying this repo master](.github/workflows/main.yml) includes example of building, testing and installing this repo (test-action-demo) and GoogleTest (test-action-googletest).


```
test-action:
    name: "Test action with google test"
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@master
      - uses: rainlabs-eu/ghaction-cmake-quality@v1
        with:
          dependencies: clang-9 # not actually needed, most of common gcc/clang is included in Docker running this action
          cmake_configure_environment: "CC=clang-9 CXX=clang++-9" # Select compilers
          source_directory: . 
          build_directory: out
          cmake_configure_extra_args: "-Dgmock_build_tests=ON" # For example enable extra tests
          ctest_enable: true # Run tests with ctest
          cmake_install_enable: true
          cmake_install_directory: googletest-install
```
