//
// Created by Arvind Ramesh on 4/30/18.
//

#include "test_common_subroutines.h"

#include <memory.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include "test_framework.h"
#include "../framework/interface.h"
#include "../submission/virtual_memory_controller.h"
#include "../framework/logger.h"

int *page_address_container;

int copy_from_buffer(uint8_t *source, int address, int size)
{
    int i;
    uint8_t write_data;
    for (i = 0; i < size; i++) {
        write_data = *source;
        if (pemmu_write_data(address, (void *) &write_data, BYTE_TYPE_SIZE) !=
            SUCCESS) {
            log_console(LOG_ERROR, __FILE__, __FUNCTION__, __LINE__,
                        "pemmu_write_data failed!\n");
            return FAILURE;
        }
        address++;
        source++;
    }
    return SUCCESS;
}

int copy_to_buffer(uint8_t *dest, int address, int size)
{
    int i;
    uint8_t read_data;
    for (i = 0; i < size; i++) {
        read_data = 0;
        if (pemmu_read_data(address, (void *) &read_data, BYTE_TYPE_SIZE) !=
            SUCCESS) {
            log_console(LOG_ERROR, __FILE__, __FUNCTION__, __LINE__,
                        "pemmu_read_data failed!\n");
            return FAILURE;
        }
        *dest = read_data;
        address++;
        dest++;
    }
    return SUCCESS;
}

void test_subroutine_cleanup(void)
{
    if (page_address_container != NULL) {
        free(page_address_container);
        page_address_container = NULL;
    }
}

int allocate_single_addresses(int total_size)
{
    int i;
    int address;
    for (i = 0; i < total_size; i++) {
        address = pemmu_malloc(BYTE_TYPE, 1);
        if (address < 0) {
            log_console(LOG_ERROR, __FILE__, __FUNCTION__, __LINE__,
                        "pemmu_malloc failed!\n");
            return FAILURE;
        }
    }
    return SUCCESS;
}

int deallocate_single_addresses(int total_size)
{
    int i;
    for (i = 0; i < total_size; i++) {
        if (pemmu_free(i) != SUCCESS) {
            log_console(LOG_ERROR, __FILE__, __FUNCTION__, __LINE__,
                        "pemmu_free failed!\n");
            return FAILURE;
        }
    }
    return SUCCESS;
}

int allocate_page_addresses(int max_pages)
{
    int i, address;
    if (page_address_container == NULL) {
        log_console(LOG_CRITICAL, __FILE__, __FUNCTION__, __LINE__,
                    "page_address_container is NULL\n");
        return FAILURE;
    }

    for (i = 0; i < max_pages; i++) {
        address = pemmu_malloc(BYTE_TYPE, mem_config.max_page_entries);
        if (address < 0) {
            log_console(LOG_ERROR, __FILE__, __FUNCTION__, __LINE__,
                        "pemmu_malloc failed!\n");
            return FAILURE;
        }
        page_address_container[i] = address;
    }
    return SUCCESS;
}

int deallocate_page_addresses(int max_pages)
{
    int i;
    if (page_address_container == NULL) {
        log_console(LOG_CRITICAL, __FILE__, __FUNCTION__, __LINE__,
                    "page_address_container is NULL\n");
        return FAILURE;
    }

    for (i = 0; i < max_pages; i++) {
        if (pemmu_free(page_address_container[i]) != SUCCESS) {
            log_console(
                LOG_ERROR, __FILE__, __FUNCTION__, __LINE__,
                "pemmu_free failed on page address container number: %d\n", i);
        }
    }
    return SUCCESS;
}

int fill_memory(int data_type, int fill_value, int memory_type)
{
    int i, j;
    int max_pages;
    uint32_t offset;
    uint64_t data;
    int address = 0;

    if (memory_type == PHYSICAL_MEMORY) {
        max_pages = mem_config.max_physical_pages;
    } else if (memory_type == VIRTUAL_MEMORY) {
        max_pages = mem_config.max_virtual_pages;
    } else {
        log_console(LOG_CRITICAL, __FILE__, __FUNCTION__, __LINE__,
                    "Invalid memory type!\n");
        return FAILURE;
    }
    address = 0;
    if (data_type == BYTE_TYPE) {
        uint8_t read_data;
        offset = 0;
        for (i = 0; i < max_pages; i++) {
            for (j = 0; j < mem_config.max_page_entries; j++) {
                if (fill_value == FILL_TYPE_UPCOUNTER) {
                    data = (offset + address) % UINT8_MAX;
                } else if (fill_value == FILL_TYPE_PAGE_NO) {
                    data = (i % UINT8_MAX);
                } else {
                    data = 0;
                }
                if (pemmu_write_data(address, (void *) &data, BYTE_TYPE_SIZE) !=
                    SUCCESS) {
                    log_console(LOG_ERROR, __FILE__, __FUNCTION__, __LINE__,
                                "pemmu_write_data failed\n");
                    return FAILURE;
                }
                if (pemmu_read_data(address, (void *) &read_data,
                                    BYTE_TYPE_SIZE) != SUCCESS) {
                    log_console(LOG_ERROR, __FILE__, __FUNCTION__, __LINE__,
                                "pemmu_read_data failed\n");
                    return FAILURE;
                }
                if (data != read_data) {
                    log_console(LOG_CRITICAL, __FILE__, __FUNCTION__, __LINE__,
                                "Data missmatch! Expected: %d, Actual: %d\n",
                                data, read_data);
                    return FAILURE;
                }
                address += BYTE_TYPE_SIZE;
            }
        }
    } else if (data_type == WORD_TYPE) {
        uint16_t read_data;
        uint16_t offset = UINT8_MAX;
        for (i = 0; i < max_pages; i++) {
            for (j = 0; j < mem_config.max_page_entries / WORD_TYPE_SIZE; j++) {
                if (fill_value == FILL_TYPE_UPCOUNTER) {
                    data = (offset + address) % UINT16_MAX;
                } else if (fill_value == FILL_TYPE_PAGE_NO) {
                    data = (i % UINT16_MAX);
                } else {
                    data = 0;
                }
                if (pemmu_write_data(address, (void *) &data, WORD_TYPE_SIZE) !=
                    SUCCESS) {
                    log_console(LOG_ERROR, __FILE__, __FUNCTION__, __LINE__,
                                "pemmu_write_data failed\n");
                    return FAILURE;
                }
                if (pemmu_read_data(address, (void *) &read_data,
                                    WORD_TYPE_SIZE) != SUCCESS) {
                    log_console(LOG_ERROR, __FILE__, __FUNCTION__, __LINE__,
                                "pemmu_read_data failed\n");
                    return FAILURE;
                }
                if (data != read_data) {
                    log_console(LOG_CRITICAL, __FILE__, __FUNCTION__, __LINE__,
                                "Data missmatch! Expected: %d, Actual: %d\n",
                                data, read_data);
                    return FAILURE;
                }
                address += WORD_TYPE_SIZE;
            }
        }
    } else if (data_type == DWORD_TYPE) {
        uint32_t read_data;
        uint32_t offset = UINT16_MAX;
        for (i = 0; i < max_pages; i++) {
            for (j = 0; j < mem_config.max_page_entries / DWORD_TYPE_SIZE;
                 j++) {
                if (fill_value == FILL_TYPE_UPCOUNTER) {
                    data = (offset + address) % UINT32_MAX;
                } else if (fill_value == FILL_TYPE_PAGE_NO) {
                    data = (i % UINT32_MAX);
                } else {
                    data = 0;
                }
                if (pemmu_write_data(address, (void *) &data,
                                     DWORD_TYPE_SIZE) != SUCCESS) {
                    log_console(LOG_ERROR, __FILE__, __FUNCTION__, __LINE__,
                                "pemmu_write_data failed\n");
                    return FAILURE;
                }
                if (pemmu_read_data(address, (void *) &read_data,
                                    DWORD_TYPE_SIZE) != SUCCESS) {
                    log_console(LOG_ERROR, __FILE__, __FUNCTION__, __LINE__,
                                "pemmu_read_data failed\n");
                    return FAILURE;
                }
                if (data != read_data) {
                    log_console(LOG_CRITICAL, __FILE__, __FUNCTION__, __LINE__,
                                "Data missmatch! Expected: %d, Actual: %d\n",
                                data, read_data);
                    return FAILURE;
                }
                address += DWORD_TYPE_SIZE;
            }
        }
    } else if (data_type == QWORD_TYPE) {
        uint64_t read_data;
        uint64_t offset = UINT32_MAX;
        for (i = 0; i < max_pages; i++) {
            for (j = 0; j < mem_config.max_page_entries / QWORD_TYPE_SIZE;
                 j++) {
                if (fill_value == FILL_TYPE_UPCOUNTER) {
                    data = (offset + address) % UINT32_MAX;
                } else if (fill_value == FILL_TYPE_PAGE_NO) {
                    data = i;
                } else {
                    data = 0;
                }
                if (pemmu_write_data(address, (void *) &data,
                                     QWORD_TYPE_SIZE) != SUCCESS) {
                    log_console(LOG_ERROR, __FILE__, __FUNCTION__, __LINE__,
                                "pemmu_write_data failed\n");
                    return FAILURE;
                }
                if (pemmu_read_data(address, (void *) &read_data,
                                    QWORD_TYPE_SIZE) != SUCCESS) {
                    log_console(LOG_ERROR, __FILE__, __FUNCTION__, __LINE__,
                                "pemmu_read_data failed\n");
                    return FAILURE;
                }
                if (data != read_data) {
                    log_console(LOG_CRITICAL, __FILE__, __FUNCTION__, __LINE__,
                                "Data missmatch! Expected: %d, Actual: %d\n",
                                data, read_data);
                    return FAILURE;
                }
                address += QWORD_TYPE_SIZE;
            }
        }
    }
    return SUCCESS;
}
