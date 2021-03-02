
#include "virtual_memory_controller.h"

#include "../framework/interface.h"
#include "../framework/logger.h"
#include "../tests/test_framework.h"
#include "linkedlist.h"
# include <stdlib.h> //for malloc
#include <math.h>// for pow()

/* TLB requirement
1. virtual page numner
2. physiacl frame number
3. dirty bit
- reference bit?
*/

/*
0. Should I create a TLB/Pagetable on my own as struct?
1. where to set page replacement policy? -or is it a part of the test case?
2. How do I determine the corresponding physical memory address?
*/
struct Node* head;
struct Node* tail;
struct pageTable* pgTable;
/********* DO NOT CHANGE FUNCTION SIGNATURE *********/
int master_initialization(void)
{
    //TODO : Put all your system initialization code here
	//initialize head
	head = (struct Node*) malloc(sizeof(struct Node));
	tail = (struct Node*) malloc(sizeof(struct Node));
	pgTable = (struct pageTable*) malloc(sizeof(struct pageTable));

	head -> size = -1;
	head -> pageNum = -1;
	head -> virtualAddress = -1;
	head -> prev = NULL;
	head -> next = NULL;
	head -> emptyFlag = -1;

	tail -> size = -1;
	tail -> pageNum = -1;
	tail -> virtualAddress = -1;
	tail -> prev = NULL;
	tail -> next = NULL;
	tail -> emptyFlag = -1;

	int pageSetUp = mem_config.virtual_memory_size / mem_config.page_size;
	if (pageSetUp > mem_config.max_virtual_pages) {
		pageSetUp = mem_config.max_virtual_pages;
	}

	for (int i = 0; i < pageSetUp; i++){
		struct Node* page = (struct Node*) malloc(sizeof(struct Node));
		page -> pageNum = i;
		page -> size = mem_config.page_size;
		page -> virtualAddress = mem_config.page_size * i;
		page -> emptyFlag = 1;
		appendNode(head, page);

	}
	appendNode(head, tail);

	pgTable = (struct pageTable*)malloc (sizeof(struct pageTable) * pageSetUp);
    for(int i = 0; i < pageSetUp; i++)
    {
        pgTable[i].frameNumber = 0;
				pgTable[i].validBit = 0;
				pgTable[i].dirtyBit = 0;
				pgTable[i].referenceBit = 0;
    }

	return SUCCESS;
}

/********* DO NOT CHANGE FUNCTION SIGNATURE *********/
int mmu_write(const void *data, int virtual_page_num, int page_offset,
              int data_size)
{
  //convert virtual address to physical address
  //write to the corresponding pysical address (frame# + offset)

    //TODO : Implement the write functionality here
    /*
    1. Memory Allocation should follow the First-Fit algorithm.
    2. Unlike Unix, where the memory allocated may be equal to or greater than the size requested (due to alignment boundaries and minimum block sizes), your implementation should allocate the exact size requested. You should not align your addresses to any specific boundary since the granularity of memory access for penn-mmu is 1 byte.
    3. Your implementation should track memory allocateds uchthat it should not allow data to be read from or written to addresses that are not allocated.
    4. Any address that lies within the range of allocated memory is a valid address for read/write operations.
    5. Only addresses that are returned by pemmu_malloc can be freed. Calling pemmu_free on any other address or on an address that has already been freed should result in an error.

    */
    return FAILURE;
}

/********* DO NOT CHANGE FUNCTION SIGNATURE *********/
int mmu_read(void *data, int virtual_page_num, int page_offset, int data_size)
{
    //TODO : Implement the read functionality here
    /*
    To-Do
    translate virtual_page_num to physical_page_num
    set reference bit to 1
    return SUCCESS (=0) upon success or Failure

    Questions
    1. int pemmu_read_data(int address, void *data, int size)
    - difference between address & *data? If we have pointer to data, why do we need address?
    */
    return FAILURE;
}

/********* DO NOT CHANGE FUNCTION SIGNATURE *********/
int mmu_allocate_variable(int type, int number)
{
    //TODO : Implement the memory allocate functionality here
    /*
    1.Should it come before write?
    */
    return addNode(head, pow(2, (type-1)) * number);

}
/*
split / merge algorithm
Questions
1. type int/char or byte size, etc. / number - size -> interface.h check DEFINE
2. allocate exact requested size? -> why not in magnitude of 2^n? / we don't use buddy system?
3. First Fit?
4. 1.1.1 free contiguous block?
*/
/********* DO NOT CHANGE FUNCTION SIGNATURE *********/
int mmu_free_variable(int address)
{
    //TODO : Implement the memory free functionality here
	return popNode(head, address);
}

/********* DO NOT CHANGE FUNCTION SIGNATURE *********/
void master_clean_up(void)
{
    //TODO : Put all your system cleanup code here
}
