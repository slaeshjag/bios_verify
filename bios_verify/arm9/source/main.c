/*---------------------------------------------------------------------------------

	demo of ansi escape sequences

---------------------------------------------------------------------------------*/
#include <nds.h>
#include <stdio.h>

#include "biostest.h"
#include "crcdata.h"

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


//---------------------------------------------------------------------------------
int main(void) {
//---------------------------------------------------------------------------------
	consoleDemoInit();
	if (!testcrc());
	//else if (next_test());
	else {
		iprintf("All tests passed.");
		for(;;);
	}
	
	iprintf("A test has failed. HALTED");

	while(1) {
//		swiWaitForVBlank();
	}

	return 0;
}
