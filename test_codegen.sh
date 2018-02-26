#!/bin/bash

#make compiler -B

if [[ "$1" != "" ]] ; then
    compiler="$1"
else
    compiler="bin/c_compiler"
fi

have_compiler=1
if [[ ! -f bin/c_compiler ]] ; then
    >&2 echo "Warning : cannot find compiler at path ${compiler}. Only checking C reference against python reference."
    have_compiler=0
fi

input_dir="testbench/codegen"

working="tmp/codegen_test"
mkdir -p ${working}
rm ${working}/*

for j in ${input_dir}/* ; do

    test_group=$j;
    if [ -z "$(ls -A ${test_group})" ]; then
        echo "NO TESTS FOR [$(basename $test_group)]"
    else
        echo "TESTING [$(basename $test_group)] :"
        for i in ${test_group}/*.driver.c ; do

            base=$(echo $i | sed -E -e "s|${test_group}/([^.]+)[.]driver[.]c|\1|g");

         	# Compile the driver to .o
            mips-linux-gnu-gcc -S $i -o $working/$base.driver.s

            if [[ ${have_compiler} -eq 1 ]] ; then
                # Compile to .c to .s
                $compiler --compile ${test_group}/$base.c -o ${working}/$base-slave.s
                
                # Link driver and slave
                mips-linux-gnu-gcc --static ${working}/$base-slave.s $working/$base.driver.s -o $working/$base-test
                
                # Run driver
                qemu-mips $working/$base-test
                TEST_OUT=$?    
            fi

            if [[ $TEST_OUT -ne 0 ]] ; then
                echo "$base, Fail, $TEST_OUT"
            else
                echo "$base, Pass"
            fi
        done
    fi

done