/* Copyright 1998-2017 IAR Systems AB. */


/*
 * This is the default implementation of the "clock" function of the
 * standard library.  It can be replaced with a system-specific
 * implementation.
 *
 * The "clock" function should return the processor time used by the
 * program from some implementation-defined start time.  The value
 * should be such that if divided by the macro CLOCKS_PER_SEC the
 * result should yield the time in seconds.
 *
 * The value "(clock_t)-1" means that the processor time is not
 * available.
 *
 */

#include <time.h>
extern clock_t clk_count;
extern time_t time_dat, time_dat_copy;

#pragma module_name = "?clock"

clock_t (clock)(void)
{
  return (clk_count);
}
