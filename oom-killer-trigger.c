/**
* oom-killer-trigger: Allocate and touch memory of given size to trigger an
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

#define MB                  (1024 * 1024)
#define CHUNK_MB_SZ         128   /* 128 MB */
#define WAIT_S              30    /* 30 seconds */
#define PAGE_SZ             4096  /* 4 KB */

/**
* Touch the firt byte of each page. It should be faster than initializing the
* whole allocated chunk.
*/
void
touch_pages(char *ptr, const uint64_t size)
{
    const uint64_t bytes = size * MB;

    for (int i = 0; i <= bytes; i += PAGE_SZ)
        ptr[i] = 1;
}

/**
* Allocate 'size' MB of memory.
*/
void *
allocate(const uint64_t size)
{
    if (size == 0)
        return NULL;

    void *ptr = malloc(size * MB);

    if (ptr == NULL)
        printf("Unable to allocate chunk of %luMB\n", size);

    return ptr;
}

int
main (int argc, char *argv[])
{
    uint64_t i;
    if (argc != 2)
    {
        printf("Allocate and map X MB.\nUsage %s <alloc_MB>\n", argv[0]);
        exit(0);
    }

    const uint64_t size_mbytes = atoi(argv[1]);
    printf("Allocating %lu MB...\n", size_mbytes);

    printf("Checking overcommit max size...\n");

    const uint64_t chunks = size_mbytes / CHUNK_MB_SZ;

    void **array_ptr = calloc(chunks, sizeof(void *));
    if (array_ptr == NULL)
    {
        printf("Unable to allocate the array of pointers\n");
        exit(1);
    }


    printf("Trying allocating %lu MB...\n", chunks * CHUNK_MB_SZ);
    printf("Checking overcommit max size...\n");

    for (i = 0; i < chunks; i++)
    {
        array_ptr[i] = allocate(CHUNK_MB_SZ);
        if (array_ptr[i] == NULL)
            break;
    }

    if (array_ptr[chunks - 1] != NULL)
        printf("Total: allocated %lu MB\n", chunks * CHUNK_MB_SZ);
    else
        printf("Total: allocated %lu MB\n", i * CHUNK_MB_SZ);


    printf("Wainting %ds...\n", WAIT_S);
    sleep(WAIT_S);


    printf("Touching pages... (it may generate an OOM)\n");

    for (i = 0; i < chunks; i++)
    {
        if (array_ptr[i] == NULL)
            break;

        touch_pages((char*)array_ptr[i], CHUNK_MB_SZ);
        printf("Touched %lu MB...\n", i + 1 * CHUNK_MB_SZ);
    }

    printf("Waiting again %ds...\n", WAIT_S);
    sleep(WAIT_S);

    return 0;
}
