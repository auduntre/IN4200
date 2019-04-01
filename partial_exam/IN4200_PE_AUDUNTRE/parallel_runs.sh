#!/bin/bash

make PE_15301
GRAPH_FILE=web_graphs/web-NotreDame.txt

# NB! NEEDS THE NOTRE DAME GRAPH FILE TO RUN 
# IT WILL DOWNLOAD IT AND STORE IT IN THE web_graphs FOLDER
if [ ! -f "$GRAPH_FILE" ]; then
    cd web_graphs
    wget https://snap.stanford.edu/data/web-NotreDame.txt.gz
    gunzip web-NotreDame.txt.gz
    cd ..
fi

# Send the number of threads to be used as a argument
for i in $(seq 1 $1); do
    echo "RUNNNING CODE WITH $i THREADS:"
    echo "-----------------------------"
    OMP_NUM_THREADS=$i ./PE_15301 1000 0.85 1.0E-16 1 $GRAPH_FILE
    echo
done
