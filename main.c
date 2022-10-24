#include "huffmanCoding.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    int data_size = 24;
    int data_ptr[24] = { 0x03, 0x74, 0x04, 0x04, 0x04, 0x35, 0x35, 0x64, 
                         0x64, 0x64, 0x64, 0x00, 0x00, 0x00, 0x00, 0x00,
                         0x56, 0x45, 0x56, 0x56, 0x56, 0x09, 0x09, 0x09 };

    
    //printf("a\n");

    int newsize = byte_compress(data_ptr, data_size);
    printf("The new size of the array is %i resulting in a %.3f%% compression\nThe compressed array contains elements\n", newsize, 100 * (1 - (double)newsize / data_size));
    for (int i = 0; i < newsize; i++)
        printf("%x\n", data_ptr[i]);

    
}
