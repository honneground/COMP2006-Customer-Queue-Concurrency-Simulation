## Customer Queue Simulation with Teller Threads ##

# Student Name: YE Hongran
# Student ID: [REDACTED]

## Synopsis
Assignment-01 of OS

## Overview
This program simulates a scenario where customers arrive periodically and wait in a queue to be served by 3 tellers. 
Threads were implemented to simulate the activities of customers arriving and tellers simultaneously serving in a bank-like scenario. 
The goal is to handle the queue of customers efficiently and log the details, such as the arrival time, response time, and completion time for each customer. 

## Syntax
Usage: 
    $ C Main Program
        - To compile:
        make
        - To run:
        ./cq <length_of_customer_queue> <periodic_time_for_customer_arrival>
        Arguments:
        + length_of_customer_queue: The size of the customer queue.
        + periodic_time_for_customer_arrival: The time interval (in seconds) at which customers arrive at the queue.
    $ Bash testharness
        To run: 
        bash testharness.sh [n] [mode]
        + Both parameters, `[n]` and `[mode]`, are optional.
        Parameters:
        1. [n]:
        - `help`: Shows the usage of the test harness.
        - `clean`: Reverts the files in the directory to their default state.
        - <number_of_customer>: Specify a custom number of customers for the simulation. Must be an integer.
        2. [mode]:
        - `q` or `quick`: Launches a single test only (Quick Mode).
        * Notice this test harness is run with valgrind to detect potential memory leaks.

## Contents
- main.c: The primary source code for the customer-teller simulation program.
- makefile: A utility file with commands to compile the simulation program.
- testharness.sh: A script that automates the testing of the customer queue simulation program.
- sample_infile_generator.py: Python script used to generate sample input files (`c_file`) for testing the main program.
- r_log: Log output file of the program containing detailed records of simulation events.
- c_file: Input file holding customer details, including customer numbers and their respective service times.
- report.pdf: A detailed document discussing the design, implementation, and results of the customer-teller simulation project.
- /sample_io: A directory containing sample input and output files to demonstrate and test the functionality of the customer-teller simulation program.
 + This directory contains sample input and output files from the simulation programs named sample_c_file and r_log_m where m is the length of the queue parameter in testing.

## Memory Leak Detecting
valgrind --leak-check=full ./program

## Version information
11/09/2023  - final version of Assignment 01 maze game programs

