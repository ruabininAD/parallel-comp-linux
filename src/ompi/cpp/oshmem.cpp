#include <iostream>
#include "shmem.h"

#if !defined(OSHMEM_SPEC_VERSION) || OSHMEM_SPEC_VERSION < 10200
#error This application uses API 1.2 and up
#endif

int main(int argc, char* argv[])
{
    int proc, nproc;
    char name[SHMEM_MAX_NAME_LEN];
    int major, minor;

    shmem_init();
    nproc = shmem_n_pes();
    proc = shmem_my_pe();
    shmem_info_get_name(name);
    shmem_info_get_version(&major, &minor);

    std::cout << "Hello, world, I am " << proc << " of " << nproc << ": " << name
              << " (version: " << major << "." << minor << ")" << std::endl;

    shmem_finalize();

    return 0;
}