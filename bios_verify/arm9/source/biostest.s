.code 16
.text
.align 2
.global test_crc
.global test_waitbyloop
.global test_vblankintrwait
.global test_cpuset
.global test_cpufastset
.global test_sqrt
.global test_diff8unfilter
.global test_diff16unfilter
.global test_bitunpack


test_bitunpack:
	swi #0x10
	bx lr

test_diff8unfilter:
	swi #0x16
	bx lr

test_diff16unfilter:
	swi #0x18
	bx lr


test_sqrt:
	swi #0xD
	bx lr


test_cpufastset:
	swi #0xC
	bx lr


test_cpuset:
	swi #0xB
	bx lr

test_vblankintrwait:
	swi #0x5
	bx lr


test_waitbyloop:
	swi #0x3
	bx lr

test_crc:
	push {r1-r2}
	swi #0xE
	pop {r1-r2}
	bx lr

