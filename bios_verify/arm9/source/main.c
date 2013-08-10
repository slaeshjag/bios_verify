/*---------------------------------------------------------------------------------

	demo of ansi escape sequences

---------------------------------------------------------------------------------*/
#include <nds.h>
#include <stdio.h>

#include "biostest.h"
#include "crcdata.h"
#include "ipc.h"


int testcpuset() {
	char *src = malloc(4096), *dst;
	const char *error;
	unsigned int *iptr;
	int i;
	dst = src;

	iprintf("CpuSet test............ ");
	*src = 0;
	test_cpuset(src, dst, (512 | 0x6000000));
	for (i = 0; i < 2048; i++)
		if (dst[i]) {
			error = "32-bit fill FAILED\n";
			goto errorh;
		}
	test_cpuset(dst, src, (1024 | 0x2000000));
	for (i = 0; i < 2048; i++)
		if (dst[i]) {
			error = "16-bit fill FAILED\n";
			goto errorh;
		}
	iptr = (void *) dst;
	for (i = 0; i < 512; i++)
		iptr[i] = i | (i << 24);
	test_cpuset(dst, src, (512 | 0x4000000));
	iptr = (void *) src;
	for (i = 0; i < 512; i++)
		if (iptr[i] != (i + (i << 24))) {
			error = "32-bit copy FAILED\n";
			goto errorh;
		}
	for (i = 0; i < 512; i++)
		iptr[i] = (512 - i) + ((512 - i) << 24);
	test_cpuset(src, dst, 1024);
	iptr = (void *) dst;
	for (i = 0; i < 512; i++) 
		if (iptr[i] != ((512 - i) + ((512 - i) << 24))) {
			error = "16-bit copy FAILED\n";
			goto errorh;
		}
	iprintf("PASS\n");
	return 1;

	errorh:
	iprintf("FAIL %i\n", i);
	iprintf(error);
	return 0;
}
	
	



int testwaitbyloop() {
	volatile unsigned short *timer = (void *) 0x4000100;
	volatile unsigned short *setting = (void *) 0x4000102;
	unsigned short t, n;
	iprintf("WaitByLoop test........ ");

	*setting = 0x81;
	while (*timer > 20);
	n = *timer;
	test_waitbyloop(65536);
	t = *timer;

	if (t - n < 1023) {
		iprintf("FAIL\n");
		iprintf("d/t = %i, > 1024 expected\n", t - n);
		return 0;
	}
	
	*setting = 0;

	iprintf("PASS\n");

	return 1;
}


int testvblankwait() {
	volatile unsigned short *timer = (void *) 0x4000100;
	volatile unsigned short *setting = (void *) 0x4000102;
	short t, n;
	iprintf("VBlankWait test........ ");
	
	test_vblankintrwait();
	*setting = 0x81;
	t = *timer;
	test_vblankintrwait();
	n = *timer;

	if (6000 > (unsigned short) (n - t) || 10000 < (unsigned short) (n - t)) {
		iprintf("FAIL\n");
		iprintf("d/t = %i,  6000 < i < 10000 expected\n", (unsigned) n - t);
		return 0;
	}

	*setting = 0;
	iprintf("PASS\n");

	return 1;
}


int testcrc() {
	int i, res;

	res = 0xFFFF;
	iprintf("CRC16 Test............. ");
	for (i = 0; crc_data[i]; i++) {
		res = (test_crc(res, crc_data[i], (strlen(crc_data[i]) & 0xFFFFE)));
		if (res != crc_facit[i]) {
			iprintf("FAIL\nExpected 0x%X, got 0x%X\n", crc_facit[i], res);
			iprintf("Fail on CRC entry %i\n", i);
			return 0;
		}
	}
	
	iprintf("PASS\n");
	return 1;
}


void waitForARM7() {
	#if 0
	int i;
	while (!(*((volatile unsigned int *) 0x2300000)));
	iprintf("\n0x%X\n", (*((volatile unsigned int *) 0x2300000)));
	iprintf("\n0x%X\n", (unsigned int *) malloc(65536));
	#endif

	return;
}


//---------------------------------------------------------------------------------
int main(void) {
//---------------------------------------------------------------------------------
	consoleDemoInit();
	
	if (!testcrc());
	else if (!testwaitbyloop());
	else if (!testvblankwait());
	else if (!testcpuset());
	else {
		iprintf("All tests passed.\n");
		for(;;);
	}

	
	iprintf("A test has failed. HALTED");

	while(1) {
//		swiWaitForVBlank();
	}

	return 0;
}
