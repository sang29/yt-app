#ifndef TEST_ALLOCATION_SUBROUTINES_H
#define TEST_ALLOCATION_SUBROUTINES_H

#define MAX_STRESS_TEST_ADDRESSES     2000
#define MAX_STRESS_TEST_LOOPS         4
#define MAX_SINGLE_ADDRESS_ALLOCATION 5000

int test_address_allocation_user_defined(int data_type,
                                         int memory_configuration);
int test_address_allocation_sanity(int data_type, int memory_configuration);
int test_address_allocation_functionality(int data_type,
                                          int memory_configuration);
int test_address_allocation_combine_blocks(int data_type,
                                           int memory_configuration);
int test_address_allocation_offset_access(int data_type,
                                          int memory_configuration);

#endif
