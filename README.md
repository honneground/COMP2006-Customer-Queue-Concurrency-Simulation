# Customer Queue and Teller Thread Simulation

- **Unit**: Operating Systems (`COMP2006`) — Assignment 1
- **Study Period**: 2023 Trimester 2A
- **Degree Programme**: Bachelor of Information Technology — Awarded with Distinction by Curtin University
- **Overall Unit Mark**: 80 (High Distinction)

> **README for Assignment Submission**: [README for Assignment Submission](ORIGINAL_README.md)

<table>
  <thead>
    <tr>
      <th colspan="6">Curtin University &bull; Bachelor of Information Technology (Awarded with Distinction &bull; CWA 82.19)</th>
    </tr>
  </thead>
  <tbody>
    <tr align="center" style="font-weight: bold; background-color: #f6f8fa;">
      <td>2022 T3A</td>
      <td>2023 T1A</td>
      <td>2023 T2A</td>
      <td>2023 T3A</td>
      <td>2024 T1A</td>
      <td>2024 T2A</td>
    </tr>
    <tr>
      <td valign="top">
        <abbr title="Fundamentals of Programming (92 HD)"><b>COMP1005</b></abbr><br>
        &bull; <a href="https://github.com/honneground/COMP1005-Vampire-Ecosystem-Simulation">Vampire Eco</a><br>        &bull; <a href="https://github.com/honneground/COMP1005-Vehicle-Emissions-Analyzer">Emissions</a><br>        <abbr title="Computer Systems (83 HD)"><font color="#8c959f">COMP2000</font></abbr><br>
        <abbr title="Linear Algebra and Statistics for Engineers (71 D)"><font color="#8c959f">MATH1015</font></abbr><br>
        <abbr title="Fundamental Concepts of Data Security (73 D)"><font color="#8c959f">ISEC2001</font></abbr>
      </td>
      <td valign="top">
        <abbr title="Data Structures and Algorithms (89 HD)"><b>COMP1002</b></abbr><br>
        &bull; <a href="https://github.com/honneground/COMP1002-StudyHelp-Academic-Network">StudyHelp</a><br>        &bull; <a href="https://github.com/honneground/COMP1002-Advanced-Data-Structures-and-Visualizer">Visualizer</a><br>        <abbr title="Unix and C Programming (90 HD)"><b>COMP1000</b></abbr><br>
        &bull; <a href="https://github.com/honneground/COMP1000-Terminal-Maze-Survival-Game">Maze Survival</a><br>        &bull; <a href="https://github.com/honneground/COMP1000-Terminal-Maze-Darkness-Game">Maze Darkness</a><br>        &bull; <a href="https://github.com/honneground/COMP1000-Bookstore-Transaction-Simulator">Bookstore</a><br>        <abbr title="Introduction to Software Engineering (95 HD)"><b>ISAD1000</b></abbr><br>
        &bull; <a href="https://github.com/honneground/ISAD1000-Software-Testing-and-Pocket-Beast-Game">Pocket Beast</a><br>        <abbr title="Integrating Indigenous Science and STEM (73 D)"><font color="#8c959f">NPSC1003</font></abbr>
      </td>
      <td valign="top" style="background-color: #f1f8ff; border: 2px solid #0969da;">
        <abbr title="Unix Systems Programming (88 HD)"><b>COMP2002</b></abbr><br>
        &bull; <a href="https://github.com/honneground/COMP2002-Parallel-Directory-File-Calculator">Dir Calculator</a><br>        <abbr title="Operating Systems (80 HD)"><b>COMP2006</b></abbr><br>
        &bull; <b>&#128073; Queue Sim</b><br>        <abbr title="Computer Communications (75 D)"><font color="#8c959f">CNCO2000</font></abbr><br>
        <abbr title="Mobile Cloud Computing (79 D)"><font color="#8c959f">CNCO3003</font></abbr>
      </td>
      <td valign="top">
        <abbr title="Database Systems (80 HD)"><b>ISYS2014</b></abbr><br>
        &bull; <a href="https://github.com/honneground/ISYS2014-Relational-Database-Management-System">FootyStats DB</a><br>        <abbr title="Quantitative Techniques for Business (87 HD)"><b>ECOM2001</b></abbr><br>
        &bull; <a href="https://github.com/honneground/ECOM2001-Econometric-Statistical-Analysis-R">Econometric R</a><br>        <abbr title="Managing Social Media Platforms (69 C)"><font color="#8c959f">MKTG2006</font></abbr><br>
        <abbr title="Computing Topics (61 C)"><font color="#8c959f">COMP2005</font></abbr>
      </td>
      <td valign="top">
        <abbr title="Financial Decision Making (93 HD)"><b>ACCT1002</b></abbr><br>
        &bull; <a href="https://github.com/honneground/ACCT1002-Financial-Decision-Calculators">Finance Tools</a><br>        <abbr title="Human Computer Interface (71 D)"><b>ICTE3002</b></abbr><br>
        &bull; <a href="https://github.com/honneground/ICTE3002-Lumen-Interaction-Design">Lumen UI</a><br>        <abbr title="Engineering Management and Professional Practice (79 D)"><b>MGMT3000</b></abbr><br>
        &bull; <a href="https://github.com/honneground/MGMT3000-Phone-Router-Repurposing-Design">Phone Router</a><br>        <abbr title="Distributed Networks (83 HD)"><font color="#8c959f">CNCO3000</font></abbr>
      </td>
      <td valign="top">
        <abbr title="Capstone Computing Project 1 (94 HD)"><b>ISAD3000</b></abbr><br>
        &bull; <a href="https://github.com/honneground/ISAD3000-CurtinSG-App-Design">CurtinSG UI</a><br>        <abbr title="Capstone Computing Project 2 (95 HD)"><b>ISAD3001</b></abbr><br>
        &bull; <a href="https://github.com/honneground/ISAD3001-CurtinSG-Student-App">CurtinSG App</a><br>        <abbr title="Advanced Computer Communications (88 HD)"><b>CNCO3002</b></abbr><br>
        &bull; <a href="https://github.com/honneground/CNCO3002-Multi-Service-Socket-Server">Socket Server</a><br>        <abbr title="Accounting - The Fundamentals of Business (93 HD)"><font color="#8c959f">ACCT2013</font></abbr>
      </td>
    </tr>
  </tbody>
</table>


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

---

## Portfolio note

This Git repository was created retrospectively from the original project code, with assistance from OpenAI Codex and Antigravity in curating the repository and writing this README for publication. The initial commit timestamp is backdated to the original coursework submission date, followed by the final release commit.

