# named-objects
Header-only named object base class.
This can be useful to manage user-defined data structures (based on runtime input)

-------------------

Compilation of tests with:
> mkdir build \
> cd build \
> cmake .. \
> make

Execution with:
> ./run_tests

A somewhat faulty (not taking into account unused inline functions) HTML coverage report with 'gcovr':
> make run_tests_coverage

---------------------

Acknowledgements: \
To [this](https://github.com/selyunin/gtest_submodule) repository for use of googletest as submodule.
