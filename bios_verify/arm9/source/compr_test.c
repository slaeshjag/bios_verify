#include "data/compression_test_data.h"
#include "bitunpack.h"
#include "biostest.h"
#include <nds.h>
#include <stdio.h>


short getcrc(void *addr, int len) {
	return test_crc(0xFFFF, addr, len);
}


static void clearwords(int c, void *dst, int words) {
	test_cpufastset(&c, dst, words | 0x1000000);
	return;
}


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

	error = 0;
	clearwords(5, buff, 32);
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
	error = 0;	/* Never appears in decompressed data */
	clearwords(5, buff, 32);
	
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


int testbitunpack() {
	char src[256], dst[256];
	int i, j, k, l;

	struct {
		unsigned int	src_len		: 16;
		unsigned int	src_bits	: 8;
		unsigned int	dst_bits	: 8;
		unsigned int	data_offset	: 31;
		unsigned int	zero_flag	: 1;
	} bitinfo;
	
	iprintf("BitUnPack test......... ");
	
	for (i = 0; i < 256; i++)
		src[i] = ((i * 4 + (i * 3) % 41 + i * 96 + 5) & 0xFF);
	
	for (j = l = 0; j < 4; j++) 
		for (i = j; i < 6; i++) 
			for (k = 0; k < 2; k++, l++) {
				clearwords(0xFF, dst, 64);
				bitinfo.src_bits = (1 << j);
				bitinfo.dst_bits = (1 << i);
				bitinfo.src_len = 256 / (bitinfo.dst_bits / bitinfo.src_bits);
				bitinfo.data_offset = (j * i + k) & (~0 >> (32 - bitinfo.dst_bits));
				bitinfo.zero_flag = k;
				test_bitunpack(src, dst, &bitinfo);
				if (getcrc(dst, 256) != bitunpack_facit[l]) {
					iprintf("FAIL\n");
					if (i == j && k == 0)
						for (i = 0; i < 256; i++)
							if (src[i] != dst[i] + (dst[i] ? bitinfo.data_offset : 0)) {
								iprintf("byte %i, 0x%X!=0x%X\n", i, src[i], dst[i]);
								break;
							}
					iprintf("FAIL w/ sb=%i,db=%i,sl=%i\nzf=%i,do=%i,n=%i\ncrc=0x%X\n", bitinfo.src_bits, bitinfo.dst_bits, bitinfo.src_len, bitinfo.data_offset, bitinfo.zero_flag, l, getcrc(dst, 256));
					return 0;
				}
			}
	iprintf("PASS\n");
	return 1;
}


int testdecompression() {
	if (!testbitunpack());
	else if (!testdiff8unfilter());
	else if (!testdiff16unfilter());
	else 
		return 1;
	return 0;
}
