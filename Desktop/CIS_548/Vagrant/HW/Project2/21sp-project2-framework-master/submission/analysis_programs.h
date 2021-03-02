#ifndef ANALYSIS_PROGRAMS_H
#define ANALYSIS_PROGRAMS_H

/**
 * Do not change the signature of any of the following functions
 */

/**
 * Test program that performs better for fifo than it does for clock
 * @return SUCCESS or FAILURE
 */
int fifo_over_clock(void);

/**
 * Test program that performs better for clock than it does for fifo
 * @return SUCCESS or FAILURE
 */
int clock_over_fifo(void);

/**
 * Test program that performs better for lru than it does for clock
 * @return SUCCESS or FAILURE
 */
int lru_over_clock(void);

/**
 * Test program that performs better for clock than it does for lru
 * @return SUCCESS or FAILURE
 */
int clock_over_lru(void);

/**
 * Test program that performs better for fifo than it does for lru
 * @return SUCCESS or FAILURE
 */
int fifo_over_lru(void);

/**
 * Test program that performs better for lru than it does for fifo
 * @return SUCCESS or FAILURE
 */
int lru_over_fifo(void);

#endif
