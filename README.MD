# Project testrun_lib

This module is self supported and may be build, tested, installed and 
run independently. 

## Overview

* [Description](#description)
* [Usage](#usage)
* [Installation](#installation)
* [Requirements](#requirements)
* [Structure](#structure)
* [Tests](#tests)
* [Tips](#tips)
* [Copyright](#copyright)   

## Description



## Usage



## Installation

Package installation will be done by default as static library 
**/usr/local/lib/testrun_lib.a** as well as shared library
**/usr/local/lib/testrun_lib.so**. 

A package config file will be written to 
**/usr/local/lib/pkgconfig/testrun_lib.pc**

In addition to the installation all header files of the module will be provided
for external development at **/usr/local/include/testrun_lib/**

### build sources

```bash
make
```

### build documentation

```bash
make documentation
```

### test sources

```bash
make tested
```

### install binaries

```bash
sudo make install
```

### uninstall binaries

```bash
sudo make uninstall
```

## Requirements

## Structure

### Default structure of the folder:

```
<pre>
.
├── COPYRIGHT
├── README.MD
├── makefile
├── testrun_makefile.src
│
├── doxygen
│   ├── documentation
│   └── doxygen.config
│
├── docs
│   ├── CHANGELOG.MD
│   └── ...
│
├── include
│   ├── testrun_lib.h
│   └── ...
│
├── src
│   ├── testrun_lib.c
│   └── ...
│
└── tests
    ├── resources
    ├── tools
    │   ├── unit_test_runner.h
    │   ├── testrun_simple_coverage_tests.sh
    │   ├── testrun_simple_unit_tests.sh
    │   └── testrun_simple_loc.sh
    ├── acceptance
    │   ├── ...
    │   └── ...
    └── unit
        ├── .c
        └── ...

</pre>
```

## Tests

All test sources of will be recompiled on each make run. That means, 
all module tests will be created new on any change in any source file.  

### Test a project (all files contained in tests/unit)

Test compile and run 
~~~
make tested
~~~

### Test a dedicated source file (single file of tests/unit)

To develop a test for a specific source file, 
it may be helpful to use a separated test run. 
This may be done via an integrated make functionality.

Test compile example
~~~
make test testname=tests/unit/filename_tests.c 
~~~

Test compile and run in valgrind
~~~
make test testname=tests/unit/filename_tests.c && valgrind ./build/test/unit/filename_tests.test 
~~~

## Tips

## Copyright

Copyright 2017 Markus Toepfer

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License. 

This file is part of the testrun project. http://testrun.info
