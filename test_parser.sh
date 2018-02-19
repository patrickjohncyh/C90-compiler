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

input_dir="testbench/c_parser"

working="tmp/c_parser_test"
mkdir -p ${working}

for i in ${input_dir}/*.c ; do
    base=$(echo $i | sed -E -e "s|${input_dir}/([^.]+)[.]c|\1|g");

 	# Compile the reference C version
    gcc $i -o $working/$base

    # Run the reference C version
    $working/$base
    REF_C_OUT=$?

    if [[ ${have_compiler} -eq 1 ]] ; then
	    # Parse and output C89
	    $compiler --parse $i -o ${working}/$base-got.c
	    
	    # Run parsed C
	    gcc ${working}/$base-got.c -o $working/$base-got
	    $working/$base-got
	    GOT_C_OUT=$?    
	fi

	if [[ $REF_C_OUT -ne $GOT_C_OUT ]] ; then
		echo "$base, GOT_FAIL, Expected ${REF_C_OUT}, got ${GOT_C_OUT}"
 	else
        echo "$base, Pass"
    fi

done