/**
 * DO NOT CHANGE - you will not be submitting this file
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include "interface.h"
#include "../tests/test_framework.h"

static const char *const data_type_c[5] = {
    [UNKNOWN_TYPE] = "Unknown Datatype", [BYTE_TYPE] = "BYTE (1 byte)",
    [WORD_TYPE] = "WORD (2 bytes)",      [DWORD_TYPE] = "DWORD (4 bytes)",
    [QWORD_TYPE] = "QWORD (8 bytes)",
};

static void SIGALRMHandler(int signo)
{
    if (signo != SIGALRM) {
        return;
    }
    printf("ERROR: Test timed out\n");
    exit(-1);
}

int main(int argc, char *argv[])
{
    int test_type = -1;
    int data_type = -1;
    int mem_cfg = -1;
    int policy = -1;
    int run_time = -1;
    int ret = -1;
    printf("-- [PEMMU] - Virtual Memory Simulator --\n");
    if (argc >= 5) {
        test_type = (int) strtol(argv[1], NULL, 10);
        data_type = (int) strtol(argv[2], NULL, 10);
        mem_cfg = (int) strtol(argv[3], NULL, 10);
        policy = (int) strtol(argv[4], NULL, 10);
        if (argc == 6) {
            run_time = (int) strtol(argv[5], NULL, 10);
            printf("Max Runtime Allocated : %d seconds\n", run_time);
        }
        printf("Test Type : %s\n", test_type_c[test_type]);
        printf("Data Type : %s\n", data_type_c[data_type]);
        printf("Memory Configuration : %s\n", memory_config_type_c[mem_cfg]);
        printf("Replacement Policy : %s\n", replacement_policy_type_c[policy]);
    } else if (argc == 2) {
        test_type = RUN_ANALYSIS_PROGRAMS;
        printf("Test Type : %s\n", test_type_c[test_type]);
    } else {
        printf("ERROR : main() Invalid Arguments. Usage: [binary] [test_type] "
               "[data_type] [memory_config] [replacement_policy] [runtime]\n");
        return -1;
    }

    if (run_time > 0) {
        if (signal(SIGALRM, SIGALRMHandler) == SIG_ERR) {
            printf("ERROR: Error Installing signal handler\n");
        }
        alarm(run_time);
    }

    const clockid_t clockid = CLOCK_TAI;
    struct timespec start, end;

    if (clock_gettime(clockid, &start) < 0) {
        perror("clock_gettime");
        exit(EXIT_FAILURE);
    }

    //Run the test framework
    ret = run_cutest_framework(test_type, data_type, mem_cfg, policy);
    if (ret != 0) {
        fprintf(stderr, "run_cutest_framework failed!\n");
    }

    if (clock_gettime(clockid, &end) < 0) {
        perror("clock_gettime");
        exit(EXIT_FAILURE);
    }

    alarm(0);

    time_t sec = end.tv_sec - start.tv_sec;
    int msec = (end.tv_nsec - start.tv_nsec) / (1000 * 1000);
    if (msec < 0) {
        sec--;
        msec += 1000;
    }

    printf("Total Execution Time: %jd.%03d seconds\n\n", sec, msec);

    return ret;
}
