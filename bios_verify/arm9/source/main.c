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
	dst = src + 2048;

	iprintf("CpuSet test............ ");
	for (i = 0; i < 2048; i++)
		dst[i] = src[i] = 1;
	iptr = (void *) src;
	*iptr = 0;
	test_cpuset(src, dst, (512 | 0x5000000));
	for (i = 0; i < 2048; i++)
		if (dst[i]) {
			error = "32-bit fill FAILED\n";
			goto errorh;
		}
	test_cpuset(dst, src, (1024 | 0x1000000));
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
	free(src);
	return 1;

	errorh:
	free(src);
	iprintf("FAIL\n");
	iprintf(error);
	iprintf("32-bit element #%id failed\n", i);
	return 0;
}
	
	
int testcpufastset() {
	char *src = malloc(4112), *dst;
	const char *error;
	unsigned int *iptr;
	int i;
	dst = src + 2056;

	iprintf("CpuFastSet test........ ");
	test_vblankintrwait();
	iptr = (void *) dst;
	for (i = 0; i < 513; i++)
		iptr[i] = 1;
	iptr = (void *) src;
	*iptr = 0;
	iptr = (void *) dst;
	test_cpufastset(src, dst, 513 | 0x1000000);
	for (i = 0; i < 513; i++)
		if (iptr[i]) {
			error = "Fill FAILED\n";
			goto errorh;
		}
	iptr[513] = 1;
	iptr = (void *) src;
	iptr[513] = 0;
	for (i = 0; i < 513; i++)
		iptr[i] = ((i << 24) + 23 * i + (i >> 4) + 0x10000005);
	test_cpufastset(src, dst, 514);
	iptr = (void *) dst;
	for (i = 0; i < 513; i++)
		if (iptr[i] != ((i << 24) + 23 * i + (i >> 4) + 0x10000005)) {
			error = "Copy FAILED\n";
			goto errorh;
		}
	iptr = (void *) src;
	if (iptr[513] == iptr[1027]) {
		error = "Overcopying detected\n";
		goto errorh;
	}
	iprintf("PASS\n");
	return 1;

	errorh:

	iprintf("FAIL\n");
	iprintf("Failed at element #%id 0x%X\n", i, iptr[i]);
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
	} else if (t - n > 5120) {
		iprintf("FAIL\n");
		iprintf("d/t = %i, < 5120 expected\n", t - n);
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
	else if (!testcpufastset());
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
