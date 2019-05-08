#!/usr/bin/sh

cd serial_code 

input_image=../images/mona_lisa_noisy.jpg
output_image=../images/mona_lisa_denoised.jpg

time ./serial_main 0.2 $1 $input_image $output_image

cd ..
