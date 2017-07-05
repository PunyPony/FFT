#!/bin/sh

image=lena_color.ppm
make && ./fft $image && feh --force-aliasing --auto-zoom output.ppm fftoutput.ppm $image
