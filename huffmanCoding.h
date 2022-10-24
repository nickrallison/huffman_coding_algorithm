int byte_compress(int data_ptr[], int data_size);

void bubbleSort(int freqArray[], int byteArray[], int data_size);

typedef struct huffmanNode huffmanNode;

struct huffmanNode {
    int byte;
    int freq;

    //int internode;

    huffmanNode* leftNode;
    huffmanNode* rightNode;
    
    };

huffmanNode* newNode(int byte, int freq);

void recursiveKeys(huffmanNode* head, int* bitsArray, int* modified, int depth, int path);

int bitLeng(int x);

int newsize(int data_ptr[], int data_size, int depth[]);