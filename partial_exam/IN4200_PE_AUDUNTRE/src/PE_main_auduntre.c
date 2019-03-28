#include <stdio.h>
#include <stdlib.h>

#include "PE_functions_auduntre.h"


int main (int argc, char **argv)
{
    CRS crs_test = read_graph_from_file("web_graphs/8-webpages.txt");
    printf("1: %f\n2: %d\n3: %d\n", crs_test.val[0], crs_test.row_ptr[0], crs_test.col_idx[0]);
    
    free(crs_test.val);
    free(crs_test.col_idx);
    free(crs_test.row_ptr);
    return 0;
}
