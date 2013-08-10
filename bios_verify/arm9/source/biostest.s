.code 16
.text
.align 2
.global test_crc

test_crc:
	push {r1-r2}
	swi #0xE
	pop {r1-r2}
	bx lr

