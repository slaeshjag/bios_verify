#ifndef __BIOSTEST_H__
#define	__BIOSTEST_H__

void test_diff8unfilter(void *src, void *dest);
void test_diff16unfilter(void *src, void *dest);

int test_sqrt(unsigned int num);
void test_cpufastset(void *src, void *dst, int wordcount);
void test_cpuset(void *src, void *dst, int wordcount);
short test_crc(int initial_crc, void *addr, int len);
void test_waitbyloop(int wait);
void test_vblankintrwait();

#endif
