.intel_syntax noprefix

.section .text
.global inb
inb:
	push ebp
	mov ebp, esp

	xor eax, eax
	xor dx, dx
	mov dx, [ebp + 8]
	inb al, dx

	mov esp, ebp
	pop ebp
	ret

.global inw
inw:
	push ebp
	mov ebp, esp

	xor eax, eax
	xor dx, dx
	mov dx, [ebp+8]
	in ax, dx

	mov esp, ebp
	pop ebp
	ret


.global inl
inl:
	push ebp
	mov esp, ebp

	xor eax, eax
	xor dx, dx
	mov dx, [ebp+8]
	in eax, dx

	mov ebp, esp
	pop ebp
	ret
