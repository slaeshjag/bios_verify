#include <nds.h>

#include "ipc.h"

int main() {
	volatile void **blah = (void *) 0x27fff00;
	struct ipc_ptr *ipcptr;

	irqInit();
	*blah = 0;
	while (! ((volatile) *blah));
	ipcptr = *blah;
	ipcptr->init = 1;
	

	// Keep the ARM7 mostly idle
	while (1) {
		if ( 0 == (REG_KEYINPUT & (KEY_SELECT | KEY_START | KEY_L | KEY_R))) {
			/* DO SOMETHING */
		}
		
		/* Magic memory location to report test result at */
	}
	return 0;
}
