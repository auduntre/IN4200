#include <stdio.h>
#include <stdlib.h>


int main (int argc, char **argv)
{
    int line_counter (char *filename);

    char *filename;
    char *fileline = NULL;

    if (argc > 1) {
        filename = argv[1];
    }
    else {
        printf("Filename must be given. \n");
        return 1;
    }

    int line_count = line_counter(filename);
    size_t line_length;

    FILE *infile = fopen(filename, "r");
    double *temps = (double *) malloc (line_count * sizeof(double));
    int *hour = (int *) malloc (line_count * sizeof(int));
    int *minute = (int *) malloc (line_count * sizeof(int));

    double avg_temp = 0.0;
    double max_temp, min_temp;
    double std_dev = 0.0;
    int max_idx = 0;
    int min_idx = 0;


    if (infile == NULL) {
        printf("Not able to open file. \n");
        return 1;
    }
        
    getline(&fileline, &line_length, infile);
    sscanf(fileline, "%d:%d  %lf", &hour[0], &minute[0],  &temps[0]);
    
    max_temp = temps[0];
    min_temp = temps[0];
    
    for (int i = 1; i < line_count; i++) {
        getline(&fileline, &line_length, infile);
        sscanf(fileline, "%d:%d  %lf", &hour[i], &minute[i],  &temps[i]);

        if (max_temp < temps[i]) {
            max_temp = temps[i];
            max_idx = i;
        } 
        else if (min_temp > temps[i]) {
            min_temp = temps[i];
            min_idx = i;
        }

        avg_temp += temps[i];
    }
    avg_temp /= line_count;

    for (int i = 1; i < line_count; i++) {
        std_dev += (temps[i] - avg_temp) * (temps[i] - avg_temp);
    }
    std_dev /= line_count - 1;

    printf("Average Temperature was %f with standard deviation of %f. \n",
           avg_temp, std_dev);
    printf("Highest Teperature was %f at %d:%d. \n", max_temp, hour[max_idx],
           minute[max_idx]);
    printf("Lowest Teperature was %f at %d:%d. \n", min_temp, hour[min_idx],
           minute[min_idx]);

    fclose(infile); 
    
    free(hour);
    free(minute);
    free(temps);
    return 0;
}


int line_counter (char *filename)
{
    int line_count = 0;
    size_t line_length = 0;
    
    char *line = NULL;
    FILE *infile = fopen(filename, "r");
    
    if (infile == NULL) {
        printf("Not able to open file. \n");
        exit(1);
    }
   
    // Count lines untill end of file 
    while (getline(&line, &line_length, infile) != -1) {
        line_count++;
    }

    printf("LINE COUNT = %d \n", line_count);
    fclose(infile);
    
    return line_count;
}
