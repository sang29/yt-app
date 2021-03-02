#include <stdint.h>

#ifndef TEST_COMMON_SUBROUTINES_H
#define TEST_COMMON_SUBROUTINES_H

#define BYTE_TYPE_SIZE  1
#define WORD_TYPE_SIZE  2
#define DWORD_TYPE_SIZE 4
#define QWORD_TYPE_SIZE 8

#define FILL_TYPE_UPCOUNTER 1
#define FILL_TYPE_PAGE_NO   2
#define FILL_TYPE_ZERO_DATA 3

#define PHYSICAL_MEMORY 1
#define VIRTUAL_MEMORY  2

extern int *page_address_container;

void test_subroutine_cleanup(void);
int allocate_page_addresses(int max_pages);
int deallocate_page_addresses(int max_pages);
int allocate_single_addresses(int total_size);
int deallocate_single_addresses(int total_size);

int fill_memory(int data_type, int fill_value, int memory_type);
int copy_from_buffer(uint8_t *source, int address, int size);
int copy_to_buffer(uint8_t *dest, int address, int size);

#endif
