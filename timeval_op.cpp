/**
 *	Imie: Michal
 * 	Nazwisko: Lech
 *	nr indeksu: 248006
 */

#include <ctime>
#include "timeval_op.h"

/* convert struct timeval to ms(milliseconds) */
double tv2ms(struct timeval *a) {
    return ((a->tv_sec * 1000) + (a->tv_usec / 1000.0));
}

/* return an integer greater than, equal to, or less than 0,
   according as the timeval a is greater than,
   equal to, or less than the timeval b. */
int compare_timeval(struct timeval *a, struct timeval *b) {
    if (a->tv_sec > b->tv_sec)
        return 1;
    else if (a->tv_sec < b->tv_sec)
        return -1;
    else if (a->tv_usec > b->tv_usec)
        return 1;
    else if (a->tv_usec < b->tv_usec)
        return -1;
    return 0;
}

/* Perform timeval subtraction
    - "to - from = result"
    - return -1 if 'from' is larget(later) than 'to'
    - return 0 if success */
int subtract_timeval(struct timeval *to, struct timeval *from, struct timeval *result) {
    if (compare_timeval(to, from) < 0)
        return -1;
    result->tv_sec = to->tv_sec - from->tv_sec;
    result->tv_usec = to->tv_usec - from->tv_usec;

    if(result->tv_usec < 0)    {
        result->tv_sec--;
        result->tv_usec += 1000000;
    }
    return 0;
}
