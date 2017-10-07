#ifndef DEF_H_
#define DEF_H_

#include <stdio.h>
#include <stdlib.h>

#define ERR_EXIT(m) \
do { \
  perror(m); \
    exit(EXIT_FAILURE); \
} while(0)


const unsigned int kMaxArgvNum = 10;
const unsigned int kMaxCmdNum = 10; 
const unsigned int kMaxInputCmdLength = 1024;
const unsigned int kMaxFileNameLength = 64; 
const unsigned int kMaxPipeline = 10;

#endif // DEF_H_
