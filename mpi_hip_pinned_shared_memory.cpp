#include <mpi.h>
#include <hip/hip_runtime.h>
#include <iostream>
#include <chrono>

#define HIPAssert(expr) _HIPAssert((expr), __FILE__, __LINE__)

void _HIPAssert(hipError_t err, const char *file, int line) {
  if (err != hipSuccess) {
    fprintf(stderr, "HIP Assert: %s %s %d\n", hipGetErrorString(err), file, line);
    exit(EXIT_FAILURE);
  }
}

int main (int argc, char *argv[]) {
  MPI_Init(&argc, &argv);

  if(argc != 2) {
    printf("Usage: %s <GB to allocate>\n", argv[0]);
    exit(1);
  }
  int gb_to_allocate = atoi(argv[1]);

  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);


  void* data;
  MPI_Win win;
  MPI_Win_allocate_shared(rank==0 ? 1e9*gb_to_allocate : 0, 1, MPI_INFO_NULL, MPI_COMM_WORLD, &data, &win);

  MPI_Aint win_size;
  int disp_unit;
  MPI_Win_shared_query(win, 0, &win_size, &disp_unit, &data);

  HIPAssert(hipHostRegister(data, win_size, hipHostRegisterPortable));

  void* dev_ptr;
  HIPAssert(hipMalloc(&dev_ptr, win_size));
  std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
  HIPAssert(hipMemcpyAsync(dev_ptr, data, win_size, hipMemcpyHostToDevice));
  std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
  std::cout << "hipMemcpyAsync took " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms on rank " << rank << std::endl;
  
  return 0;
}
