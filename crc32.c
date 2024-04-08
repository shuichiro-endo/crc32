/*
 * Title:  crc32.c
 * Author: Shuichiro Endo
 */

#include <stdio.h>
#include <string.h>
#include <stdint.h>


static void usage(char *filename)
{
    printf("usage   : %s function_name\n", filename);
    printf("example : %s LoadLibraryA\n", filename);

    return;
}


static uint32_t crc32(const uint8_t *input, size_t size)
{
    const uint32_t poly = 0xEDB88320;
    uint32_t crc = 0xFFFFFFFF;

    for(size_t i=0; i<size; i++){
        crc ^= input[i];
        for(size_t j=0; j<8; j++){
            crc = (crc >> 1) ^ (crc & 1) * poly;
        }
    }

    return ~crc;
}


int main(int argc, char **argv)
{
    if(argc != 2){
        usage(argv[0]);
        return 0;
    }

    uint8_t *input = (uint8_t *)argv[1];
    uint32_t crc32_hash = 0x0;

    crc32_hash = crc32((const uint8_t *)input, strlen(input));

    printf("%-60s : %08X  %08x\n", input, crc32_hash, crc32_hash);

    return 0;
}

