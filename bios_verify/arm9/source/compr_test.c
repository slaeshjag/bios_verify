#include "data/compression_test_data.h"
#include "biostest.h"
#include <nds.h>
#include <stdio.h>


int verifydata(volatile unsigned int *data, int *error) {
	unsigned int d, i;

	d = 0x11111111;
	for (i = 0; i < 32; i++)
		if (data[i] != (d * (i & 0xF))) {
			*error = i;
			return 0;
		}
	return 1;
}


int testdiff8unfilter() {
	unsigned int *buff;
	int error;
	buff = malloc(128);

	error = 5;	/* Never appears in decompressed data */
	test_cpufastset(&error, buff, 32 | 0x1000000);
	iprintf("Diff8UnFilter test..... ");
	test_diff8unfilter(diff8_data, buff);
	if (!verifydata(buff, &error)) {
		iprintf("FAIL\n");
		iprintf("Bad data at i=%i (0x%X)\n", error, buff[error]);
		free(buff);
		return 0;
	}

	free(buff);
	iprintf("PASS\n");
	return 1;
}


int testdiff16unfilter() {
	unsigned int *buff;
	int error;
	buff = malloc(128);
	error = 5;	/* Never appears in decompressed data */
	test_cpufastset(&error, buff, 32 | 0x1000000);
	
	iprintf("Diff16UnFilter test.... ");
	test_diff16unfilter(diff16_data, buff);
	if (!verifydata(buff, &error)) {
		iprintf("FAIL\n");
		iprintf("Bad data at i=%i (0x%X)\n", error, buff[error]);
		free(buff);
		return 0;
	}

	free(buff);
	iprintf("PASS\n");
	return 1;
}


int testdecompression() {
	if (!testdiff8unfilter());
	else if (!testdiff16unfilter());
	else 
		return 1;
	return 0;
}
