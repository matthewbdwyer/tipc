# bin
The bin directory contains a number of useful scripts for working with tipc.

## bootstrap.sh
Bootstraps a tipc build by installing all of the required dependencies. Further,
it sets a number of required environment variables in your `.bashrc`.  

This bootstrap script supports the macOS and Ubuntu platforms. It will detect the
OS for you.  

_example usage:_
```bash
./bootstrap.sh
```

## gencov.sh
Uses [LCOV][1] to generate a code coverage report.  

After a successful run, you can view an HTML version of the report report 
at `<tipc project root>/coverage.out/index.html`.  

_example usage:_
```bash
./gencov.sh
```

## gendocs.sh
Uses [doxygen][2] to generate documentation from the project source code.

After a successful run, you can view the docs
at `<tipc project root>/docs/html/index.html`.  

Additionally, if you have the Python module [coverxygen][3] installed, `gendoc.sh` will
output a document coverage report for you. The report strictly checks for documentation 
coverage of classes.  

_example usage:_
```bash
./gendocs.sh
```

## runtests.sh
Runs the full suite of tests.  

By default the script runs both the system and unit tests. This behavior
can be changed at the command line by providing the `-s` switch to
run only the system tests, or by providing the `-u` switch to run only
the unit tests.  

_example usage:_
```bash
# Run only the unit tests.
./runtests.sh -u
```


[1]: http://ltp.sourceforge.net/coverage/lcov.php
[2]: https://www.doxygen.nl/manual/commands.html
[3]: https://github.com/psycofdj/coverxygen
