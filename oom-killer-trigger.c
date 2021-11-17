/**
* oom-killer-trigger: Allocate and map memory of given size to trigger an
*                     Out-Of-Memory.
*
* URL       https://github.com/jyvet/oom-killer-trigger
* License   MIT
* Author    Jean-Yves VET <contact[at]jean-yves.vet>
* Copyright (c) 2021
******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>

#define MB          (1024 * 1024)
#define CHUNK_MB_SZ  1024         /* 1 GB */

/**
 * Allocate and initialize (to force mapping) 'size' MB of memory.
 */
void *
allocate(const uint64_t size)
{
    if (size == 0)
        return NULL;

    void *ptr = malloc(size * MB);

    if (ptr == NULL)
    {
        printf("Allocation error. Exit...\n");
        exit(1);
    }

    memset(ptr, 1, size * MB);
    return ptr;
}


int
main (int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Allocate and initialize X MB.\nUsage %s <alloc_MB>\n", argv[0]);
        exit(0);
    }

    const uint64_t size_mbytes = atoi(argv[1]);
    printf("Allocating %lu MB...\n", size_mbytes);

    const uint64_t size_chunk = size_mbytes / CHUNK_MB_SZ;
    const uint64_t last_iter = size_mbytes % CHUNK_MB_SZ;

    for (uint64_t i = 0; i < size_chunk; i++)
    {
        printf("%lu MB... \t", i * CHUNK_MB_SZ);
        void *ptr = allocate(CHUNK_MB_SZ);
        printf(" DONE (%p)\n", ptr);
    }

    allocate(last_iter);

    sleep(10);

    return 0;
}
