/**
 * DO NOT CHANGE - you will not be submitting this file
 */

#ifndef INTERFACE_H
#define INTERFACE_H

#include <stdbool.h>
#include <stdint.h>

#define LOG_INFO                1
#define LOG_WARNING             2
#define LOG_ERROR               3
#define DISABLE_CONSOLE_LOGGING 4
#define LOG_CRITICAL            5

#define UNKNOWN_TYPE 0
#define BYTE_TYPE    1 //8 bits
#define WORD_TYPE    2 //16 bits
#define DWORD_TYPE   3 //32 bits
#define QWORD_TYPE   4 //64 bits

#define FAILURE           -1
#define SUCCESS           0
#define TRUE              1
#define FALSE             0
#define MAX_FILE_PATH_LEN 256

/**
 *  MC : Memory Configuration Structure
 *  @desc : This structure define the configuration and the layout of the memory configuration
 *  we use for the simulator. The memory has to be configured before the start of every test!
 */
typedef struct memory_configuration {
    int page_size;
    int data_size;
    int max_page_entries;
    int virtual_memory_size;
    int max_virtual_pages;
    int physical_memory_size;
    int max_physical_pages;
    int swap_memory_size;
    int max_swap_memory_pages;
    bool tlb_enabled;
    int max_tlb_entries;
} MC;

extern MC mem_config;

//Console logging function

/**
 * Console logging function
 *
 * @param log_type              :   Type of log : LOG_INFO, LOG_WARNING, LOG_ERROR, DISABLE_CONSOLE_LOGGING, LOG_CRITICAL
 * @param file_name             :   The file name in which the caller function resides
 * @param function              :   Name of the caller functions
 * @param line                  :   Line number where the caller function is located
 * @param format                :   Format specifier for the string to be printed
 * @param variable_arg          :   Variable arguments to the format specifier.
 */
void log_console(char log_type, const char *file_name, const char *function,
                 int line, const char *format, ...);

/**
 * Function to set the logging level
 *
 * @param level : logging level (LOG_INFO, LOG_WARNING, LOG_ERROR, DISABLE_CONSOLE_LOGGING, LOG_CRITICAL)
 */
void set_log_level(int level);

/**
 * Initialize memory configuration. Called by the testing framework
 *
 * @param page_size             :   size of the page in bytes
 * @param physical_mem_size     :   the size of the physical memory
 * @param virtual_mem_size      :   the size of the virtual memory
 * @param swap_mem_size         :   the size of the swap memory
 * @param tlb_enable            :   flag to enable/dissable TLB
 * @param max_tlb_entries       :   the size of the TLB
 */
void set_memory_configuration(int page_size, int physical_mem_size,
                              int virtual_mem_size, int swap_mem_size,
                              bool tlb_enable, int max_tlb_entries);

/**
 * Splits the virtual memory address to virtual page number and offset
 *
 * @param virtual_address       : virtual address to write to.
 * @param virtual_page_num      : pointer to the page number in virtual memory.
 * @param page_offset           : pointer to the page offset.
 *
 */
void split_virtual_address(int virtual_address, int *virtual_page_num,
                           int *page_offset);

/**
 * Allocate memory
 *
 * @param type      : type of variable to allocate (BYTE,WORD,DWORD,QWORD)
 * @param number    : number of variables to allocate
 *
 * @desc :
 * @return SUCCESS or FAILURE
 */
int pemmu_malloc(int type, int number);

/**
 * Free memory
 *
 * @param address   : memory address to free.
 *
 * @desc :
 * @return SUCCESS or FAILURE
 */
int pemmu_free(int address);

/**
 * Reads the value stored at given address
 *
 * @param address   : virtual address to read from.
 * @param data      : pointer to the data being read.
 * @param size      : size of the data being read.
 *
 * @desc :
 * @return SUCCESS or FAILURE
 */
int pemmu_read_data(int address, void *data, int size);

/**
 * Writes the value to a given address
 *
 * @param address   : virtual address to write to.
 * @param data      : pointer to the data being written.
 * @param size      : size of the data being written.
 *
 * @desc :
 * @return SUCCESS or FAILURE
 */
int pemmu_write_data(int address, void *data, int size);

#endif
