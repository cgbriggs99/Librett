
#ifdef SYCL
#include <CL/sycl.hpp>
#include "dpct/dpct.hpp"
#endif
#include "CudaMem.h"

//----------------------------------------------------------------------------------------
//
// Allocate gpu memory
// pp = memory pointer
// len = length of the array
//
#ifdef SYCL
void allocate_device_T(void **pp, const size_t len, const size_t sizeofT) try {
#ifdef CUTT_HAS_UMPIRE
  *pp = cutt_umpire_allocator.allocate(sizeofT*len);
#else
  /*
  DPCT1003:0: Migrated API does not return error code. (*, 0) is inserted. You
  may need to rewrite this code.
  */
  cudaCheck((*pp = (void *)sycl::malloc_device(sizeofT * len, dpct::get_default_queue()), 0));
#endif
}
catch (sycl::exception const &exc) {
  std::cerr << exc.what() << "Exception caught at file:" << __FILE__
            << ", line:" << __LINE__ << std::endl;
  std::exit(1);
}
#else // CUDA
void allocate_device_T(void **pp, const size_t len, const size_t sizeofT) {
#ifdef CUTT_HAS_UMPIRE
  *pp = cutt_umpire_allocator.allocate(sizeofT*len);
#else
  cudaCheck(cudaMalloc(pp, sizeofT*len));
#endif
}
#endif

//----------------------------------------------------------------------------------------
//
// Deallocate gpu memory
// pp = memory pointer
//
#ifdef SYCL
void deallocate_device_T(void **pp) try {
#ifdef CUTT_HAS_UMPIRE
  cutt_umpire_allocator.deallocate((void *) (*pp) );
#else
  if (*pp != NULL) {
    /*
    DPCT1003:2: Migrated API does not return error code. (*, 0) is inserted. You
    may need to rewrite this code.
    */
    cudaCheck((sycl::free((void *)(*pp), dpct::get_default_queue()), 0));
    *pp = NULL;
  }
#endif
}
catch (sycl::exception const &exc) {
  std::cerr << exc.what() << "Exception caught at file:" << __FILE__
            << ", line:" << __LINE__ << std::endl;
  std::exit(1);
}
#else // CUDA
void deallocate_device_T(void **pp) {
#ifdef CUTT_HAS_UMPIRE
  cutt_umpire_allocator.deallocate((void *) (*pp) );
#else
  if (*pp != NULL) {
    cudaCheck(cudaFree((void *)(*pp)));
    *pp = NULL;
  }
#endif
}
#endif
