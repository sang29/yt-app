struct Node {
	int pageNum;
	int emptyFlag;
	int size;
	int virtualAddress;
	struct Node* next;
	struct Node * prev;
};

struct pageTable {
	int frameNumber;
	int validBit;
	int dirtyBit;
	int referenceBit; //maybe
};

void appendNode(struct Node* head,struct Node* newNode);
int popNode (struct Node* head, int address);
int addNode(struct Node* head, int size);
