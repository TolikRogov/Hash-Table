;==============================================================================
;					My crc32 05.05.2025 @Rogov Anatoliy
;==============================================================================

global crc32Asm										;make global for linker
section .text										;start of code segment

;==============================================================================
;	Give string hash
;	Entry:		RDI - address of bytes string
;				RSI - size of string in bytes
;	Exit:		AX - string hash
;	Destroy:	RSI, RDI, RAX
;==============================================================================
crc32Asm:
	add rsi, rdi
	mov rax, 0xFFFFFFFF
	crc_loop:
		add rdi, 0x1
		crc32 rax, byte [rdi - 0x1]
		cmp rdi, rsi
	jne crc_loop
	not rax
	ret
;==============================================================================
