.code 16
.text
.align 2
.global test_abi
.global sync_ipc

test_abi:
	mov r0, #4
	bx lr

sync_ipc:
	push {r2}
	mov r2, #0
	str r2, [r0]
	
