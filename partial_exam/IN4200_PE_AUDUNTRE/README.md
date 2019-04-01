# Partial Exam IN4200/IN3200

## Computing PageRank

* For compilaton and how to run, see below.
* For the report about programming structure and findings and measurementers,
see: `report/report.pdf`.
* Header files with function documentations in `src/` folder.
* Source code (`.c` files) in `src/` folder.
* Web graph files in `web_graphs` folder.

To make a zip file of the project run the shell script `collect_submisison.sh`.

### Compiling

run `make PE_15301` to compile.

##### Verbose output from `PE_15301`:

To compile the program with verbose output add the command line argument `DFLAGS=-DVERBOSE_$(FUNC)` where `$(FUNC)` is the function where verbose output is wanted.

* `-DVERBOSE_MAIN` for the `main` function.
* `-DVERBOSE_ITERATIONS` for the `PageRank_iterations` function.

Remember to run `make clean` first if recompiling.


### Running

The program is run by `.\PE_15301 [MAXITER] [DAMPING] [THRESHOLD] [PATH/FILENAME]` where the arguments are given in order.
The program defaults to `.\PE_15301 1000 0.85 1.0E-16 web_graphs/8-webpages.txt` for the arguments not given. 
