cmake_minimum_required(VERSION 3.21..3.26)
cmake_policy(SET CMP0074 NEW)
cmake_policy(SET CMP0083 NEW)
cmake_policy(SET CMP0104 NEW)
if(POLICY CMP0135)
  cmake_policy(SET CMP0135 NEW)
endif()

project(PSM
  VERSION 0.0.1
  LANGUAGES C CXX)

set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Werror -std=gnu++17")
if(${GPU_RUNTIME} MATCHES "HIP")
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wno-c99-extensions")
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(pshmem ${CMAKE_CURRENT_SOURCE_DIR}/mpi_hip_pinned_shared_memory.cpp)
add_executable(pshmem_one_rank ${CMAKE_CURRENT_SOURCE_DIR}/mpi_hip_pinned_shared_memory.cpp)

find_package(HIP REQUIRED)
find_package(MPI REQUIRED)

target_link_libraries(pshmem PUBLIC
    MPI::MPI_CXX
    hip::host)
target_link_libraries(pshmem_one_rank PUBLIC
    MPI::MPI_CXX
    hip::host)

set_target_properties(pshmem PROPERTIES HIP_ARCHITECTURES "gfx90a")
target_compile_features(pshmem PUBLIC cxx_std_17)
target_compile_definitions(pshmem PUBLIC PIN_ALL_RANKS)
set_target_properties(pshmem_one_rank PROPERTIES HIP_ARCHITECTURES "gfx90a")
target_compile_features(pshmem_one_rank PUBLIC cxx_std_17)


