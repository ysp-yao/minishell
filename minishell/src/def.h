#ifndef DEF_H_
#define DEF_H_

#include <stdio.h>
#include <stdlib.h>

#define ERR_EXIT(m) \
do { \
  perror(m); \
    exit(EXIT_FAILURE); \
} while(0)





#endif // DEF_H_
