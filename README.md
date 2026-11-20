# Parallel Database Query Processing System

**A small SQL-style query engine written in C: it parses `SELECT … FROM … WHERE …` queries with nested boolean logic and runs them over a one-million-record database, with serial, OpenMP and MPI implementations.**

This was a five-person team project for COSC 420 (High-Performance Computing) at Salisbury University in fall 2024.

## What it does

1. **Generates a database.** `dataGen` writes a text database of **1,000,000 randomly generated cars**. Each record has an ID, model (225 options), year, color (15), price and dealer (100). OpenMP and MPI versions of the generator split the work across threads or processes.
2. **Parses a query.** For example:
   ```sql
   SELECT ID, Model, YearMake, Color
   FROM car_inventory
   WHERE Model="accord" AND YearMake="2015" AND (Color="blue" OR Color="gray");
   ```
   - The `SELECT` list chooses which fields to print. `FROM` names the database file.
   - The `WHERE` clause can use `=`, `!=`, `<`, `>`, `<=` and `>=`, combined with `AND`, `OR` and parentheses.
   - The parser tokenizes the clause and converts it from **infix to postfix** with a stack (shunting-yard style).
3. **Evaluates it.**
   - The postfix expression is evaluated with a stack of result sets.
   - Each condition becomes a `find_all` scan over the database.
   - `AND` and `OR` become **intersections** and **unions** of the matching sets.
   - Matching records are printed with the selected fields.

## Parallel implementations

| Version | Files | Approach |
|---|---|---|
| Serial | `searchUtility.c`, `dataGen.c` | Single-threaded scans and set operations (the version built by the makefile) |
| OpenMP | `searchUtilityOMP.c`, `dataGenOMP.c` | Shared-memory threads: `find_all` splits the scan across threads with guided scheduling and per-thread result buffers merged in a critical section; union and intersect are also parallelized |
| MPI | `searchUtilityMPI.c`, `dataGenMPI.c` | Distributed memory: each rank scans a slice of the database, and results are gathered on rank 0 with non-blocking `Isend`/`Irecv`. The generator builds chunks per rank and combines them with `MPI_Gatherv`. |

## Building and running

You need GCC. The OpenMP versions need `-fopenmp`, and the MPI versions need an MPI implementation such as Open MPI or MPICH.

```bash
# 1. Generate the database (writes car_inventory.txt)
gcc -o dataGen dataGen.c
./dataGen

# 2. Build the query engine
make clean && make            # produces ./SerialDataBase

# 3. Run the query in SQLCommand.txt
./SerialDataBase $(cat SQLCommand.txt)
```

Parallel generators:

```bash
gcc -fopenmp -o dataGenOMP dataGenOMP.c && ./dataGenOMP
mpicc -o dataGenMPI dataGenMPI.c && mpirun -np 4 ./dataGenMPI
```

### Query syntax rules
- `SELECT`, `FROM` and `WHERE` each start their own line.
- Field names are case-sensitive: `ID`, `Model`, `YearMake`, `Color`, `Price`, `Dealer`.
- A condition has no spaces inside it (`Color="gray"`). Quotes around values are optional.
- Parentheses may sit directly against a logical operator: `AND(Color="blue" OR Color="gray")`.
- The `WHERE` line ends with a semicolon.

[`README.txt`](README.txt) is the original course submission guide, with more detail. [`Proj2doc.pdf`](Proj2doc.pdf) is the project specification.

## Code structure

| File | Responsibility |
|---|---|
| `serialMain.c` | Entry point: ties parsing, loading, evaluation and output together |
| `parse.c/.h` | Splits the query into its clauses; converts `WHERE` from infix to postfix |
| `QueryParsing.c/.h` | Evaluates the postfix expression against the database |
| `searchUtility*.c/.h` | `find_all`, comparisons, `union_arrays` and `intersect_arrays` (serial, OpenMP, MPI) |
| `theDatabaser.c/.h` | Loads the text database into memory and frees it |
| `stringStack.c/.h`, `structStack.c/.h` | Stacks used by the parser and the evaluator |
| `dataGen*.c` | Database generators (serial, OpenMP, MPI) |

## Team

| Name | Contributions |
|---|---|
| [Kyle Tranfaglia](https://github.com/ktranfaglia1) | Database generator; search algorithms (`find_all`, union, intersect); OpenMP and MPI versions of them |
| [Timothy McKirgan](https://github.com/TimothyJMcKirgan) | Query parsing; infix-to-postfix conversion; overall code architecture |
| [Dustin O'Brien](https://github.com/Omniladder) | Main program and operation dispatch; memory management; build and linking; second MPI version |
| [Olivia Brague](https://github.com/obrague2002) | Testing and time-complexity analysis; hashmap and stack structures |
| [Nicholas Gammel](https://github.com/snowcov) | Structures and loading for the database file; print and free routines |
