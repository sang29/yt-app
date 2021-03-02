#ifndef TEST_TRANSLATION_SUBROUTINES_H
#define TEST_TRANSLATION_SUBROUTINES_H

int test_translation_user_defined(int data_type, int memory_configuration,
                                  int replacement_policy);
int test_fill_virtual_memory(int data_type, int memory_configuration,
                             int replacemt_policy);
int test_fill_physical_memory(int data_type, int memory_configuration);
int test_write_readback(int data_type, int memory_configuration,
                        int replacement_policy);
int test_clean_dirty_eviction(int data_type, int memory_configuration,
                              int replacement_policy);
int test_data_access_pattern_fifo(int memory_configuration);
int test_data_access_pattern_clock(int memory_configuration);
int test_data_access_pattern_lru(int memory_configuration);
int test_tlb_sanity(int memory_configuration, int replacement_policy);

#endif
