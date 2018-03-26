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
    exit
    have_compiler=0
fi




function run_log {
    ( $1 ) > ${working}/$2.stdout.txt  2> ${working}/$2.stderr.txt
    ret=$?
    return $ret
}




input_dir="testbench/codegen_driver"
working="tmp/codegen_drive_test"
mkdir -p ${working}
rm ${working}/*
count=0;
echo "######################################"
echo "    Testing Compiler with Drivers     "
echo "######################################"
for i in ${input_dir}/*.driver.c ; do
    count=$((count+1));
    base=$(echo ${i} | sed -E -e "s|${input_dir}/([^.]+)[.]driver[.]c|\1|g");
    {
        echo "Compile Driver"
        run_log "mips-linux-gnu-gcc -march=mips1 -mfp32  -S $i -o $working/$base.driver.s" $base-driver-compile
        DRIVE_COMP=$?    
        if [[ ${DRIVE_COMP} -ne 1 ]] ; then
            echo "Compile Driver"
            run_log "mips-linux-gnu-gcc -march=mips1 -mfp32  -S ${input_dir}/$base.slave.c -o $working/$base.slave.s" $base-slave-compile
            SLAVE_COMP=$? 
            if [[ ${SLAVE_COMP} -ne 1 ]] ; then  
                rm  $working/$base.slave.s
                echo "Compile Slave with DUT"
                run_log "$compiler -S ${input_dir}/$base.slave.c -o ${working}/$base.slave.dut.s" $base-slave-dut-compile
                    
                echo "Link Driver and Slave"
                run_log "mips-linux-gnu-gcc -std=c89 -march=mips1 -mfp32 --static -O0 ${working}/$base.slave.dut.s ${working}/$base.driver.s -o ${working}/$base.out" $base-link
                    
                echo "Execute output file"
                run_log "qemu-mips $working/$base.out" $base-execute
                OUT_RET=$?
            fi
        fi
    }>${working}/$base-compile.log

    if [[ ${DRIVE_COMP} -eq 1 ]] ; then
        Status="Fail"
        printf  "%-5s %-35s \e[31m %-10s \e[0m  Error in driver source .c\n" $count $base $Status
    else
        if [[ ${SLAVE_COMP} -eq 1 ]] ; then
        Status="Fail"
        printf  "%-5s %-35s \e[31m %-10s \e[0m  Error in slave source .c\n" $count $base $Status
        else
            if [[ $OUT_RET -ne 0 ]] ; then
                Status="Fail"
                printf  "%-5s %-35s \e[31m %-10s \e[0m \n" $count $base $Status

            else
                Status="Pass"
                printf  "%-5s %-35s \e[32m %-10s \e[0m \n" $count $base $Status
            fi   
        fi
    fi
done



input_dir="testbench/codegen"
working="tmp/codegen_test"
mkdir -p ${working}
rm ${working}/*


count=0;
echo "######################################"
echo "Testing Compiler with Individual files"
echo "######################################"
for i in ${input_dir}/*.c ; do
    count=$((count+1));
    base=$(echo $i | sed -E -e "s|${input_dir}/([^.]+)[.]c|\1|g");
    {
     	echo "Compile the reference C version"
        run_log "mips-linux-gnu-gcc  --static -march=mips1 -mfp32 $i -o $working/$base-ref" $base-ref-compile
        REF_COMP=$?
        if [[ ${REF_COMP} -eq 1 ]] ; then
            echo "Unable to compile reference C Version"
        else
            echo "Run the reference C version"
            # Run reference
            run_log "qemu-mips $working/$base-ref" $base-ref-execute
            REF_OUT=$? 
            echo "REF_OUT=${REF_OUT}"
            if [[ ${have_compiler} -eq 1 ]] ; then
                echo "Compile to .c to .s using ${compiler}"
                run_log "$compiler -S $i -o ${working}/$base-out.s" $base-dut-compile
                
                echo "Statically link ${base-out}.s"
                run_log "mips-linux-gnu-gcc -std=c89 -march=mips1 -mfp32 --static -O0 ${working}/$base-out.s -o $working/$base-out" $base-dut-link

                echo "Run DUT executable"
                run_log "qemu-mips $working/$base-out" $base-dut-execute
                GOT_OUT=$? 
                echo "GOT_OUT=${GOT_OUT}"
            fi
        fi
    }>${working}/$base-compile.log

    if [[ $REF_COMP -eq 1 ]] ; then
        Status="Fail"
        printf  "%-5s %-35s \e[31m %-10s \e[0m Cannot Compile .c source \n" $count $base $Status
    else
        if [[ $REF_OUT -ne $GOT_OUT ]] ; then
            Status="Fail"
            printf  "%-5s %-35s \e[31m %-10s \e[0m REF:%-7s GOT:%-7s \n" $count $base $Status $REF_OUT $GOT_OUT
        else
            Status="Pass"
            printf  "%-5s %-35s \e[32m %-10s \e[0m REF:%-7s GOT:%-7s \n" $count $base $Status $REF_OUT $GOT_OUT
        fi
    fi
done


















