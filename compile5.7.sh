#!/bin/bash

function load_unsupported_rocm()
{
        if [ -z $1 ]; then
                echo "Must pass rocm version"
                return
        fi
        local version=${1}
        local major_version=${version%.*}
        module use /software/setonix/unsupported
        module load rocm/${version}
        export PE_PKGCONFIG_LIBS=$(echo $PE_PKGCONFIG_LIBS | sed "s/rocm-${major_version}//g")
}

module load gcc/12.2.0
load_unsupported_rocm 5.7.1
module load cmake/3.24.3
module load cce/15.0.1

mkdir -p build
pushd build
cmake ..
make
popd
