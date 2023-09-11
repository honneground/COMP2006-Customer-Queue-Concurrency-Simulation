# Customer Queue and Teller Thread Simulation

- **Unit**: Operating Systems (`COMP2006`) — Assignment 1
- **Study Period**: 2023 Trimester 2A
- **Degree Programme**: Bachelor of Information Technology — Awarded with Distinction by Curtin University
- **Overall Unit Mark**: 80 (High Distinction)

> **README for Assignment Submission**: [README for Assignment Submission](ORIGINAL_README.md)

## Assignment context

The Operating Systems assignment asked for a C program on Linux that simulates a bank queue using POSIX threads. One customer thread reads a workload file and adds customers at a fixed interval to a bounded FIFO queue. Three teller threads remove customers and simulate service with `sleep()`.

The task required synchronization with mutexes and condition variables, blocking when the queue is full or empty, and an event log covering arrival, response, completion and teller termination. The last teller records the per-teller counts and total customers served. The report also had to explain shared resources, which threads access them, synchronization and test evidence.

## Design

`main.c` holds the queue implementation, producer/customer function and teller function. It creates one customer thread and three teller threads, then joins them during shutdown.

- `mutex_queue` coordinates queue state and `cond_full` / `cond_empty` provide waiting and notification.
- `mutex_output` serializes writes to the shared event log.
- The customer thread marks input completion and wakes waiting tellers. Tellers finish after the queue is empty and no further customers will arrive.
- `sample_infile_generator.py` generates a chosen number of customers with service times randomly selected from 1 to 10 seconds.
- `testharness.sh` generates an input, compiles and runs example queue configurations under Valgrind. Its regular queue lengths are 1, 5, 12, 25 and 50; the quick path uses length 8 and a two-second arrival interval.

Arrivals occur at the configured fixed interval. The report explains the synchronization design and shows sample runs.

## Input and output

The file `c_file` contains one customer number and service time per line, for example:

```text
1 5
2 2
3 3
```

The application writes `r_log` in the current directory. Logs include timestamps and identifiers for customer/teller events, termination records and final counts. `sample_io/` retains selected historical input/output examples.

## Build and run

Use a Linux or WSL environment with GCC, Make and POSIX threading support. Python 3 is needed for workload generation, and the shell harness additionally requires Bash and Valgrind.

From this directory:

```bash
python3 sample_infile_generator.py 20
make
./cq 10 1
```

The arguments are queue capacity and the fixed customer-arrival interval in seconds. The implementation accepts capacity of at least 1 and an interval of at least 0. Service itself still uses the times in `c_file`.

## Scripted example runs

```bash
bash testharness.sh help
bash testharness.sh 20 quick
```

The harness regenerates `c_file` and writes logs. Omitting the quick argument runs the five queue lengths, with logs saved as `r_log_1`, `r_log_5`, and so on. With real `sleep()` delays, these runs can take time.

```bash
valgrind --leak-check=full ./cq 10 1
make clean
```

Valgrind reports memory diagnostics for the run.
