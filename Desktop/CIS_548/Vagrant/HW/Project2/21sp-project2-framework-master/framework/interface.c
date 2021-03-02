/**
 * DO NOT CHANGE - you will not be submitting this file
 *
 * Interface Controller that behaves as a lightweight OS
 */

#include "interface.h"

#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdarg.h>
#include <assert.h>
#include "logger.h"
#include "../submission/virtual_memory_controller.h"

MC mem_config;  // Global definition of the memory configuration

static int virtual_page_offset_bits;
static int g_log_level;

void set_log_level(int level)
{
    g_log_level = level;
}

void log_console(char log_type, const char *file_name, const char *function,
                 int line, const char *format, ...)
{
    char buffer[256] = {};
    char location_info[128] = {};
    va_list args;

    if (log_type < g_log_level) {
        return;
    }
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer) - 1, format, args);
    if (strlen(file_name) > 0) {
        sprintf(location_info, "[%s : %s() : line %d]", file_name, function,
                line);
    }

    if (log_type == LOG_INFO) {
        printf("[INFO] %s %s", location_info, buffer);
    } else if (log_type == LOG_WARNING) {
        printf("[WARNING] %s %s", location_info, buffer);
    } else if (log_type == LOG_ERROR) {
        printf("[ERROR] %s %s", location_info, buffer);
    } else if (log_type == LOG_CRITICAL) {
        printf("[CRITICAL] %s %s", location_info, buffer);
    } else {
        printf("[UNKNOWN] %s %s", location_info, buffer);
    }
    va_end(args);
}

void set_memory_configuration(int page_size, int physical_mem_size,
                              int virtual_mem_size, int swap_mem_size,
                              bool tlb_enable, int max_tlb_entries)
{
    memset(&mem_config, 0, sizeof(MC));
    virtual_page_offset_bits = 0;

    if (!(physical_mem_size &&
          !(physical_mem_size & (physical_mem_size - 1)))) {
        log_console(LOG_CRITICAL, __FILE__, __FUNCTION__, __LINE__,
                    "Physical Memory size %d is not a power of 2\n",
                    physical_mem_size);
        return;
    }
    if (!(virtual_mem_size && !(virtual_mem_size & (virtual_mem_size - 1)))) {
        log_console(LOG_CRITICAL, __FILE__, __FUNCTION__, __LINE__,
                    "Virtual Memory size %d is not a power of 2\n",
                    virtual_mem_size);
        return;
    }
    if (!(page_size && !(page_size & (page_size - 1)))) {
        log_console(LOG_CRITICAL, __FILE__, __FUNCTION__, __LINE__,
                    "Page size %d is not a power of 2\n", page_size);
        return;
    }
    if (mem_config.virtual_memory_size < mem_config.physical_memory_size) {
        log_console(
            LOG_CRITICAL, __FILE__, __FUNCTION__, __LINE__,
            "Virtual Memory Size: %d is less than Physical Memory Size: %d\n",
            mem_config.virtual_memory_size, mem_config.physical_memory_size);
        return;
    }

    mem_config.page_size = page_size;
    mem_config.data_size = sizeof(char);
    mem_config.max_page_entries = page_size / mem_config.data_size;
    assert(mem_config.max_page_entries > 1);
    mem_config.physical_memory_size = physical_mem_size;
    mem_config.virtual_memory_size = virtual_mem_size;
    mem_config.swap_memory_size = swap_mem_size;
    mem_config.max_physical_pages = physical_mem_size / page_size;
    mem_config.max_virtual_pages = virtual_mem_size / page_size;
    mem_config.max_swap_memory_pages = swap_mem_size / page_size;
    mem_config.tlb_enabled = tlb_enable;
    mem_config.max_tlb_entries = max_tlb_entries;
    virtual_page_offset_bits = log2(mem_config.max_page_entries);
}

void split_virtual_address(int virtual_address, int *virtual_page_num,
                           int *page_offset)
{
    // extract virtual page number
    *virtual_page_num = virtual_address >> virtual_page_offset_bits;
    // extract page offset
    const int page_offset_mask = (1 << virtual_page_offset_bits) - 1;

    *page_offset = virtual_address & page_offset_mask;
}

int pemmu_write_data(int address, void *data, int size)
{
    assert(address >= 0);
    int virtual_page_num, page_offset;
    split_virtual_address(address, &virtual_page_num, &page_offset);
    assert(virtual_page_num <= mem_config.max_virtual_pages);
    assert(page_offset <= mem_config.max_page_entries);
    init_mmu_operation_log(address, virtual_page_num, page_offset, size, true);
    if (mmu_write(data, virtual_page_num, page_offset, size) != SUCCESS) {
        log_console(
            LOG_ERROR, __FILE__, __FUNCTION__, __LINE__,
            "Write to address %d (Page: %d, Offset: %d) of size: %d Failed!\n",
            address, virtual_page_num, page_offset, size);
        return FAILURE;
    }
    if (log_mmu_operation() != SUCCESS) {
        log_console(LOG_CRITICAL, __FILE__, __FUNCTION__, __LINE__,
                    "Failed to log operation!\n");
        return FAILURE;
    }
    return SUCCESS;
}

int pemmu_read_data(int address, void *data, int size)
{
    assert(address >= 0);
    int virtual_page_num, page_offset;
    split_virtual_address(address, &virtual_page_num, &page_offset);
    init_mmu_operation_log(address, virtual_page_num, page_offset, size, false);
    if (mmu_read(data, virtual_page_num, page_offset, size) != SUCCESS) {
        log_console(
            LOG_ERROR, __FILE__, __FUNCTION__, __LINE__,
            "Read from address %d (Page: %d, Offset: %d) of size: %d Failed!\n",
            address, virtual_page_num, page_offset, size);
        return FAILURE;
    }
    if (log_mmu_operation() != SUCCESS) {
        log_console(LOG_CRITICAL, __FILE__, __FUNCTION__, __LINE__,
                    "Failed to log operation!\n");
        return FAILURE;
    }
    return SUCCESS;
}

int pemmu_malloc(int type, int number)
{
    int address = mmu_allocate_variable(type, number);
    if (address >= 0) {
        log_address_allocation(address);
    } else {
        log_console(LOG_ERROR, __FILE__, __FUNCTION__, __LINE__,
                    "Failed to allocate memory (type: %d num: %d)\n", type,
                    number);
    }
    return address;
}

int pemmu_free(int address)
{
    int ret_val;
    ret_val = mmu_free_variable(address);
    if (ret_val == SUCCESS) {
        log_address_free(address);
    } else {
        log_console(LOG_ERROR, __FILE__, __FUNCTION__, __LINE__,
                    "Failed to free address: %d\n", address);
    }
    return ret_val;
}
