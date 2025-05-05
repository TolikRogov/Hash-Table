;==============================================================================
;					My crc32 05.05.2025 @Rogov Anatoliy
;==============================================================================

global crc32Asm										;make global for linker

section .rodata
CRC32_INIT_CRC  equ 0xFFFFFFFF

section .text										;start of code segment

;==============================================================================
;	Give string hash
;	Entry:		RDI - address of bytes string
;				RSI - size of string in bytes
;	Exit:		AX - string hash
;	Destroy:	None
;==============================================================================
crc32Asm:
	pop r10											;returning address

	push rbp
	push rbx
	push r12
	push r13										;saved registers
	push r14
	push r15

	add rsi, rdi
	mov rax, CRC32_INIT_CRC
	crc_loop:
		add rdi, 0x1
		crc32 rax, byte [rdi - 0x1]
		cmp rdi, rsi
	jne crc_loop
	not rax

	pop r15
	pop r14
	pop r13
	pop r12											;return saved registers
	pop rbx
	pop rbp

	push r10										;put return address
	ret
;==============================================================================
