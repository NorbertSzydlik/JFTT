Compilation
=================
1) To compile program library `cln` is needed and compiler has to have access to it.

2) Compile program using `make` command

Usage
=====
  compiler [input program] [output program]

Tests
=====
Full program tests are located under `tests/test_WholePrograms.cpp`. Input is defined in `std::istringstream stdIn;` and expected output is defined in call to `Fixture().checkOutputStr`.


