#!/bin/bash

#make compiler -B

if [[ "$1" != "" ]] ; then
    compiler="$1"
else
    compiler="bin/c_compiler"
fi

have_compiler=1
if [[ ! -f bin/c_compiler ]] ; then
    >&2 echo "Warning : cannot find compiler at path ${compiler}."
    have_compiler=0
fi

input_dir="testbench/codegen"

working="tmp/codegen_test"
mkdir -p ${working}
rm ${working}/*

count=0;

for i in ${input_dir}/*.c ; do
    count=$((count+1));
    base=$(echo $i | sed -E -e "s|${input_dir}/([^.]+)[.]c|\1|g");

 	# Compile the .c to .o to get reference
    mips-linux-gnu-gcc  --static -march=mips1 -mfp32 $i -o $working/$base-ref

    if [[ ${have_compiler} -eq 1 ]] ; then
        # Compile to .c to .s using Compiler
        $compiler -S $i -o ${working}/$base-out.s
        
        # Statically link .s
        mips-linux-gnu-gcc -std=c89 -march=mips1 -mfp32 --static -O0 ${working}/$base-out.s -o $working/$base-out
                
        # Run reference
        qemu-mips $working/$base-ref
        REF_OUT=$? 

        # Run reference
        qemu-mips $working/$base-out
        GOT_OUT=$? 
    fi

    if [[ $REF_OUT -ne $GOT_OUT ]] ; then
        Status="Fail"
        printf  "%-5s %-27s \e[31m %-10s \e[0m REF:%-7s GOT:%-7s \n" $count $base $Status $REF_OUT $GOT_OUT
    else
        Status="Pass"
        printf  "%-5s %-27s \e[32m %-10s \e[0m \n" $count $base $Status
    fi
done