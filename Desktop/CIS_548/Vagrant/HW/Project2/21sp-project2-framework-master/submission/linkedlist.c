/*
0. Should I malloc for the virtual memory linkedlist itself as well?
for TLB and page table do I keep track of size in terms of # of pages?
2. Should I do circular pagetable/tlb ?
3. Should I implement eviction logic on my own as well? Or should I make another set of methods to keep track?
 -
*/

#include "linkedlist.h"
#include "../framework/logger.h"
#include "../framework/interface.h"
#include <stddef.h> //add for NULL
#include <stdlib.h> //for free
#include <stdio.h> //for perror

void appendNode(struct Node* head, struct Node* newNode){
	struct Node * currentPtr = head;
	while (currentPtr -> next != NULL){
		currentPtr = currentPtr -> next;
	}
	currentPtr -> next = newNode;
	newNode -> prev = currentPtr;
	newNode -> next = NULL;

}

int popNode (struct Node* head, int address){
	struct Node* currentPtr = head;
	while(currentPtr ->next->next != NULL){
		currentPtr = currentPtr -> next;
		if(currentPtr -> virtualAddress == address){
			if(currentPtr -> emptyFlag == 0){
				int pageCheck = currentPtr -> pageNum;
				// currentPtr -> prev != NULL && not needed in while loop as head and tail have emptyFlag value of -1
				while(currentPtr -> prev -> emptyFlag == 1 &&  pageCheck == currentPtr -> prev -> pageNum) {
					struct Node* temp;
					temp = currentPtr;
					currentPtr = currentPtr -> prev;

					temp -> prev -> size = temp -> prev-> size + temp ->size;
					temp -> next -> prev = temp -> prev;
					temp -> prev -> next = temp -> next;

					free(temp);
				}
				// && currentPtr -> next != NULL, not needed in while loop as head and tail have emptyFlag value of -1
				while(currentPtr -> next -> emptyFlag == 1  && pageCheck == currentPtr -> next -> pageNum){
					struct Node* temp;
					temp = currentPtr->next;

					temp ->  prev -> size = temp -> prev -> size + temp ->size;
					temp -> next -> prev = temp -> prev;
					temp -> prev -> next  = temp->next;

					free(temp);

				}
				return SUCCESS;
			}
			else{
				return FAILURE;
			}
		}
	}
	return FAILURE;
}

//return the virtual address for occupied space?
//How do I log success & failure?
int addNode(struct Node* head, int size) {
	printf("Requested size: %d\n", size);
	fflush(stdout);

	if (size > mem_config.page_size) {
		// perror("requested size is larger than page size");
		// fflush(stdout);
		return FAILURE;
	}

  struct Node* current = head; //initialize current at head
  while ((current -> next != NULL && current -> size < size) || current -> emptyFlag == 0) {
    current = current -> next;
  }
  if (current -> next == NULL) { //reached the tail without finding a node
    // printf("Reached the tail without finding space\n");
		// fflush(stdout);
    return FAILURE;
  } else { // found the node

    if (current -> size == size) {//if current size equals, no need to split memory
      current -> emptyFlag = 0; //set it as occupied
			// printf("allocated current address: %d\n", current -> virtualAddress);
			// fflush(stdout);
			// log_address_allocation(current -> virtualAddress);//
      //populate current
    } else {//split the memory / create extra node
      int newSize = current -> size - size; //store the size for the newly split node
      current -> size = size;//update size of current node to requested size
      current -> emptyFlag = 0;//set it as occupied

      //populate other relevant fields in current
      struct Node* newNode = (struct Node*) malloc(sizeof(struct Node));//allocate memory for the new node
      struct Node* next = current -> next; //save the pointer to the next node

      current -> next = newNode;

      newNode -> prev = current;
      newNode -> next = next;
      newNode -> size = newSize;
			newNode -> emptyFlag = 1;

			newNode -> pageNum = current -> pageNum; //maintain the same page number as the original
      newNode -> virtualAddress = current -> virtualAddress + current -> size; //update its addr with the size offset

			printf("\tcurrent address: %d\n", current -> virtualAddress);
			printf("\tallocated newNode address: %d\n", newNode -> virtualAddress);
			fflush(stdout);
			log_address_allocation(current -> virtualAddress);//
      //populate other values;
    }

    return current -> virtualAddress;
  }
}

//head == tail
//eviction
//FIFO - pop the foremost node
//circular list
// struct page {
//   int pid;//maybe not needed since it is single process?
//   int frame; //page number
//   int virtual;
//   int swap; //swap frame, set as -1 if not in swap
//   int valid; //do I need this? having been created means that it's already valid?
//   int dirty;
//   int current; //for LRU
//   struct page* next;
// }
//
// struct tlb_entry {
//   int pid;
//   int frame;
//   int virtual;
//   int swap;
//   int valid;
//   int dirty;
//   int current; //for LRU
//   struct page* next;
// }

// if (nodeNum <= max_tlb_entries) {
//   /*
//   1. What if requested size is larger than a page size? How do I map multiple pages for one size
//   2. Since frame size is fixed, I'll have internal fragmentation, but how come I allocate the exact requested size in the tlb?
//   */
// } else if (nodeNum > max_tlb_entries) { // check if I exceed max tlb entry
//   /*
//   add to the page table instead of tlb
//   eviction for tlb -> Do I always evict? When do I not evict from TLB?
//   */
// } else if (nodeNum > physical_memory_size ?? max_physical_pages??) { //size exceeding max_page_entries
//   /*
//   if larger, then write to swap instead
//   */
// } else if (nodeNum > swap_memory_size?? max_swap_memory_pages) {
//   /*
//   return error if requested memory is larger than the remaining swap memory
//   or... do I also do eviction in swap memory?
//   */
// }
