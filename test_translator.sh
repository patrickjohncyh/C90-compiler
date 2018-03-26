#!/bin/bash

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

input_dir="testbench/translator"

working="tmp/translator_test"
mkdir -p ${working}

for i in ${input_dir}/*.c ; do
    base=$(echo $i | sed -E -e "s|${input_dir}/([^.]+)[.]c|\1|g");

 	# Compile the reference C version
    gcc -std=c89 $i -o $working/$base
    # Run the reference C version
    $working/$base
    REF_C_OUT=$?

    if [[ ${have_compiler} -eq 1 ]] ; then
	    # Parse and output C89
	    $compiler --translate $i -o ${working}/$base-dut.py
	    
	    # Run parsed C
	    python3 ${working}/$base-dut.py
	    GOT_PY_OUT=$?    
	fi

	if [[ $REF_C_OUT -ne $GOT_PY_OUT ]] ; then
		echo "$base, GOT_FAIL, Expected ${REF_C_OUT}, got ${GOT_PY_OUT}"
 	else
        echo "$base, Pass, Expected ${REF_C_OUT}, got ${GOT_PY_OUT}"
    fi

done