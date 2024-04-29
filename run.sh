#!/bin/bash
cmake -S . -B build
cd build && make
../bin/RaytracerWeekend > ../bin/image.ppm
mupdf ../bin/image.ppm
