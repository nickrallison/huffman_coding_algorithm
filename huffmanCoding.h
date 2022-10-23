void byte_compress(int data_ptr[], int data_size);

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

void print2DUtil(huffmanNode* root, int space);
 
// Wrapper over print2DUtil()
void print2D(huffmanNode* root);