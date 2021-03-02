import sys, pytest, subprocess, os, difflib, time
#pytest test_pemmu.py -s : Pytest execution with program output ecoe'd to stdout
#pytest test_pemmu.py -v : Pytest execution in verbose mode
#Usage: ./penn-mmu [test_type] [data_type] [memory_config] [replacement_policy]
testTimeout = 120

testAddressAllocationDataSet = \
[
    #---------- Test 1 :ADDRESS_ALLOCATION_SANITY_TEST -----------------
    pytest.param(1, 1, 1, 1,id='ADDRESS_ALLOCATION_SANITY_BYTE_MEM_A'),
    pytest.param(1, 2, 1, 1,id='ADDRESS_ALLOCATION_SANITY_WORD_MEM_A'),
    pytest.param(1, 3, 1, 1,id='ADDRESS_ALLOCATION_SANITY_DWORD_MEM_A'),
    pytest.param(1, 4, 1, 1,id='ADDRESS_ALLOCATION_SANITY_QWORD_MEM_A'),

    #---------- Test 2 :ADDRESS_ALLOCATION_FUNCTIONALITY_TEST ----------
    pytest.param(2, 1, 1, 1,id='ADDRESS_ALLOCATION_FUNCTIONALITY_BYTE_MEM_A'),
    pytest.param(2, 2, 1, 1,id='ADDRESS_ALLOCATION_FUNCTIONALITY_WORD_MEM_A'),
    pytest.param(2, 3, 1, 1,id='ADDRESS_ALLOCATION_FUNCTIONALITY_DWORD_MEM_A'),
    pytest.param(2, 4, 1, 1,id='ADDRESS_ALLOCATION_FUNCTIONALITY_QWORD_MEM_A'),

    #---------- Test 3 :ADDRESS_ALLOCATION_COMBINE_BLOCKS_TEST ---------
    pytest.param(3, 1, 1, 1,id='ADDRESS_ALLOCATION_COMBINE_BLOCKS_BYTE_MEM_A'),
    pytest.param(3, 2, 1, 1,id='ADDRESS_ALLOCATION_COMBINE_BLOCKS_WORD_MEM_A'),
    pytest.param(3, 3, 1, 1,id='ADDRESS_ALLOCATION_COMBINE_BLOCKS_DWORD_MEM_A'),
    pytest.param(3, 4, 1, 1,id='ADDRESS_ALLOCATION_COMBINE_BLOCKS_QWORD_MEM_A'),
]


testTranslationDataSet = \
[
    #---------- Test 10 :PHYSICAL_MEMORY_FILL_TEST ---------
    pytest.param(10, 1, 1, 1,id='PHYSICAL_MEMORY_FILL_TEST_BYTE_MEM_A'),
    pytest.param(10, 2, 1, 1,id='PHYSICAL_MEMORY_FILL_TEST_WORD_MEM_A'),
    pytest.param(10, 3, 1, 1,id='PHYSICAL_MEMORY_FILL_TEST_DWORD_MEM_A'),
    pytest.param(10, 4, 1, 1,id='PHYSICAL_MEMORY_FILL_TEST_QWORD_MEM_A'),

    #---------- Test 11 :VIRTUAL_MEMORY_FILL_TEST ---------
    pytest.param(11, 1, 1, 1,id='VIRTUAL_MEMORY_FILL_TEST_BYTE_MEM_A_FIFO'),
    pytest.param(11, 2, 1, 1,id='VIRTUAL_MEMORY_FILL_TEST_WORD_MEM_A_FIFO'),
    pytest.param(11, 3, 1, 1,id='VIRTUAL_MEMORY_FILL_TEST_DWORD_MEM_A_FIFO'),
    pytest.param(11, 4, 1, 1,id='VIRTUAL_MEMORY_FILL_TEST_QWORD_MEM_A_FIFO'),
    pytest.param(11, 1, 1, 2,id='VIRTUAL_MEMORY_FILL_TEST_BYTE_MEM_A_CLOCK'),
    pytest.param(11, 2, 1, 2,id='VIRTUAL_MEMORY_FILL_TEST_WORD_MEM_A_CLOCK'),
    pytest.param(11, 3, 1, 2,id='VIRTUAL_MEMORY_FILL_TEST_DWORD_MEM_A_CLOCK'),
    pytest.param(11, 4, 1, 2,id='VIRTUAL_MEMORY_FILL_TEST_QWORD_MEM_A_CLOCK'),
    pytest.param(11, 1, 1, 3,id='VIRTUAL_MEMORY_FILL_TEST_BYTE_MEM_A_LRU'),
    pytest.param(11, 2, 1, 3,id='VIRTUAL_MEMORY_FILL_TEST_WORD_MEM_A_LRU'),
    pytest.param(11, 3, 1, 3,id='VIRTUAL_MEMORY_FILL_TEST_DWORD_MEM_A_LRU'),
    pytest.param(11, 4, 1, 3,id='VIRTUAL_MEMORY_FILL_TEST_QWORD_MEM_A_LRU'),

    #---------- Test 12 :WRITE_READBACK_TEST ---------
    pytest.param(12, 1, 1, 1,id='WRITE_READBACK_TEST_BYTE_MEM_A_FIFO'),
    pytest.param(12, 2, 1, 1,id='WRITE_READBACK_TEST_WORD_MEM_A_FIFO'),
    pytest.param(12, 3, 1, 1,id='WRITE_READBACK_TEST_DWORD_MEM_A_FIFO'),
    pytest.param(12, 4, 1, 1,id='WRITE_READBACK_TEST_QWORD_MEM_A_FIFO'),
    pytest.param(12, 1, 1, 2,id='WRITE_READBACK_TEST_BYTE_MEM_A_CLOCK'),
    pytest.param(12, 2, 1, 2,id='WRITE_READBACK_TEST_WORD_MEM_A_CLOCK'),
    pytest.param(12, 3, 1, 2,id='WRITE_READBACK_TEST_DWORD_MEM_A_CLOCK'),
    pytest.param(12, 4, 1, 2,id='WRITE_READBACK_TEST_QWORD_MEM_A_CLOCK'),
    pytest.param(12, 1, 1, 3,id='WRITE_READBACK_TEST_BYTE_MEM_A_LRU'),
    pytest.param(12, 2, 1, 3,id='WRITE_READBACK_TEST_WORD_MEM_A_LRU'),
    pytest.param(12, 3, 1, 3,id='WRITE_READBACK_TEST_DWORD_MEM_A_LRU'),
    pytest.param(12, 4, 1, 3,id='WRITE_READBACK_TEST_QWORD_MEM_A_LRU'),

    #---------- Test 13 :CLEAN_DIRTY_EVICTION_TEST ---------
    pytest.param(13, 1, 1, 1,id='CLEAN_DIRTY_EVICTION_TEST_BYTE_MEM_A_FIFO'),
    pytest.param(13, 2, 1, 1,id='CLEAN_DIRTY_EVICTION_TEST_WORD_MEM_A_FIFO'),
    pytest.param(13, 3, 1, 1,id='CLEAN_DIRTY_EVICTION_TEST_DWORD_MEM_A_FIFO'),
    pytest.param(13, 4, 1, 1,id='CLEAN_DIRTY_EVICTION_TEST_QWORD_MEM_A_FIFO'),
    pytest.param(13, 1, 1, 2,id='CLEAN_DIRTY_EVICTION_TEST_BYTE_MEM_A_CLOCK'),
    pytest.param(13, 2, 1, 2,id='CLEAN_DIRTY_EVICTION_TEST_WORD_MEM_A_CLOCK'),
    pytest.param(13, 3, 1, 2,id='CLEAN_DIRTY_EVICTION_TEST_DWORD_MEM_A_CLOCK'),
    pytest.param(13, 4, 1, 2,id='CLEAN_DIRTY_EVICTION_TEST_QWORD_MEM_A_CLOCK'),
    pytest.param(13, 1, 1, 3,id='CLEAN_DIRTY_EVICTION_TEST_BYTE_MEM_A_LRU'),
    pytest.param(13, 2, 1, 3,id='CLEAN_DIRTY_EVICTION_TEST_WORD_MEM_A_LRU'),
    pytest.param(13, 3, 1, 3,id='CLEAN_DIRTY_EVICTION_TEST_DWORD_MEM_A_LRU'),
    pytest.param(13, 4, 1, 3,id='CLEAN_DIRTY_EVICTION_TEST_QWORD_MEM_A_LRU'),

    #---------- Test 15 :PATTERN_ACCCESS_TEST_FIFO_TEST ---------
    pytest.param(15, 1, 1, 1,id='PATTERN_ACCCESS_TEST_FIFO_MEM_A'),

    #---------- Test 16 :PATTERN_ACCCESS_TEST_CLOCK_TEST ---------
    pytest.param(16, 1, 1, 2,id='PATTERN_ACCCESS_TEST_CLOCK_MEM_A'),

    #---------- Test 17 :PATTERN_ACCCESS_TEST_LRU_TEST ---------
    pytest.param(17, 1, 1, 3,id='PATTERN_ACCCESS_TEST_LRU_MEM_A'),

    #---------- Test 18 :TLB_SANITY_TEST ---------
    pytest.param(18, 1, 7, 1,id='TLB_SANITY_TEST_FIFO_TLB_MEM_A'),
    pytest.param(18, 1, 7, 2,id='TLB_SANITY_TEST_CLOCK_TLB_MEM_A'),
    pytest.param(18, 1, 7, 3,id='TLB_SANITY_TEST_LRU_TLB_MEM_A'),
]

@pytest.mark.parametrize("test_type, data_type, mem_config, replacement_policy", testAddressAllocationDataSet)
def test_submission_pemmu_address_allocation(test_type, data_type, mem_config, replacement_policy):

    failed_tests = open('Address_Allocation_Failed_Tests.txt',"a+")
    passed_tests = open('Address_Allocation_Passed_Tests.txt',"a+")
    penn_vm_command = './penn-mmu ' + str(test_type) + ' ' + str(data_type) + ' ' + str(mem_config) + ' ' + str(replacement_policy) + '\n'

    process = subprocess.Popen(["./penn-mmu",str(test_type), str(data_type), str(mem_config), str(replacement_policy)], \
        cwd='./', universal_newlines=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    start_time = time.time()
    while process.returncode == None:
        process.poll()
        if (time.time() - start_time > testTimeout):
            process.kill()

    if process.returncode < 0 :
        failed_tests.write("TIMED OUT : " + penn_vm_command)
        pytest.fail()
    elif process.returncode != 0 :
        failed_tests.write("FAILED : " + penn_vm_command)
        out , err = process.communicate()
        print(out)
        print(err)
        pytest.fail()
    else:
        passed_tests.write(penn_vm_command)

@pytest.mark.parametrize("test_type, data_type, mem_config, replacement_policy", testTranslationDataSet)
def test_submission_pemmu_translation(test_type, data_type, mem_config, replacement_policy):

    failed_tests = open('Translation_Failed_Tests.txt',"a+")
    passed_tests = open('Translation_Passed_Tests.txt',"a+")
    penn_vm_command = './penn-mmu ' + str(test_type) + ' ' + str(data_type) + ' ' + str(mem_config) + ' ' + str(replacement_policy) + '\n'

    process = subprocess.Popen(["./penn-mmu",str(test_type), str(data_type), str(mem_config), str(replacement_policy)], \
        cwd='./', universal_newlines=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    start_time = time.time()
    while process.returncode == None:
        process.poll()
        if (time.time() - start_time > testTimeout):
            process.kill()

    if process.returncode < 0 :
        failed_tests.write("TIMED OUT : " + penn_vm_command)
        pytest.fail()
    elif process.returncode != 0 :
        failed_tests.write("FAILED : " + penn_vm_command)
        out , err = process.communicate()
        print(out)
        print(err)
        pytest.fail()
    else:
        passed_tests.write(penn_vm_command)
