#include "test_allocation_subroutines.h"

#include <stdlib.h>
#include <stdarg.h>
#include "test_framework.h"
#include "test_common_subroutines.h"
#include "../framework/interface.h"
#include "../framework/logger.h"

/**
 *  Function : test_address_allocation_user_defined
 *
 *  @brief  Function where the user can write their own test
 *
 *  @return SUCCESS or FAILURE
 * */
int test_address_allocation_user_defined(int data_type,
                                         int memory_configuration)
{
    int ret_value = FAILURE;
    char log_filename[MAX_LOG_FILE_NAME_SIZE] = {};
    int ret = run_test_initilization_sequence(
        "address_allocation_user_defined_test", log_filename, data_type,
        memory_configuration, 0);
    if (ret != SUCCESS) {
        log_console(LOG_ERROR, __FILE__, __FUNCTION__, __LINE__,
                    "run_test_initilization_sequence Failed!\n");
        goto exit_condition;
    }

    log_console(LOG_INFO, "", "", __LINE__, "TEST START: %s\n", log_filename);
    //------------ START USER DEFINED TEST HERE --------------- //

exit_condition:
    if (ret_value == FAILURE) {
        log_console(LOG_ERROR, __FILE__, __FUNCTION__, __LINE__,
                    "TEST FAILED : %s\n", log_filename);
    } else {
        log_console(LOG_INFO, "", "", __LINE__, "TEST PASSED : %s\n",
                    log_filename);
    }

    return ret_value;
}

int test_address_allocation_offset_access(int data_type,
                                          int memory_configuration)
{
    int i, j, address, data_size;
    int data, expected_data;
    int ret_value = FAILURE;
    int max_addresses_to_write, offset, address_increment;
    char log_filename[MAX_LOG_FILE_NAME_SIZE] = {};

    //The replacement policy does not matter in this test
    int ret = run_test_initilization_sequence(
        "address_allocation_offset_access_test", log_filename, data_type,
        memory_configuration, 0);
    if (ret != SUCCESS) {
        log_console(LOG_ERROR, __FILE__, __FUNCTION__, __LINE__,
                    "run_test_initilization_sequence Failed!\n");
        goto exit_condition;
    }

    log_console(LOG_INFO, "", "", __LINE__, "TEST START: %s\n", log_filename);

    // Note: Data will wrap around for BYTE type test
    if (data_type == BYTE_TYPE) {
        address_increment = BYTE_TYPE_SIZE;
        data_size = BYTE_TYPE_SIZE;
        max_addresses_to_write =
            mem_config.max_page_entries / (2 * BYTE_TYPE_SIZE);
    } else if (data_type == WORD_TYPE) {
        address_increment = WORD_TYPE_SIZE;
        data_size = WORD_TYPE_SIZE;
        max_addresses_to_write =
            mem_config.max_page_entries / (2 * WORD_TYPE_SIZE);
    } else if (data_type == DWORD_TYPE) {
        address_increment = DWORD_TYPE_SIZE;
        data_size = DWORD_TYPE_SIZE;
        max_addresses_to_write =
            mem_config.max_page_entries / (2 * DWORD_TYPE_SIZE);
    } else if (data_type == QWORD_TYPE) {
        address_increment = QWORD_TYPE_SIZE;
        data_size = QWORD_TYPE_SIZE;
        max_addresses_to_write =
            mem_config.max_page_entries / (2 * QWORD_TYPE_SIZE);
    } else {
        log_console(LOG_CRITICAL, __FILE__, __FUNCTION__, __LINE__,
                    "Unsupported data type: %d!\n", data_type);
        goto exit_condition;
    }

    offset = mem_config.max_page_entries / 2;

    if (allocate_page_addresses(mem_config.max_physical_pages) != SUCCESS) {
        log_console(LOG_ERROR, __FILE__, __FUNCTION__, __LINE__,
                    "allocate_page_addresses of %d pages Failed\n",
                    mem_config.max_physical_pages);
        goto exit_condition;
    }

    //Write Data
    for (i = 0; i < mem_config.max_physical_pages; i++) {
        //Start the data write at an offset
        address = page_address_container[i] + offset;

        //In each loop write the page number as the data to the second half of the page! The data is an upcounter starting
        //from a value that is equal to the current page number
        data = i;
        for (j = 0; j < max_addresses_to_write; j++) {
            if (pemmu_write_data(address, (void *) &data, data_size) !=
                SUCCESS) {
                log_console(
                    LOG_ERROR, __FILE__, __FUNCTION__, __LINE__,
                    "pemmu_write_data Failed on address: %d for size: %d\n",
                    address, data_size);
                goto exit_condition;
            }
            address += address_increment;
            data++;
        }
    }

    //Read Data
    for (i = 0; i < mem_config.max_physical_pages; i++) {
        //Start the data read at an offset
        address = page_address_container[i] + offset;
        expected_data = i;
        for (j = 0; j < max_addresses_to_write; j++) {
            if (pemmu_read_data(address, (void *) &data, data_size) !=
                SUCCESS) {
                log_console(
                    LOG_ERROR, __FILE__, __FUNCTION__, __LINE__,
                    "pemmu_read_data Failed on address: %d for size: %d\n",
                    address, data_size);
                goto exit_condition;
            }
            if (data != expected_data) {
                log_console(LOG_CRITICAL, __FILE__, __FUNCTION__, __LINE__,
                            "Data missmatch! Expected: %d, Actual: %d\n",
                            expected_data, data);
                goto exit_condition;
            }
            address += address_increment;
            expected_data++;
        }
    }

    ret_value = SUCCESS;

exit_condition:
    if (ret_value == FAILURE) {
        log_console(LOG_ERROR, __FILE__, __FUNCTION__, __LINE__,
                    "TEST FAILED : %s\n", log_filename);
    } else {
        log_console(LOG_INFO, "", "", __LINE__, "TEST PASSED : %s\n",
                    log_filename);
    }
    return ret_value;
}

int test_address_allocation_combine_blocks(int data_type,
                                           int memory_configuration)
{
    int ret_value = FAILURE;
    int address;
    int counter;
    int expected_address;
    int increment_step = 0;
    int max_addresses = 0;
    char log_filename[MAX_LOG_FILE_NAME_SIZE] = {};

    //The replacement policy does not matter in this test
    int ret = run_test_initilization_sequence(
        "address_allocation_combine_blocks", log_filename, data_type,
        memory_configuration, 0);
    if (ret != SUCCESS) {
        log_console(LOG_ERROR, __FILE__, __FUNCTION__, __LINE__,
                    "run_test_initilization_sequence Failed!\n");
        return FAILURE;
    }

    log_console(LOG_INFO, "", "", __LINE__, "TEST START: %s\n", log_filename);

    if (data_type == BYTE_TYPE) {
        max_addresses = mem_config.virtual_memory_size;
        increment_step = BYTE_TYPE_SIZE;
    } else if (data_type == WORD_TYPE) {
        max_addresses = mem_config.virtual_memory_size / 2;
        increment_step = WORD_TYPE_SIZE;
    } else if (data_type == DWORD_TYPE) {
        max_addresses = mem_config.virtual_memory_size / 4;
        increment_step = DWORD_TYPE_SIZE;
    } else if (data_type == QWORD_TYPE) {
        max_addresses = mem_config.virtual_memory_size / 8;
        increment_step = QWORD_TYPE_SIZE;
    } else {
        log_console(LOG_CRITICAL, __FILE__, __FUNCTION__, __LINE__,
                    "Unsupported data type: %d!\n", data_type);
        goto exit_condition;
    }

    // Allocate addresses upto the maximum in allocation_size!
    counter = 0;
    expected_address = 0;
    while (counter < max_addresses) {
        address = pemmu_malloc(data_type, 1);
        if (address != expected_address) {
            log_console(LOG_ERROR, __FILE__, __FUNCTION__, __LINE__,
                        "Address Allocation Missmatch! Allocated Address: %d | "
                        "Expected Address: %d\n",
                        address, expected_address);
            goto exit_condition;
        }
        counter++;
        expected_address += increment_step;
    }

    // Free all the addresses allocated!
    counter = 0;
    address = 0;
    while (counter < max_addresses) {
        if (pemmu_free(address) != SUCCESS) {
            log_console(LOG_ERROR, __FILE__, __FUNCTION__, __LINE__,
                        "Failed to free address %d!\n", address);
            goto exit_condition;
        }
        counter++;
        address += increment_step;
    }

    // All the addresses blocks are now of a single address size. Allocate full pages now. The allocation algo should combine small blocks
    if (allocate_page_addresses(mem_config.max_virtual_pages) != SUCCESS) {
        log_console(LOG_ERROR, __FILE__, __FUNCTION__, __LINE__,
                    "allocate_page_addresses failed for pages: %d!\n",
                    mem_config.max_virtual_pages);
        goto exit_condition;
    }

    ret_value = SUCCESS;

exit_condition:
    if (ret_value == FAILURE) {
        log_console(LOG_ERROR, __FILE__, __FUNCTION__, __LINE__,
                    "TEST FAILED : %s\n", log_filename);
    } else {
        log_console(LOG_INFO, "", "", __LINE__, "TEST PASSED : %s\n",
                    log_filename);
    }
    return ret_value;
}

/**
 *  Function : test_address_allocation_functionality
 *
 *  @brief  Test function to test the allocation and free functionality of the address allocator.
 *
 *  @detail This test allocates 32 addresses and then frees every alternate address. Then 16 addresses
 *          are allocated again. These addresses should match the deleted once as per the first fit
 *          algorithm
 *
 *  @return SUCCESS or FAILURE
 * */
int test_address_allocation_functionality(int data_type,
                                          int memory_configuration)
{
    int ret_value = FAILURE;
    int counter;
    int max_addresses = 0;
    int expected_address;
    int increment_step = 0;
    int *address = NULL;
    char log_filename[MAX_LOG_FILE_NAME_SIZE] = {};

    //The replacement policy does not matter in this test
    int ret = run_test_initilization_sequence(
        "address_allocation_functionality_", log_filename, data_type,
        memory_configuration, 0);
    if (ret != SUCCESS) {
        log_console(LOG_ERROR, __FILE__, __FUNCTION__, __LINE__,
                    "run_test_initilization_sequence Failed!\n");
        goto exit_condition;
    }

    log_console(LOG_INFO, "", "", __LINE__, "TEST START: %s\n", log_filename);

    if (data_type == BYTE_TYPE) {
        increment_step = BYTE_TYPE_SIZE;
        max_addresses = mem_config.virtual_memory_size / BYTE_TYPE_SIZE;
    } else if (data_type == WORD_TYPE) {
        increment_step = WORD_TYPE_SIZE;
        max_addresses = mem_config.virtual_memory_size / WORD_TYPE_SIZE;
    } else if (data_type == DWORD_TYPE) {
        increment_step = DWORD_TYPE_SIZE;
        max_addresses = mem_config.virtual_memory_size / DWORD_TYPE_SIZE;
    } else if (data_type == QWORD_TYPE) {
        increment_step = QWORD_TYPE_SIZE;
        max_addresses = mem_config.virtual_memory_size / QWORD_TYPE_SIZE;
    } else {
        log_console(LOG_CRITICAL, __FILE__, __FUNCTION__, __LINE__,
                    "Unsupported data type: %d!\n", data_type);
        goto exit_condition;
    }

    address = (int *) calloc(max_addresses, sizeof(int));
    if (address == NULL) {
        goto exit_condition;
    }

    counter = 0;
    expected_address = 0;
    while (counter < max_addresses) {
        address[counter] = pemmu_malloc(data_type, 1);
        if (address[counter] != expected_address) {
            log_console(LOG_ERROR, __FILE__, __FUNCTION__, __LINE__,
                        "Address Allocation Missmatch! Allocated Address: %d | "
                        "Expected Address: %d\n",
                        address[counter], counter);
            goto exit_condition;
        }
        counter++;
        expected_address += increment_step;
    }

    // Free every alternate variable that we allocated in the previous step
    counter = 0;
    while (counter < max_addresses) {
        if (counter % 2 == 0) {
            pemmu_free(address[counter]);
        }
        counter++;
    }

    // Now if we allocate the variables again, by first fit algorithm, we expect the same addresses that were freed
    counter = 0;
    expected_address = 0;
    int variable;
    while (counter < max_addresses / 2) {
        variable = pemmu_malloc(data_type, 1);
        if (variable != expected_address) {
            log_console(LOG_ERROR, __FILE__, __FUNCTION__, __LINE__,
                        "Address Allocation Missmatch! Allocated Address: %d | "
                        "Expected Address: %d\n",
                        variable, expected_address);
            goto exit_condition;
        }
        counter++;
        expected_address += 2 * increment_step;
    }

    ret_value = SUCCESS;

exit_condition:

    //free local data structures used by the test
    if (address != NULL) {
        free(address);
    }
    if (ret_value == FAILURE) {
        log_console(LOG_ERROR, __FILE__, __FUNCTION__, __LINE__,
                    "TEST FAILED : %s\n", log_filename);
    } else {
        log_console(LOG_INFO, "", "", __LINE__, "TEST PASSED : %s\n",
                    log_filename);
    }
    return ret_value;
}

/**
 *  Function : test_address_allocation_sanity
 *
 *  @brief  Sanity function to test the address allocation of various supported data formats
 *
 *  @return SUCCESS or FAILURE
 * */
int test_address_allocation_sanity(int data_type, int memory_configuration)
{
    int ret_value = FAILURE;
    int address;
    int counter;
    int expected_address;
    int increment_step = 0;
    int max_addresses = 0;
    char log_filename[MAX_LOG_FILE_NAME_SIZE] = {};

    //The replacement policy does not matter in this test
    int ret = run_test_initilization_sequence("address_allocation_sanity_",
                                              log_filename, data_type,
                                              memory_configuration, 0);
    if (ret != SUCCESS) {
        log_console(LOG_ERROR, __FILE__, __FUNCTION__, __LINE__,
                    "run_test_initilization_sequence Failed!\n");
        goto exit_condition;
    }

    log_console(LOG_INFO, "", "", __LINE__, "TEST START: %s\n", log_filename);

    if (data_type == BYTE_TYPE) {
        max_addresses = mem_config.virtual_memory_size;
        increment_step = BYTE_TYPE_SIZE;
    } else if (data_type == WORD_TYPE) {
        max_addresses = mem_config.virtual_memory_size / 2;
        increment_step = WORD_TYPE_SIZE;
    } else if (data_type == DWORD_TYPE) {
        max_addresses = mem_config.virtual_memory_size / 4;
        increment_step = DWORD_TYPE_SIZE;
    } else if (data_type == QWORD_TYPE) {
        max_addresses = mem_config.virtual_memory_size / 8;
        increment_step = QWORD_TYPE_SIZE;
    } else {
        log_console(LOG_CRITICAL, __FILE__, __FUNCTION__, __LINE__,
                    "Unsupported data type: %d!\n", data_type);
        goto exit_condition;
    }

    // Allocate addresses upto the maximum in allocation_size!
    counter = 0;
    expected_address = 0;
    while (counter < max_addresses) {
        address = pemmu_malloc(data_type, 1);
        if (address != expected_address) {
            log_console(LOG_ERROR, __FILE__, __FUNCTION__, __LINE__,
                        "Address Allocation Missmatch! Allocated Address: %d | "
                        "Expected Address: %d\n",
                        address, expected_address);
            goto exit_condition;
        }
        counter++;
        expected_address += increment_step;
    }

    // Free all the addresses allocated!
    counter = 0;
    address = 0;
    while (counter < max_addresses) {
        if (pemmu_free(address) != SUCCESS) {
            log_console(LOG_ERROR, __FILE__, __FUNCTION__, __LINE__,
                        "Failed to free address %d!\n", address);
            goto exit_condition;
        }
        counter++;
        address += increment_step;
    }

    ret_value = SUCCESS;

exit_condition:
    if (ret_value == FAILURE) {
        log_console(LOG_ERROR, __FILE__, __FUNCTION__, __LINE__,
                    "TEST FAILED : %s\n", log_filename);
    } else {
        log_console(LOG_INFO, "", "", __LINE__, "TEST PASSED : %s\n",
                    log_filename);
    }
    return ret_value;
}
