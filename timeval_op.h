#pragma once

/** convert struct timeval to ms(milliseconds) */
double tv2ms(struct timeval *a);

/** return an integer greater than, equal to, or less than 0,
   according as the timeval a is greater than,
   equal to, or less than the timeval b.
   */
int compare_timeval(struct timeval *a, struct timeval *b);

/** Perform timeval subtraction
    - "to - from = result"
    - return -1 if 'from' is larget(later) than 'to'
    - return 0 if success
    */
int subtract_timeval(struct timeval *to, struct timeval *from, struct timeval *result);
