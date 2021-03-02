/**
 * DO NOT CHANGE - you will not be submitting this file
 *
 * Logging utility: to be used to ensure accurate grading
 */

#include "logger.h"

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "interface.h"

typedef struct {
    bool is_write;
    bool is_page_table_access;
    bool is_page_fault;
    bool is_eviction;
    bool is_diskwrite;
    bool is_tlb_hit;
    bool is_tlb_miss;
    bool is_tlb_add;
    int address;
    int virtual_page_num;
    int page_offset;
    int size;
    int page_fault_virtual_page_no;
    int eviction_virtual_page;
    int eviction_physical_page;
    int translate_virtual_page_no;
    int translate_physical_page_no;
    int disk_write_physical_page_num;
    int tlb_hit_virtual_page_no;
    int tlb_hit_physical_page_no;
    int tlb_add_virtual_page_no;
    int tlb_add_physical_page_no;
    int tlb_miss_virtual_page_num;
} logger_t;

typedef struct {
    int disk_writes;
    int evictions;
    int page_faults;
    int tlb_miss;
    int tlb_hit;
} perf_statistics_t;

static FILE *logfile;
static FILE *perf_log;
static perf_statistics_t perf_stats;
static logger_t mmu_oprtaion_log;

/* NEED NOT BE USED IN SUBMISSION FILES */
static int log_integrity_check(void)
{
    if (mmu_oprtaion_log.is_eviction && mmu_oprtaion_log.is_diskwrite) {
        if (mmu_oprtaion_log.eviction_physical_page !=
            mmu_oprtaion_log.disk_write_physical_page_num) {
            log_console(
                LOG_CRITICAL, __FILE__, __FUNCTION__, __LINE__,
                "Eviction Page No %d not the same as Disk Write Page No %d\n",
                mmu_oprtaion_log.eviction_physical_page,
                mmu_oprtaion_log.disk_write_physical_page_num);
            return FAILURE;
        }
    }
    if (mmu_oprtaion_log.is_page_fault) {
        if (mmu_oprtaion_log.page_fault_virtual_page_no !=
            mmu_oprtaion_log.translate_virtual_page_no) {
            log_console(LOG_CRITICAL, __FILE__, __FUNCTION__, __LINE__,
                        "Page Fault VP %d not the same as Translate VP %d\n",
                        mmu_oprtaion_log.page_fault_virtual_page_no,
                        mmu_oprtaion_log.translate_virtual_page_no);
            return FAILURE;
        }
    }
    if (mmu_oprtaion_log.is_tlb_hit && mmu_oprtaion_log.is_tlb_miss) {
        log_console(LOG_CRITICAL, __FILE__, __FUNCTION__, __LINE__,
                    "Detected TLB HIT and TLB MISS in the same operation\n");
        return FAILURE;
    }
    if (mmu_oprtaion_log.is_tlb_hit && mmu_oprtaion_log.is_page_fault) {
        log_console(LOG_CRITICAL, __FILE__, __FUNCTION__, __LINE__,
                    "Detected TLB HIT and PAGE_FAULT in the same operation\n");
        return FAILURE;
    }
    if (mmu_oprtaion_log.is_tlb_miss && !mmu_oprtaion_log.is_tlb_add) {
        log_console(
            LOG_CRITICAL, __FILE__, __FUNCTION__, __LINE__,
            "Detected TLB MISS without a TLB ADD in the same operation\n");
        return FAILURE;
    }
    if (!mmu_oprtaion_log.is_tlb_miss && mmu_oprtaion_log.is_tlb_add) {
        log_console(
            LOG_CRITICAL, __FILE__, __FUNCTION__, __LINE__,
            "Detected TLB ADD without a TLB MISS in the same operation\n");
        return FAILURE;
    }

    return SUCCESS;
}

void log_page_fault(int virtual_page_num)
{
    //fprintf(logfile, "[PAGE_FAULT]:\t\t[Virtual Page: %d]\n", virtual_page_num);
    mmu_oprtaion_log.is_page_fault = true;
    mmu_oprtaion_log.page_fault_virtual_page_no = virtual_page_num;
    perf_stats.page_faults++;
}

void log_translate(int virtual_page_num, int physical_page_num)
{
    //fprintf(logfile, "[TRANSLATE]:\t\t[Virtual Page: %d]\t[Physical Frame: %d]\n", virtual_page_num, physical_page_num);
    mmu_oprtaion_log.is_page_table_access = true;
    mmu_oprtaion_log.translate_physical_page_no = physical_page_num;
    mmu_oprtaion_log.translate_virtual_page_no = virtual_page_num;
}

void log_eviction(int physical_page_num, int virtual_page_num)
{
    //fprintf(logfile, "[EVICTION]:\t\t\t[Virtual Page: %d]\t[Physical Frame: %d]\n", virtual_page_num, physical_page_num);
    mmu_oprtaion_log.is_eviction = true;
    mmu_oprtaion_log.eviction_physical_page = physical_page_num;
    mmu_oprtaion_log.eviction_virtual_page = virtual_page_num;
    perf_stats.evictions++;
}

void log_disk_write(int physical_page_num)
{
    //fprintf(logfile, "[DISK_WRITE]:\t\t[Physical Page: %d]\n", physical_page_num);
    mmu_oprtaion_log.is_diskwrite = true;
    mmu_oprtaion_log.disk_write_physical_page_num = physical_page_num;
    perf_stats.disk_writes++;
}

void log_add_TLB_entry(int virtual_page_num, int physical_frame_num)
{
    //fprintf(logfile, "[TLB_ADD]:\t\t\t[TLB ENTRY: %d]\t[Virtual Page: %d]\t[Physical Frame: %d]\n", tlb_entry_idx, virtual_page_num, physical_frame_num);
    mmu_oprtaion_log.is_tlb_add = true;
    mmu_oprtaion_log.tlb_add_virtual_page_no = virtual_page_num;
    mmu_oprtaion_log.tlb_add_physical_page_no = physical_frame_num;
}

void log_TLB_hit(int virtual_page_num, int physical_frame_num)
{
    //fprintf(logfile, "[TLB HIT]:\t\t\t[Virtual Page: %d]\t[Physical Frame: %d]\n", virtual_page_num, physical_frame_num);
    mmu_oprtaion_log.is_tlb_hit = true;
    mmu_oprtaion_log.tlb_hit_virtual_page_no = virtual_page_num;
    mmu_oprtaion_log.tlb_hit_physical_page_no = physical_frame_num;
    perf_stats.tlb_hit++;
}

void log_TLB_miss(int virtual_page_num)
{
    //fprintf(logfile, "[TLB MISS]:\t\t\t[Virtual Page: %d]\n", virtual_page_num);
    mmu_oprtaion_log.is_tlb_miss = true;
    mmu_oprtaion_log.tlb_miss_virtual_page_num = virtual_page_num;
    perf_stats.tlb_miss++;
}

void log_address_allocation(int address)
{
    fprintf(logfile, "[ADDRESS ALLOC]:\t[Address: %d]\n", address);
}

void log_address_free(int address)
{
    fprintf(logfile, "[ADDRESS FREE ]:\t[Address: %d]\n", address);
}

/* NEED NOT BE USED IN SUBMISSION FILES */
int create_log_file(char *fname)
{
    char file_path[MAX_FILE_PATH_LEN];

    //Create the log file
    memset(&file_path, 0, MAX_FILE_PATH_LEN);
    strcpy(file_path, "logs/");
    strcat(file_path, fname);
    logfile = fopen(file_path, "w+");
    if (logfile == NULL) {
        log_console(LOG_CRITICAL, __FILE__, __FUNCTION__, __LINE__,
                    "Failed to create log file: %s\n", file_path);
        return FAILURE;
    }
    fprintf(logfile, "Test Name: %s\n", fname);

    //Create the perf counter file
    memset(&file_path, 0, MAX_FILE_PATH_LEN);
    strcpy(file_path, "logs/perf/");
    strcat(file_path, fname);
    strcat(file_path, "_perf");
    perf_log = fopen(file_path, "w+");
    if (perf_log == NULL) {
        log_console(LOG_CRITICAL, __FILE__, __FUNCTION__, __LINE__,
                    "Failed to create log file: %s\n", file_path);
        return FAILURE;
    }
    fprintf(perf_log, "Test Name: %s\n", fname);

    //Initialize performance counters
    perf_stats.disk_writes = 0;
    perf_stats.evictions = 0;
    perf_stats.page_faults = 0;
    perf_stats.tlb_hit = 0;
    perf_stats.tlb_miss = 0;
    return SUCCESS;
}

/* NEED NOT BE USED IN SUBMISSION FILES */
void close_log(void)
{
    if (logfile != NULL) {
        fclose(logfile);
        logfile = NULL;
    }

    //Output the perf statictics and close the file
    if (perf_log != NULL) {
        fprintf(perf_log, "******** PERFORMANCE STATS START ********\n");
        fprintf(perf_log, "Total Disk Writes: %d\n", perf_stats.disk_writes);
        fprintf(perf_log, "Total Evictions: %d\n", perf_stats.evictions);
        fprintf(perf_log, "Total Page Faults: %d\n", perf_stats.page_faults);
        fprintf(perf_log, "Total TLB Hits: %d\n", perf_stats.tlb_hit);
        fprintf(perf_log, "Total TLB Misses: %d\n", perf_stats.tlb_miss);
        fprintf(perf_log, "******** PERFORMANCE STATS END ********\n");
        fclose(perf_log);
        perf_log = NULL;
    }
}

/* NEED NOT BE USED IN SUBMISSION FILES */
void log_config(void)
{
    fprintf(logfile, "*** MEMORY CONFIGURATION START ***\n");
    fprintf(logfile, "Data Size: %d\n", mem_config.data_size);
    fprintf(logfile, "Page Size: %d\n", mem_config.page_size);
    fprintf(logfile, "Physical Memory Size: %d\n",
            mem_config.physical_memory_size);
    fprintf(logfile, "Virtual Memory Size: %d\n",
            mem_config.virtual_memory_size);
    fprintf(logfile, "Swap Memory Size: %d\n", mem_config.swap_memory_size);
    fprintf(logfile, "*** MEMORY CONFIGURATION END ***\n");
}

/* NEED NOT BE USED IN SUBMISSION FILES */
void log_replacement_policy(const char *policy)
{
    fprintf(logfile, "Replacement Policy set to: %s\n", policy);
}

/* NEED NOT BE USED IN SUBMISSION FILES */
void log_test_framework(char *message)
{
    fprintf(logfile, "[Test Framework] : %s\n", message);
}

/* NEED NOT BE USED IN SUBMISSION FILES */
void init_mmu_operation_log(int address, int virtual_page_num, int page_offset,
                            int size, bool is_write)
{
    memset(&mmu_oprtaion_log, 0, sizeof(logger_t));
    mmu_oprtaion_log.address = address;
    mmu_oprtaion_log.virtual_page_num = virtual_page_num;
    mmu_oprtaion_log.page_offset = page_offset;
    mmu_oprtaion_log.size = size;
    mmu_oprtaion_log.is_write = is_write;
}

/* NEED NOT BE USED IN SUBMISSION FILES */
int log_mmu_operation(void)
{
    char log_output[500];

    if (log_integrity_check() != SUCCESS) {
        log_console(LOG_CRITICAL, __FILE__, __FUNCTION__, __LINE__,
                    "Log Integrity Check Failed\n");
        return FAILURE;
    }
    memset(&log_output, 0, 500);

    if (mmu_oprtaion_log.is_write) {
        strcat(log_output, "[WRITE] : ");
    } else {
        strcat(log_output, "[READ]  : ");
    }

    char *log_end = strchr(log_output, '\0');
    sprintf(log_end, " [Addr: %d (%d,%d) | Size: %d] ",
            mmu_oprtaion_log.address, mmu_oprtaion_log.virtual_page_num,
            mmu_oprtaion_log.page_offset, mmu_oprtaion_log.size);

    if (mmu_oprtaion_log.is_tlb_hit) {
        log_end = strchr(log_end, '\0');
        sprintf(log_end, " [TLB_HIT - VP:%d | PP:%d] ",
                mmu_oprtaion_log.tlb_hit_virtual_page_no,
                mmu_oprtaion_log.tlb_hit_physical_page_no);
    }
    if (mmu_oprtaion_log.is_tlb_miss) {
        strcat(log_end, " [TLB_MISS] ");
    }
    if (mmu_oprtaion_log.is_page_fault) {
        strcat(log_end, " [PG_FLT] ");
    }
    if (mmu_oprtaion_log.is_eviction) {
        log_end = strchr(log_end, '\0');
        sprintf(log_end, " [EVICT - VP:%d | PP:%d] ",
                mmu_oprtaion_log.eviction_virtual_page,
                mmu_oprtaion_log.eviction_physical_page);
    }
    if (mmu_oprtaion_log.is_diskwrite) {
        strcat(log_end, " [DSK_WRT] ");
    }
    if (mmu_oprtaion_log.is_page_table_access) {
        log_end = strchr(log_end, '\0');
        sprintf(log_end, " [TRANS - VP:%d | PP:%d] ",
                mmu_oprtaion_log.translate_virtual_page_no,
                mmu_oprtaion_log.translate_physical_page_no);
    }

    if (mmu_oprtaion_log.is_tlb_add) {
        log_end = strchr(log_end, '\0');
        sprintf(log_end, " [TLB_ADD - VP:%d] ",
                mmu_oprtaion_log.tlb_add_virtual_page_no);
    }

    fprintf(logfile, "%s\n", log_output);
    return SUCCESS;
}
