### Compiling

To compile the serial and the parallel program simple tpye `make -jN`,
where N is the number of threads wanted to utilize.


### Running

The simplest way of running the programs is to use the `serial_run.sh`,
and the `parallel_run.sh` shell scripts. For the serial one the first
argument given is the number of iterations, while for the
parallel script the first argument is the number of proccess to use and
the second one is the number of iterations. Kappa is by default set 
to 0.2, so to change this you need to change the scripts. 
Example with kappa = 0.2 and 100 iterations:

 - For serial run: `bash serial_run.sh 100` 
 - For parallel run with 4 processes: `bash parallel_run.sh 4 100`

Alternativly the path to the binaries are `parallel_code/parllel_main` and 
`serial_code/serial_main`. The programs follows the project description
for how to give arguments. See each programs `.c` file for more detail.


### Documentation

Function documentation in header files in each folder. The rest is 
explained by comments.
