#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "huffmanCoding.h"

#define COUNT 10

huffmanNode* head;


// The encoding scheme is a huffman coder
int byte_compress(int data_ptr[], int data_size) {
    
    // Creates two matching arrays to sort the byte array by frequency at which each byte appears
    // The frequency of a byte is the number of occurences of said byte
    int freqArray[128] = {0};
    int byteArray[128] = {0};
    for (int byte = 0; byte < 128; byte++) {
        byteArray[byte] = byte;
    }
    for (int data_index = 0; data_index < data_size; data_index++) {
        freqArray[data_ptr[data_index]]++;
    }
    bubbleSort(freqArray, byteArray, 128);

    // Takes bytes which have been used and matches them to a node which will be placed in a Huffman tree
    int len = 0;
    huffmanNode* treeQueue[128];
    for (int i = 0; i < 128; i++) {
        if (freqArray[i] != 0) {
            treeQueue[len] = newNode(byteArray[i], freqArray[i]);
            len++;
        }
    }

    // Saves the number of unique bytes
    int unique = len;

    // Takes the two nodes at the lowest frequency and creates a new node of which they are the children
    // The resulting node as a frequency of the sum of the children
    // Then it is then added back into the array in the correct spot to stay ordered correctly
    while (len > 1) {
        huffmanNode* node =  newNode(0, treeQueue[1]->freq + treeQueue[0]->freq);
        node->leftNode = treeQueue[0];
        node->rightNode = treeQueue[1];

        memmove(&treeQueue[0], &treeQueue[2], (len - 2) * sizeof treeQueue[0]);
        len = len - 2;
        int index = len;
        for (int i = 0; i < len; i++) {
            if (node->freq <= treeQueue[i]->freq) {
                index = i;
                break;
            }
        }
        memmove(&treeQueue[index + 1], &treeQueue[index], (len - index) * sizeof treeQueue[0]);
        (treeQueue[index]) = node;
        len++;

    }
    head = treeQueue[0];
    /* A view of the resulting tree from the example data
                                            64

                                0

                                        56

                    0

                                        9

                                0

                                        4

            0

                                0

                    0

                                                            45

                                                    0

                                                            3

                                        0

                                                    74

                                0

                                        35
    */

    // Encoding the tree into the keys of which each byte maps to
    // Left in the tree maps to 0, right maps to 1
    // Depths are also preserved so 001 isn't reduced to just 1 
    int byteKeys[128] = {0};
    int depth[128] = {0};
    recursiveKeys(head, byteKeys, depth, 0, 0);

    // Array created to house the compressed data
    int newLen = newsize(data_ptr, data_size, depth);
    int* compressedArray = (int*) malloc(newLen*sizeof(int));
    int compressedIndex = 0;
    int compressedBit = 0;

    // Encoding the data bit by bit into the new array 
    // It converts a bit into its unique key to traverse the tree, then feeds that key in, for each byte
    for (int index = 0; index < data_size; index++) {
        int sizeCompressed = bitLeng(byteKeys[data_ptr[index]]);

        for (int leadingzeroes = 0; leadingzeroes < depth[data_ptr[index]] - sizeCompressed; leadingzeroes++) {
            if (compressedBit > 7) {
                compressedBit -= 8;
                compressedIndex++;
            }  
            
            compressedBit++;
        }

        for (int keyBitIndex = sizeCompressed - 1; keyBitIndex >= 0; keyBitIndex--) {
            if (compressedBit > 7) {
                compressedBit -= 8;
                compressedIndex++;
            } 
            compressedArray[compressedIndex] |= (((byteKeys[data_ptr[index]]) & (1 << keyBitIndex)) >> keyBitIndex) << compressedBit;
            
            compressedBit++;
        }
    }

    // Replacing initial array with compressed array

    for (int i = 0; i < newLen; i++) {
        data_ptr[i] = compressedArray[i];
    }
    return newLen;
}


// This is O(n^2), but we will only every need to sory 128 items
// the increase in performance from quicksort or merge sort should be negligible

void bubbleSort(int freqArray[], int byteArray[], int data_size) { 
    for (int reps = 0; reps < data_size - 1; reps++) {
        for (int index = reps; index < data_size - 1; index++) {
            if (freqArray[index + 1] < freqArray[index]) {
                int freqTemp = freqArray[index + 1];
                freqArray[index + 1] = freqArray[index];
                freqArray[index] = freqTemp;

                int byteTemp = byteArray[index + 1];
                byteArray[index + 1] = byteArray[index];
                byteArray[index] = byteTemp;
            }

        }
    }
}

// Allocates space for new tree node

huffmanNode* newNode(int byte, int freq) {
    huffmanNode* node = (huffmanNode*) malloc(sizeof(huffmanNode));
    node->byte = byte;
    node->freq = freq;
    node->leftNode = NULL;
    node->rightNode = NULL;
    return node;
}

// Recursively generates keys for each node in the tree

void recursiveKeys(huffmanNode* head, int* bitsArray, int* depth, int depthCur, int path) {
    if (head->leftNode == NULL && head->rightNode == NULL) {
        bitsArray[head->byte] = path;
        depth[head->byte] = depthCur;
    }
    else {
        recursiveKeys(head->leftNode, bitsArray, depth, depthCur + 1, (path << 1));
        recursiveKeys(head->rightNode, bitsArray, depth, depthCur + 1, (path << 1) + 1);
    }
}

// Calculates binary length of number ....00110101 would return 6

int bitLeng(int x) {
    int count = 0;
    while (x) {
        x >>= 1;
        count++;
    }
    return count;
}

// Calculates size needed to allocate new array

int newsize(int data_ptr[], int data_size, int depth[]) {
    int bitsize = 0;
    for (int index = 0; index < data_size; index++) {
        bitsize += depth[data_ptr[index]];
    }
    return ceil(bitsize / 8.0);
}