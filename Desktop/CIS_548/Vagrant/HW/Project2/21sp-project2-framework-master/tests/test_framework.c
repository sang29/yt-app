/**
 * FRAMEWORK FILE
 * Feel free to add tests for your own testing purposes,
 * but remember that you will not be submitting this file
 *
 * Testing framework controller
 */

#include "test_framework.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "CuTest.h"
#include "test_common_subroutines.h"
#include "test_allocation_subroutines.h"
#include "test_translation_subroutines.h"
#include "../framework/interface.h"
#include "../framework/logger.h"
#include "../submission/virtual_memory_controller.h"
#include "../submission/analysis_programs.h"

const char *const replacement_policy_type_c[4] = {
    [UNKNOWN_REPLACEMENT_POLICY] = "Unknown Replacement Policy",
    [FIFO_REPLACEMENT_POLICY] = "FIFO Replacement Policy",
    [CLOCK_REPLACEMENT_POLICY] = "CLOCK Replacement Policy",
    [LRU_REPLACEMENT_POLICY] = "LRU Replacement Policy",
};

const char *const test_type_c[100] = {
    [RUN_ALL_TESTS] = "Complete Suite of Tests",
    [ADDRESS_ALLOCATION_SANITY_TEST] = "Address Allocation Sanity Test",
    [ADDRESS_ALLOCATION_FUNCTIONALITY_TEST] =
        "Address Allocation Functionality Test",
    [ADDRESS_ALLOCATION_COMBINE_BLOCKS_TEST] =
        "Address Allocation Combine Blocks Test",
    [ADDRESS_ALLOCATION_OFFSET_ACCESS_TEST] =
        "Address Allocation Offset Access Test",
    [PHYSICAL_MEMORY_FILL_TEST] =
        "Address Translation Physical Memory Fill Test",
    [VIRTUAL_MEMORY_FILL_TEST] = "Address Translation Virtual Memory Fill Test",
    [WRITE_READBACK_TEST] = "Address Translation Write Readback Test",
    [CLEAN_DIRTY_EVICTION_TEST] =
        "Address Translation Clean/Dirty Evictions Test",
    [PATTERN_ACCCESS_TEST_FIFO_TEST] =
        "Address Translation FIFO Pattern Access Test",
    [PATTERN_ACCCESS_TEST_CLOCK_TEST] =
        "Address Translation CLOCK Pattern Access Test",
    [PATTERN_ACCCESS_TEST_LRU_TEST] =
        "Address Translation LRU Pattern Access Test",
    [TLB_SANITY_TEST] = "Address Translation TLB Sanity Test",
    [USER_DEFINED_ALLOCATION_TEST] = "User Defined Allocation Test",
    [USER_DEFINED_TRANSLATION_TEST] = "User Defined Translation Test",
    [RUN_ANALYSIS_PROGRAMS] = "Analysis Programs",
};

const char *const memory_config_type_c[10] = {
    [MEM_CONFIG_A] =
        "A : [Page Size: 256 | Physical : 256*16 | Virtual : 256*32 | TLB : 0]",
    [TLB_MEM_CONFIG_A] = "TLB_A : [Page Size: 32 | Physical : 32*16 | Virtual "
                         ": 32*64 | TLB : 4]",
};

static test_t test_params;
static replacement_policy current_policy;
static FILE *swap_file_handle = NULL;

static void suite_test_address_allocation_sanity(CuTest *testContext)
{
    int ret = FAILURE;
    set_log_level(LOG_INFO);
    if (test_params.test_type == ADDRESS_ALLOCATION_SANITY_TEST ||
        test_params.test_type == RUN_ALL_TESTS) {
        ret = test_address_allocation_sanity(test_params.data_type,
                                             test_params.memory_configuration);
        test_framework_cleanup();
        CuAssertIntEquals(testContext, SUCCESS, ret);
    } else {
        CuTestSkip(testContext);
    }
}

static void suite_test_address_allocation_functionality(CuTest *testContext)
{
    int ret = FAILURE;
    set_log_level(LOG_INFO);
    if (test_params.test_type == ADDRESS_ALLOCATION_FUNCTIONALITY_TEST ||
        test_params.test_type == RUN_ALL_TESTS) {
        ret = test_address_allocation_functionality(
            test_params.data_type, test_params.memory_configuration);
        test_framework_cleanup();
        CuAssertIntEquals(testContext, SUCCESS, ret);
    } else {
        CuTestSkip(testContext);
    }
}

static void suite_test_address_allocation_combine_blocks(CuTest *testContext)
{
    int ret = FAILURE;
    set_log_level(LOG_INFO);
    if (test_params.test_type == ADDRESS_ALLOCATION_COMBINE_BLOCKS_TEST ||
        test_params.test_type == RUN_ALL_TESTS) {
        ret = test_address_allocation_combine_blocks(
            test_params.data_type, test_params.memory_configuration);
        test_framework_cleanup();
        CuAssertIntEquals(testContext, SUCCESS, ret);
    } else {
        CuTestSkip(testContext);
    }
}

static void suite_test_address_allocation_offset_access(CuTest *testContext)
{
    int ret = FAILURE;
    set_log_level(LOG_INFO);
    if (test_params.test_type == ADDRESS_ALLOCATION_OFFSET_ACCESS_TEST ||
        test_params.test_type == RUN_ALL_TESTS) {
        ret = test_address_allocation_offset_access(
            test_params.data_type, test_params.memory_configuration);
        test_framework_cleanup();
        CuAssertIntEquals(testContext, SUCCESS, ret);
    } else {
        CuTestSkip(testContext);
    }
}

static void suite_test_fill_physical_memory(CuTest *testContext)
{
    int ret = FAILURE;
    set_log_level(LOG_INFO);
    if (test_params.test_type == PHYSICAL_MEMORY_FILL_TEST ||
        test_params.test_type == RUN_ALL_TESTS) {
        ret = test_fill_physical_memory(test_params.data_type,
                                        test_params.memory_configuration);
        test_framework_cleanup();
        CuAssertIntEquals(testContext, SUCCESS, ret);
    } else {
        CuTestSkip(testContext);
    }
}

static void suite_test_fill_virtual_memory(CuTest *testContext)
{
    int ret = FAILURE;
    set_log_level(LOG_INFO);
    if (test_params.test_type == VIRTUAL_MEMORY_FILL_TEST ||
        test_params.test_type == RUN_ALL_TESTS) {
        ret = test_fill_virtual_memory(test_params.data_type,
                                       test_params.memory_configuration,
                                       test_params.replacement_policy);
        test_framework_cleanup();
        CuAssertIntEquals(testContext, SUCCESS, ret);
    } else {
        CuTestSkip(testContext);
    }
}

static void suite_test_write_readback_memory(CuTest *testContext)
{
    int ret = FAILURE;
    set_log_level(LOG_INFO);
    if (test_params.test_type == WRITE_READBACK_TEST ||
        test_params.test_type == RUN_ALL_TESTS) {
        ret = test_write_readback(test_params.data_type,
                                  test_params.memory_configuration,
                                  test_params.replacement_policy);
        test_framework_cleanup();
        CuAssertIntEquals(testContext, SUCCESS, ret);
    }

    else {
        CuTestSkip(testContext);
    }
}

static void suite_test_clean_dirty_evictions(CuTest *testContext)
{
    int ret = FAILURE;
    set_log_level(LOG_INFO);
    if (test_params.test_type == CLEAN_DIRTY_EVICTION_TEST ||
        test_params.test_type == RUN_ALL_TESTS) {
        ret = test_clean_dirty_eviction(test_params.data_type,
                                        test_params.memory_configuration,
                                        test_params.replacement_policy);
        test_framework_cleanup();
        CuAssertIntEquals(testContext, SUCCESS, ret);
    } else {
        CuTestSkip(testContext);
    }
}

static void suite_test_pattern_access_fifo(CuTest *testContext)
{
    int ret = FAILURE;
    set_log_level(LOG_INFO);
    if (test_params.test_type == PATTERN_ACCCESS_TEST_FIFO_TEST ||
        test_params.test_type == RUN_ALL_TESTS) {
        ret = test_data_access_pattern_fifo(test_params.memory_configuration);
        test_framework_cleanup();
        CuAssertIntEquals(testContext, SUCCESS, ret);
    } else {
        CuTestSkip(testContext);
    }
}

static void suite_test_pattern_access_clock(CuTest *testContext)
{
    int ret = FAILURE;
    set_log_level(LOG_INFO);
    if (test_params.test_type == PATTERN_ACCCESS_TEST_CLOCK_TEST ||
        test_params.test_type == RUN_ALL_TESTS) {
        ret = test_data_access_pattern_clock(test_params.memory_configuration);
        test_framework_cleanup();
        CuAssertIntEquals(testContext, SUCCESS, ret);
    } else {
        CuTestSkip(testContext);
    }
}

static void suite_test_pattern_access_lru(CuTest *testContext)
{
    int ret = FAILURE;
    set_log_level(LOG_INFO);
    if (test_params.test_type == PATTERN_ACCCESS_TEST_LRU_TEST ||
        test_params.test_type == RUN_ALL_TESTS) {
        ret = test_data_access_pattern_lru(test_params.memory_configuration);
        test_framework_cleanup();
        CuAssertIntEquals(testContext, SUCCESS, ret);
    } else {
        CuTestSkip(testContext);
    }
}

static void suite_tlb_sanity(CuTest *testContext)
{
    int ret = FAILURE;
    set_log_level(LOG_INFO);
    if (test_params.test_type == TLB_SANITY_TEST ||
        test_params.test_type == RUN_ALL_TESTS) {
        ret = test_tlb_sanity(test_params.memory_configuration,
                              test_params.replacement_policy);
        test_framework_cleanup();
        CuAssertIntEquals(testContext, SUCCESS, ret);
    } else {
        CuTestSkip(testContext);
    }
}

static void suite_test_user_defined_allocation(CuTest *testContext)
{
    int ret = FAILURE;
    set_log_level(LOG_INFO);
    if (test_params.test_type == USER_DEFINED_ALLOCATION_TEST) {
        ret = test_address_allocation_user_defined(
            test_params.data_type, test_params.memory_configuration);
        test_framework_cleanup();
        CuAssertIntEquals(testContext, SUCCESS, ret);
    } else {
        CuTestSkip(testContext);
    }
}

static void suite_test_user_defined_translation(CuTest *testContext)
{
    int ret = FAILURE;
    set_log_level(LOG_INFO);
    if (test_params.test_type == USER_DEFINED_TRANSLATION_TEST) {
        ret = test_translation_user_defined(test_params.data_type,
                                            test_params.memory_configuration,
                                            test_params.replacement_policy);
        test_framework_cleanup();
        CuAssertIntEquals(testContext, SUCCESS, ret);
    } else {
        CuTestSkip(testContext);
    }
}

/**
 * Received command line argument that is to be used as the
 * replacement policy for this run.
 *
 * The test framework will call this function to configure the
 * replacement policy for that particular test run.
 *
 * @param policy
 * @return SUCCESS or FAILURE
 */
static int set_replacement_policy(const char *policy)
{
    log_replacement_policy(policy);

    if (strcmp(policy, "fifo") == 0)
        current_policy = FIFO;
    else if (strcmp(policy, "clock") == 0)
        current_policy = CLOCK;
    else if (strcmp(policy, "lru") == 0)
        current_policy = LRU;
    else
        return FAILURE;

    return SUCCESS;
}

/**
 * Entry point to run the analysis programs
 * @return SUCCESS or FAILURE
 */
static int run_analysis_programs(void)
{
    int ret = SUCCESS;
    set_config(MEM_CONFIG_A);

    /* ------------ Analysis Program 1.1: fifo_over_clock with fifo ------------ */
    if (create_log_file("fifo_over_clock_FIFO") != SUCCESS) {
        return FAILURE;
    }
    set_replacement_policy("fifo");
    if (test_framework_initialize() != SUCCESS) {
        return FAILURE;
    }
    if ((fifo_over_clock()) == FAILURE) {
        ret = FAILURE;
        printf("FIFO OVER CLOCK FAILED\n");
    }
    test_framework_cleanup();

    /* ------------ Analysis Program 1.2:  fifo_over_clock with clock ------------ */
    if (create_log_file("fifo_over_clock_CLOCK") != SUCCESS) {
        return FAILURE;
    }
    set_replacement_policy("clock");
    test_framework_initialize();
    if ((fifo_over_clock()) == FAILURE) {
        ret = FAILURE;
        printf("FIFO OVER CLOCK FAILED\n");
    }
    test_framework_cleanup();

    /* ------------ Analysis Program 2.1: clock_over_fifo with clock  ------------ */
    if (create_log_file("clock_over_fifo_CLOCK") != SUCCESS) {
        return FAILURE;
    }
    set_replacement_policy("clock");
    test_framework_initialize();
    if ((clock_over_fifo()) == FAILURE) {
        ret = FAILURE;
        printf("CLOCK over FIFOE FAILED\n");
    }
    test_framework_cleanup();
    /* ------------ Analysis Program 2.2: clock_over_fifo with fifo ------------ */
    if (create_log_file("clock_over_fifo_FIFO") != SUCCESS) {
        return FAILURE;
    }
    set_replacement_policy("fifo");
    test_framework_initialize();
    if ((clock_over_fifo()) == FAILURE) {
        ret = FAILURE;
        printf("CLOCK over FIFOE FAILED\n");
    }
    test_framework_cleanup();

    /* ------------ Analysis Program 3.1: lru_over_clock with lru ------------ */
    if (create_log_file("lru_over_clock_LRU") != SUCCESS) {
        return FAILURE;
    }
    set_replacement_policy("lru");
    test_framework_initialize();
    if ((lru_over_clock()) == FAILURE) {
        ret = FAILURE;
        printf("LRU over CLOCK FAILED\n");
    }
    test_framework_cleanup();

    /* ------------ Analysis Program 3.2: lru_over_clock with clock ------------ */
    if (create_log_file("lru_over_clock_CLOCK") != SUCCESS) {
        return FAILURE;
    }
    set_replacement_policy("clock");
    test_framework_initialize();
    if ((lru_over_clock()) == FAILURE) {
        ret = FAILURE;
        printf("LRU over CLOCK FAILED\n");
    }
    test_framework_cleanup();

    /* ------------ Analysis Program 4.1: clock_over_lru with clock ------------ */
    if (create_log_file("clock_over_lru_CLOCK") != SUCCESS) {
        return FAILURE;
    }
    set_replacement_policy("clock");
    test_framework_initialize();
    if ((clock_over_lru()) == FAILURE) {
        ret = FAILURE;
        printf("CLOCK over LRU FAILED\n");
    }
    test_framework_cleanup();

    /* ------------ Analysis Program 4.2: clock_over_lru with lru ------------ */
    if (create_log_file("clock_over_lru_LRU") != SUCCESS) {
        return FAILURE;
    }
    set_replacement_policy("lru");
    test_framework_initialize();
    if ((clock_over_lru()) == FAILURE) {
        ret = FAILURE;
        printf("CLOCK over LRU FAILED\n");
    }
    test_framework_cleanup();

    /* ------------ Analysis Program 5.1: fifo_over_lru with fifo ------------ */
    if (create_log_file("fifo_over_lru_FIFO") != SUCCESS) {
        return FAILURE;
    }
    set_replacement_policy("fifo");
    test_framework_initialize();
    if ((fifo_over_lru()) == FAILURE) {
        ret = FAILURE;
        printf("FIFO over LRU FAILED\n");
    }
    test_framework_cleanup();

    /* ------------ Analysis Program 5.2: fifo_over_lru ------------ */
    if (create_log_file("fifo_over_lru_LRU") != SUCCESS) {
        return FAILURE;
    }
    set_replacement_policy("lru");
    test_framework_initialize();
    if ((fifo_over_lru()) == FAILURE) {
        ret = FAILURE;
        printf("FIFO over LRU FAILED\n");
    }
    test_framework_cleanup();

    /* ------------ Analysis Program 6.1: lru_over_fifo ------------ */
    if (create_log_file("lru_over_fifo_LRU") != SUCCESS) {
        return FAILURE;
    }
    set_replacement_policy("lru");
    test_framework_initialize();
    if ((lru_over_fifo()) == FAILURE) {
        ret = FAILURE;
        printf("LRU over FIFO FAILED\n");
    }
    test_framework_cleanup();

    /* ------------ Analysis Program 6.2: lru_over_fifo ------------ */
    if (create_log_file("lru_over_fifo_FIFO") != SUCCESS) {
        return FAILURE;
    }
    set_replacement_policy("fifo");
    test_framework_initialize();
    if ((lru_over_fifo()) == FAILURE) {
        ret = FAILURE;
        printf("LRU over FIFO FAILED\n");
    }

    return ret;
}

static void suite_test_analysis_programs(CuTest *testContext)
{
    int ret = FAILURE;
    set_log_level(LOG_INFO);
    if (test_params.test_type == RUN_ANALYSIS_PROGRAMS) {
        ret = run_analysis_programs();
        test_framework_cleanup();
        CuAssertIntEquals(testContext, SUCCESS, ret);
    } else {
        CuTestSkip(testContext);
    }
}

static int open_swap_file(void)
{
    char file_name[128] = {0};
    int pid = getpid();
    sprintf(file_name, "temp/%d_swap.txt", pid);
    swap_file_handle = fopen(file_name, "w+");
    if (swap_file_handle == NULL) {
        log_console(LOG_CRITICAL, __FILE__, __FUNCTION__, __LINE__,
                    "Failed to open swap file: %s\n", file_name);
        return FAILURE;
    }

    return SUCCESS;
}

static void close_swap_file(void)
{
    if (swap_file_handle != NULL) {
        fclose(swap_file_handle);
        swap_file_handle = NULL;
    }
}

replacement_policy get_replacement_policy(void)
{
    return current_policy;
}

FILE *get_swap_file_handle(void)
{
    return swap_file_handle;
}

void set_config(int mem_config)
{
    if (mem_config == MEM_CONFIG_A) {
        set_memory_configuration(256, 256 * 16, 256 * 32, 256 * 32, false, 0);
    } else if (mem_config == TLB_MEM_CONFIG_A) {
        set_memory_configuration(32, 32 * 16, 32 * 64, 32 * 64, true, 4);
    }
}

int test_framework_initialize(void)
{
    log_config();
    if (open_swap_file() != SUCCESS || master_initialization() != SUCCESS) {
        return FAILURE;
    }
    return SUCCESS;
}

void test_framework_cleanup(void)
{
    test_subroutine_cleanup();
    master_clean_up();
    close_swap_file();
    close_log();
}

int run_test_initilization_sequence(char *test_name, char *log_filename,
                                    int data_type, int memory_configuration,
                                    int replacement_policy)
{

    if ((memory_configuration != MEM_CONFIG_A) &&
        (memory_configuration != TLB_MEM_CONFIG_A)) {
        log_console(LOG_CRITICAL, __FILE__, __FUNCTION__, __LINE__,
                    "Unsupported memory configuration\n", log_filename);
        return FAILURE;
    }

    strcat(log_filename, test_name);

    if (replacement_policy == FIFO_REPLACEMENT_POLICY) {
        strcat(log_filename, "_FIFO");
    } else if (replacement_policy == CLOCK_REPLACEMENT_POLICY) {
        strcat(log_filename, "_CLOCK");
    } else if (replacement_policy == LRU_REPLACEMENT_POLICY) {
        strcat(log_filename, "_LRU");
    }

    if (data_type == BYTE_TYPE) {
        strcat(log_filename, "_BYTE");
    } else if (data_type == WORD_TYPE) {
        strcat(log_filename, "_WORD");
    } else if (data_type == DWORD_TYPE) {
        strcat(log_filename, "_DWORD");
    } else if (data_type == QWORD_TYPE) {
        strcat(log_filename, "_QWORD");
    }

    switch (memory_configuration) {
    case MEM_CONFIG_A:
        strcat(log_filename, "_CONFIG_A");
        break;
    case TLB_MEM_CONFIG_A:
        strcat(log_filename, "_TLB_CONFIG_A");
        break;
    default:
        strcat(log_filename, "_UNKNOWN_MEM_CONFIG");
    }

    if (create_log_file(log_filename) != SUCCESS) {
        log_console(LOG_CRITICAL, __FILE__, __FUNCTION__, __LINE__,
                    "Failed to create log file: %s\n", log_filename);
        return FAILURE;
    }

    //Set memory configuration and replcement policy
    set_config(memory_configuration);
    if (replacement_policy == FIFO_REPLACEMENT_POLICY) {
        set_replacement_policy("fifo");
    } else if (replacement_policy == CLOCK_REPLACEMENT_POLICY) {
        set_replacement_policy("clock");
    } else if (replacement_policy == LRU_REPLACEMENT_POLICY) {
        set_replacement_policy("lru");
    }

    // Allocate memory to store the page addresses allocated
    page_address_container =
        (int *) calloc(mem_config.max_virtual_pages + 1, sizeof(int));
    if (page_address_container == NULL) {
        log_console(LOG_ERROR, __FILE__, __FUNCTION__, __LINE__,
                    "Failed to allocate memory for page_address_container!\n");
        return FAILURE;
    }

    // Initialize all the page_addresses to a negative value.
    memset(page_address_container, 0, mem_config.max_virtual_pages + 1);

    //Initialize the test framework which includes setting the config and policy
    if (test_framework_initialize() != SUCCESS) {
        log_console(LOG_ERROR, __FILE__, __FUNCTION__, __LINE__,
                    "test_framework_initialize failed!\n");
        return FAILURE;
    }
    return SUCCESS;
}

int run_cutest_framework(int test_type, int data_type, int memory_config,
                         int replacement_policy)
{

    int ret = -1;

    test_params.test_type = test_type;
    test_params.memory_configuration = memory_config;
    test_params.replacement_policy = replacement_policy;
    test_params.data_type = data_type;

    CuString *output = CuStringNew();

    //Create Master Suite (collection of suits)
    CuSuite *master_suite = CuSuiteNew();

    SUITE_ADD_TEST(master_suite, suite_test_address_allocation_sanity);
    SUITE_ADD_TEST(master_suite, suite_test_address_allocation_functionality);
    SUITE_ADD_TEST(master_suite, suite_test_address_allocation_combine_blocks);
    SUITE_ADD_TEST(master_suite, suite_test_address_allocation_offset_access);
    SUITE_ADD_TEST(master_suite, suite_test_fill_physical_memory);
    SUITE_ADD_TEST(master_suite, suite_test_fill_virtual_memory);
    SUITE_ADD_TEST(master_suite, suite_test_write_readback_memory);
    SUITE_ADD_TEST(master_suite, suite_test_clean_dirty_evictions);
    SUITE_ADD_TEST(master_suite, suite_test_pattern_access_fifo);
    SUITE_ADD_TEST(master_suite, suite_test_pattern_access_clock);
    SUITE_ADD_TEST(master_suite, suite_test_pattern_access_lru);
    SUITE_ADD_TEST(master_suite, suite_tlb_sanity);
    SUITE_ADD_TEST(master_suite, suite_test_user_defined_allocation);
    SUITE_ADD_TEST(master_suite, suite_test_user_defined_translation);
    SUITE_ADD_TEST(master_suite, suite_test_analysis_programs);

    //Run the Master Suite
    CuSuiteRun(master_suite);

    //Collect the details of the master suite run!
    ret = CuSuiteDetails(master_suite, output);

    //Print the results
    printf("%s\n", output->buffer);

    CuStringDelete(output);
    CuSuiteDelete(master_suite);

    return ret;
}
