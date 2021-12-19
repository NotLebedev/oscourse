#ifndef _INC_RANDOM_H_
#define _INC_RANDOM_H_

#define RAND_MAX 0x7FFFFFFF

extern "C" int rand(void);
extern "C" void srand(unsigned int seed);
extern "C" void rand_init();
extern "C" unsigned char _dev_urandom[];
extern "C" unsigned int _dev_urandom_len;

#endif
