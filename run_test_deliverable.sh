#!/bin/bash

if [[ -z "$1" ]]; then 
    COMPILER=bin/c_compiler
else
    COMPILER=$1
fi

working="tmp/test_deliv"
mkdir -p ${working}
rm ${working}/*

for DRIVER in test_deliverable/test_cases/*_driver.c ; do
    NAME=$(basename $DRIVER _driver.c)
    TESTCODE=test_deliverable/test_cases/$NAME.c
    
    >&2 echo "Test case $NAME"
    
    # Compile driver with normal GCC
    mips-linux-gnu-gcc -c $DRIVER -o ${working}/${NAME}_driver.o 2> ${working}/${NAME}_driver.compile.stderr
    if [[ $? -ne 0 ]]; then
        >&2 echo "ERROR : Couldn't compile driver program using GCC."
        continue
    fi

    # Compile test function with compiler under test to assembly
    $COMPILER -S $TESTCODE -o ${working}/$NAME.s  2> ${working}/${NAME}.compile.stderr
    if [[ $? -ne 0 ]]; then
        >&2 echo "ERROR : Compiler returned error message."
        continue
    fi
    
    # Link driver object and assembly into executable
    mips-linux-gnu-gcc -static ${working}/${NAME}.s ${working}/${NAME}_driver.o -o ${working}/${NAME}.elf 2> ${working}/${NAME}.link.stderr
    if [[ $? -ne 0 ]]; then
        >&2 echo "ERROR : Linker returned error message."
        continue
    fi
    
    # Run the actual executable
    qemu-mips ${working}/${NAME}.elf
    RET=$?
    if [[ $RET -ne 0 ]]; then
        >&2 echo "ERROR : Testcase returned $RET, but expected 0."
        continue
    fi

    echo "pass"
done
