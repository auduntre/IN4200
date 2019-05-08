#!/usr/bin/sh

cd parallel_code 

input_image=../images/mona_lisa_noisy.jpg
output_image=../images/mona_lisa_denoised.jpg

time mpirun -n 12 ./parallel_main 0.2 100 $input_image $output_image

cd ..
