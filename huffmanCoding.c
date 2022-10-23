#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "huffmanCoding.h"

#define COUNT 10

void byte_compress(int data_ptr[], int data_size) {
    int freqArray[128] = {0};
    int byteArray[128] = {0};

    for (int byte = 0; byte < 128; byte++) {
        byteArray[byte] = byte;
    }

    for (int data_index = 0; data_index < data_size; data_index++) {
        freqArray[data_ptr[data_index]]++;
    }

    bubbleSort(freqArray, byteArray, 128);

    /*for (int i = 0; i < 128; i++) {
        if (freqArray[i] != 0) {
            printf("%i ", byteArray[i]);
        }
    }
    printf("\n");
    for (int i = 0; i < 128; i++) {
        if (freqArray[i] != 0) {
            printf("%i ", freqArray[i]);
        }
    }
    printf("\n");*/

    int len = 0;
    huffmanNode* treeQueue[128];

    for (int i = 0; i < 128; i++) {
        if (freqArray[i] != 0) {
            treeQueue[len] = newNode(byteArray[i], freqArray[i]);
            len++;
        }
    }

    int unique = len;

    while (len > 1) {
        for (int i = 0; i < len; i++) {
            //printf("%i ", treeQueue[i]->freq);
        }
        //printf("\n");
        huffmanNode* node =  newNode(0, treeQueue[1]->freq + treeQueue[0]->freq);
        node->leftNode = treeQueue[0];
        node->rightNode = treeQueue[1];

        memmove(&treeQueue[0], &treeQueue[2], (len - 2) * sizeof treeQueue[0]);
        len = len - 2;
        /*for (int i = 0; i < len; i++) {
            printf("%i ", treeQueue[i]->freq);
        }
        printf("\n");
        for (int i = 0; i < len; i++) {
            printf("%i ", treeQueue[i]->byte);
        }
        printf("\n");*/
        int index = len;
        for (int i = 0; i < len; i++) {
            if (node->freq <= treeQueue[i]->freq) {
                index = i;
                break;
            }
        }
        //printf("Index: %i\n", index);
        memmove(&treeQueue[index + 1], &treeQueue[index], (len - index) * sizeof treeQueue[0]);
        (treeQueue[index]) = node;
        len++;

    }
    print2D(treeQueue[0]);
    //printf("%i\n",treeQueue[0]->rightNode->rightNode->rightNode->byte);

    int* byteKeys = (int*) malloc(128*sizeof(int));
    int modified[128] = {0};
    recursiveKeys(treeQueue[0], byteKeys, modified, 0, 0);
    for (int i = 0; i < 128; i++) {
        if (modified[i] == 1) {
            printf("%x, %i\n", byteKeys[i], i);
        }
    }
    
    
}


// Yes it is O(n^2), Assuming we will only be sorting Bytes to size 7f, 
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

huffmanNode* newNode(int byte, int freq) {
    huffmanNode* node = (huffmanNode*) malloc(sizeof(huffmanNode));
    node->byte = byte;
    node->freq = freq;
    node->leftNode = NULL;
    node->rightNode = NULL;
    return node;
}

void recursiveKeys(huffmanNode* head, int* bitsArray, int* modified, int depth, int path) {
    if (head->leftNode == NULL && head->rightNode == NULL) {
        bitsArray[head->byte] = path;
        modified[head->byte] = 1;
        printf("%i \n", head->byte);
    }
    else {
        //printf("%x %x ", head->leftNode->byte, head->rightNode->byte);
        recursiveKeys(head->leftNode, bitsArray, modified, depth + 1, (path << 1));
        recursiveKeys(head->rightNode, bitsArray, modified, depth + 1, (path << 1) + 1);
    }
}


void print2DUtil(huffmanNode* root, int space)
{
    // Base case
    if (root == NULL)
        return;
 
    // Increase distance between levels
    space += COUNT;
 
    // Process right child first
    print2DUtil(root->rightNode, space);
 
    // Print current node after space
    // count
    printf("\n");
    for (int i = COUNT; i < space; i++)
        printf(" ");
    printf("%d\n", root->byte);
 
    // Process left child
    print2DUtil(root->leftNode, space);
}
 
// Wrapper over print2DUtil()
void print2D(huffmanNode* root)
{
    // Pass initial space count as 0
    print2DUtil(root, 0);
}