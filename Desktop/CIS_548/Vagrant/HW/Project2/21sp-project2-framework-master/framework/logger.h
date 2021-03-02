/**
 * DO NOT CHANGE - you will not be submitting this file
 *
 * Logging utility: to be used to ensure accurate grading
 */

#ifndef LOGGER_H
#define LOGGER_H

#include <stdbool.h>

#define MAX_LOG_FILE_NAME_SIZE 256

/* === USE THE FOLLOWING LOGGING FUNCTIONS IN YOUR IMPLEMENTATION === */

//TODO: ADD THESE FUNCTIONS TO YOUR CODE
/**
 * Logs page fault
 *
 * NOTE : MUST BE USED IN YOUR SUBMISSION CODE
 *
 * @param virtual_page_num that caused the page fault
 */
void log_page_fault(int virtual_page_num);

/**
 * Logs virtual address translation
 *
 * NOTE : MUST BE USED IN YOUR SUBMISSION CODE
 *
 * @param virtual_page_num
 * @param physical_page_num that correponds to the virtual_page_num
 */
void log_translate(int virtual_page_num, int physical_page_num);

/**
 * Logs eviction of page contained in physical_page_num
 *
 * @param physical_page_num that is being evicted
 * @param virtual_page_num that correponds to the physical_page_num being evicted
 */
void log_eviction(int physical_page_num, int virtual_page_num);

/**
 * Logs writeback operation of physical_page_num to disk
 *
 * @param physical_page_num that is being written to disk
 */
void log_disk_write(int physical_page_num);

/**
 * Logs addition of an entry to the TLB
 *
 * @param virtual_page_num that is being added to the TLB
 * @param physical_page_num that corresponds to the virtual_page_num being added to the TLB
 */
void log_add_TLB_entry(int virtual_page_num, int physical_page_num);

/**
 * Logs TLB hit
 *
 * @param virtual_page_num that caused the TLB hit
 * @param physical_page_num that corresponds to the virtual_page_num that caused the TLB hit
 */
void log_TLB_hit(int virtual_page_num, int physical_page_num);

/**
 * Logs TLB miss
 *
 * @param virtual_page_num that caused the TLB miss
 */
void log_TLB_miss(int virtual_page_num);

/* ==== DO NOT USE THE FOLLOWING. They are already called by the framework. === */
/**
 * Utility to set logfile to fname
 *
 * NOTE : Used by the testing framework. Don't use it in your submission code!
 *
 * @param fname
 */
int create_log_file(char *fname);

/**
 * Utility to close logfile
 *
 * NOTE : Used by the testing framework. Don't use it in your submission code!
 */
void close_log(void);

/**
 * Logging utility that logs config information
 *
 * NOTE : Used by the testing framework. Don't use it in your submission code!
 */
void log_config(void);

/**
 * Logs current replacement policy
 *
 * NOTE : Used by the testing framework. Don't use it in your submission code!
 *
 * @param policy
 */
void log_replacement_policy(const char *policy);

/**
 * Logging utility to record the creation of a new address
 *
 * NOTE : Used by the testing framework. Don't use it in your submission code!
 *
 * @param address
 */
void log_address_allocation(int address);

/**
 * Logging utility to log the freeing of a previously
 * allocated address
 *
 * NOTE : Used by the testing framework. Don't use it in your submission code!
 *
 * @param address
 */
void log_address_free(int address);

/**
 * Logging utility for the test framework to track
 * what the test is doing. Helps understand the logs
 * better.
 *
 * NOTE : Used by the testing framework. Don't use it in your submission code!
 *
 * @param message
 */
void log_test_framework(char *message);

/**
 * Function to output the log for the current operation
 *
 * NOTE : Used by the testing framework. Don't use it in your submission code!
 */
int log_mmu_operation(void);

/**
 * Function to initialize the log for the current operation
 *
 * NOTE : Used by the testing framework. Don't use it in your submission code!
 */
void init_mmu_operation_log(int address, int virtual_page_num, int page_offset,
                            int size, bool is_write);

#endif
