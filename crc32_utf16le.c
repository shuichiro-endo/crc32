/*
 * Title:  crc32_utf16le.c
 * Author: Shuichiro Endo
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <iconv.h>

#define BUFFER_SIZE 8192


static void usage(char *filename)
{
    printf("usage   : %s dll_name\n", filename);
    printf("example : %s NTDLL.DLL\n", filename);

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


static size_t get_number_of_bytes_of_utf16_string(uint8_t *input)
{
    size_t i = 0;

    while(!(input[i] == '\0' && input[i+1] == '\0')){
        i += 2;
    }

    return i;
}


static size_t convert_utf8_utf16le(const uint8_t *input, uint8_t *output, size_t output_size)
{
    iconv_t conv;
    uint8_t *buffer = calloc(BUFFER_SIZE, sizeof(uint8_t));
    uint8_t *input_buffer = buffer;
    uint8_t *output_buffer = output;
    size_t input_length = strlen(input);
    size_t output_length = (size_t)input_length*2;
    size_t size = output_size-2;
    size_t ret = 0;

    if(input_length*2 > BUFFER_SIZE){
        goto error;
    }

    memcpy(buffer, input, input_length);

    conv = iconv_open("UTF-16LE", "UTF-8");
    if(conv == (iconv_t)-1){
        goto error;
    }

    ret = iconv(conv, (char **)&input_buffer, &input_length, (char **)&output_buffer, &size);
    if(ret == -1){
        goto error;
    }

    ret = iconv_close(conv);
    if(ret == -1){
        goto error;
    }

    output_length = get_number_of_bytes_of_utf16_string(output);

    free(buffer);
    return output_length;

error:
    free(buffer);
    return -1;
}


int main(int argc, char **argv)
{
    if(argc != 2){
        usage(argv[0]);
        return 0;
    }

    uint8_t *input = (uint8_t *)argv[1];
    uint8_t *input_utf16le = calloc(BUFFER_SIZE*2+1, sizeof(uint8_t));
    uint32_t crc32_hash = 0x0;
    size_t ret = 0;

    ret = convert_utf8_utf16le((const uint8_t *)input, input_utf16le, (size_t)BUFFER_SIZE*2);
    if(ret != strlen(input)*2){
        goto error;
    }

    crc32_hash = crc32((const uint8_t *)input_utf16le, ret);

    printf("%-60s : %08x\n", input, crc32_hash);

    free(input_utf16le);
    return 0;

error:
    free(input_utf16le);
    return -1;
}

