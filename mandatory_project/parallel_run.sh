#!/usr/bin/sh

cd parallel_code 

input_image=../images/mona_lisa_noisy.jpg
output_image=../images/mona_lisa_denoised.jpg

time mpirun -n $1 ./parallel_main 0.2 $2 $input_image $output_image

cd ..
