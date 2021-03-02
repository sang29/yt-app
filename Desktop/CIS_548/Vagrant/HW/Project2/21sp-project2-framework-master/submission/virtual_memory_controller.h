#ifndef VIRTUAL_MEMORY_CONTROLLER_H
#define VIRTUAL_MEMORY_CONTROLLER_H

/**
 * Initialization function that is called by the test framework
 * to set the system up.
 *
 * You should palce all of your system
 * initialization functions in this since the test framework calls
 * this function only once during the start of the test.
 *
 * NOTE : DO NOT CHANGE THE SIGNATURE OF THE FUNCTION!
 */
int master_initialization(void);

/**
 * Clean up function that is called by the test framework just
 * before the test is exited.
 *
 * You should place all of your system clean up functions in this
 * since the test framework calls this function only once during the
 * end of the test.
 *
 * Do not change the signature
 */
void master_clean_up(void);

/**
 * Your implementation of read functionality.
 * This function is invoked by the framework's pemmu_read function
 * to read value stored in virtual_page_num page at page_offset
 * offset into data variable
 *
 * NOTE : DO NOT CHANGE THE SIGNATURE OF THE FUNCTION!
 *
 * @param data
 * @param virtual_page_num
 * @param page_offset
 * @param data_size
 *
 * @return SUCCESS or FAILURE
 */
int mmu_read(void *data, int virtual_page_num, int page_offset, int data_size);

/**
 * Your implementation of write functionality.
 * This function is invoked by the framework's pemmu_write function
 * to write data value to virtual_page_num page at page_offset
 * offset
 *
 * NOTE : DO NOT CHANGE THE SIGNATURE OF THE FUNCTION!
 *
 * @param data
 * @param virtual_page_num
 * @param page_offset
 * @param data_size
 *
 * @return SUCCESS or FAILURE
 */
int mmu_write(const void *data, int virtual_page_num, int page_offset,
              int data_size);

/**
 * Your implementation of malloc functionality
 * This function is invoked by the framework's pemmu_malloc function
 * to allocate memory and return a virtual address for reads/writes
 *
 * NOTE : DO NOT CHANGE THE SIGNATURE OF THE FUNCTION!
 *
 * @param type
 * @param number
 *
 * @return virtual address on SUCCESS or FAILURE
 */
int mmu_allocate_variable(int type, int number);

/**
 * Your implementation of free functionality
 * This function is invoked by the framework's pemmu_free function
 * to free the memory that was previously allocated.
 *
 * NOTE : DO NOT CHANGE THE SIGNATURE OF THE FUNCTION!
 *
 * @param address
 *
 * @return SUCCESS or FAILURE
 */
int mmu_free_variable(int address);

#endif
