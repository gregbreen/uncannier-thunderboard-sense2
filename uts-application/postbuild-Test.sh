#!/bin/sh

###-----------------------------------------------------------------------------
###
### Post-build step to execute unit tests and produce a coverage report
###
### Copyright (c) Uncannier Software 2018
###
###-----------------------------------------------------------------------------

# Set the directories where the coverage report will go
OUTDIR=Test
COVDIR=$OUTDIR/Coverage

# Clean any past runs
find . -name '*.gcda' -delete
rm -rf $COVDIR

# Somewhere to put the coverage report
mkdir -p $COVDIR

# Run the executable to run the tests, and produce the test coverage data
$OUTDIR/${1}

echo Generating test coverage report in $COVDIR

# Capture the coverage data
lcov -c --quiet --directory $OUTDIR -o $COVDIR/${1}.info

# Remove header files data - we don't want to know about coverage of inline functions by Silabs
lcov --quiet --remove $COVDIR/${1}.info "*.h" -o $COVDIR/${1}.info

# Now generate the HTML report from the coverage data
genhtml --quiet -o $COVDIR $COVDIR/${1}.info

