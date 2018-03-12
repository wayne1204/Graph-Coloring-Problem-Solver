# Graph-Coloring-Problem-Solver

A solver using SAT solver to solve graph coloring problem

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes. See deployment for notes on how to deploy the project on a live system.

### Prerequisites

If your computer doesn't have "zlib.h", install the package by typing
```
sudo apt-get install libz-dev
```
## How to use the solver ?

### Step0. Build the solver
just simply run the script and 
```
./Run.all
```
### Step1. Output CNF formats:

output CNF files for specified graph-coloring problems, Usage:
```
   ./Colortest <#nodes> <#colors> <conductivity>[-Cnf]
```
Examples:
```
	./Colortest 50 7 -Cnf
	./Colortest 65 8 -c
```
### Step2. minisat:

solving SAT problems by using CNF files, Usage:
```
   ./minisat <CNF files> <result file>
```
Examples:
```
   ./minisat 50_7_CNF result.log
   ./minisat 65_8_CNF result.log
```
### Step3. decoder:

translate the message from minisat to Graph-coloring problem, Usage:
```
   ./decoder <CNF files> <result file>
```
Exapmles:
```
   ./decoder 50_7_CNF result.log
   ./decoder 65_8_CNF result.log
```

