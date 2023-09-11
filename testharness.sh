#!/bin/bash

m_values=(1 5 12 25 50) # Array of lengths of queue values\
t=2 # periodic time

m=8 # default queue length in the quick mode
n=100 # default number of customers 

if [ "$1" == "help" ]; then
    echo 
    echo "Usage: bash testharness.sh [n] [mode]"
    echo "- where both parameters are optional"
    echo  
    echo "* By setting [n] to:"
    echo "  -\"help\" for the usage of the testharness"
    echo "  -\"clean\" for revising the files in the directory back to default"
    echo "  -<number_of_customer> for changing the default number of customers."
    echo
    echo "* By setting [mode] to:"
    echo "  -\"q\" or \"quick\" for laughing a single test only (quick mode)."
    echo 
    echo "* Motiflying the testing lengths of queue and the periodic time is also possible by editing the initial variable in the head of tesharness file."
    exit 0

elif [ "$1" == "clean" ]; then
    for m in "${m_values[@]}"; do
        rm -f "r_log_$m"
    done
    rm -f "r_log"
    make clean
    python3 sample_infile_generator.py $n
    echo
    echo "Cache files deleted."
    exit 0

# Change the number of customers 
elif [[ $1 =~ ^[0-9]+$ ]]; then
    n=$1
fi

# Generating the sample testing c_file
python3 sample_infile_generator.py $n
# Compile the c program
make

# Quick mode
if [ -n "$2" ]; then
    valgrind --leak-check=full ./cq $m $t
    echo 
    echo "Quick mode finished execution."
    exit 0
fi

# Regular mode
for m in "${m_values[@]}"; do
    valgrind --leak-check=full ./cq $m $t
    mv r_log "r_log_$m"
done
echo
echo "The test harness finished execution."