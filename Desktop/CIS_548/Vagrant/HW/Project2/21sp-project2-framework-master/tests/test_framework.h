/**
 * FRAMEWORK FILE
 * Feel free to add tests for your own testing purposes,
 * but remember that you will not be submitting this file
 *
 * Common header file for all testing functionality
 */

#ifndef TEST_FRAMEWORK_H
#define TEST_FRAMEWORK_H

#include <stdio.h>

typedef enum { FIFO, CLOCK, LRU } replacement_policy;

typedef struct test {
    int test_type;
    int data_type;
    int memory_configuration;
    int replacement_policy;
} test_t;

#define UNKNOWN_REPLACEMENT_POLICY 0
#define FIFO_REPLACEMENT_POLICY    1
#define CLOCK_REPLACEMENT_POLICY   2
#define LRU_REPLACEMENT_POLICY     3

extern const char *const replacement_policy_type_c[4];

#define RUN_ALL_TESTS 0
// ------- Tests ID 1 to 7 : Only Address Allocation ------- //
#define ADDRESS_ALLOCATION_SANITY_TEST         1
#define ADDRESS_ALLOCATION_FUNCTIONALITY_TEST  2
#define ADDRESS_ALLOCATION_COMBINE_BLOCKS_TEST 3
// ---- Tests ID 9 to 22 : Both Allocation + Translation ---- //
#define ADDRESS_ALLOCATION_OFFSET_ACCESS_TEST 9
#define PHYSICAL_MEMORY_FILL_TEST             10
#define VIRTUAL_MEMORY_FILL_TEST              11
#define WRITE_READBACK_TEST                   12
#define CLEAN_DIRTY_EVICTION_TEST             13
#define PATTERN_ACCCESS_TEST_FIFO_TEST        15
#define PATTERN_ACCCESS_TEST_CLOCK_TEST       16
#define PATTERN_ACCCESS_TEST_LRU_TEST         17
#define TLB_SANITY_TEST                       18
#define USER_DEFINED_ALLOCATION_TEST          23
#define USER_DEFINED_TRANSLATION_TEST         24
#define RUN_ANALYSIS_PROGRAMS                 25

extern const char *const test_type_c[100];

#define MEM_CONFIG_A     1
#define TLB_MEM_CONFIG_A 7

extern const char *const memory_config_type_c[10];

replacement_policy get_replacement_policy(void);

/**
 * Returns the handle to the swap file created by the framework
 *
 * Use this function to get the handle to the swap file that is
 * created by the framework.
 *
 * NOTE : DO NOT CHANGE THE SIGNATURE OF THE FUNCTION!
 *
 * @param None
 * @return Pointer to the swap file handle.
 */
FILE *get_swap_file_handle(void);

//Test Framework Initialization function
int test_framework_initialize(void);

//Test Framework Cleanup function
void test_framework_cleanup(void);

//Test Framework Memory Configuration function
void set_config(int mem_config);

int run_test_initilization_sequence(char *test_name, char *log_filename,
                                    int data_type, int memory_configuration,
                                    int replacement_policy);

//CUTest main function
int run_cutest_framework(int test_type, int memory_config,
                         int replacement_policy, int data_type);

#endif
