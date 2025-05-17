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
;	Destroy:	RSI, RDI, RAX, R8
;==============================================================================
crc32Asm:
	mov rax, 0xFFFFFFFF

	mov r8, [rdi]
	crc32 rax, r8

	mov r8, [rdi + 0x08]
	crc32 rax, r8

	mov r8, [rdi + 0x10]
	crc32 rax, r8

	mov r8, [rdi + 0x18]
	crc32 rax, r8

	not rax
	ret
;==============================================================================
