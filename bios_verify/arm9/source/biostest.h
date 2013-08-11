#ifndef __BIOSTEST_H__
#define	__BIOSTEST_H__

int test_sqrt(unsigned int num);
void test_cpufastset(void *src, void *dst, int wordcount);
void test_cpuset(void *src, void *dst, int wordcount);
short test_crc(int initial_crc, void *addr, int len);
void test_waitbyloop(int wait);
void test_vblankintrwait();

#endif
