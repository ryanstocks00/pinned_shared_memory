#!/bin/bash

module load gcc/12.2.0
module load rocm/5.4.3
module load cmake/3.24.3
module load cce/15.0.1

mkdir -b build
pushd build
cmake ..
make
popd
