.intel_syntax noprefix

.section .text
.global outb
outb:
	push ebp
	mov ebp, esp

	xor eax, eax
	xor dx, dx
	mov dx, [ebp + 8]
	outb dx, al

	mov esp, ebp
	pop ebp
	ret

.global outw
outw:
	push ebp
	mov ebp, esp

	xor eax, eax
	xor dx, dx
	mov dx, [ebp+8]
	out dx, ax

	mov esp, ebp
	pop ebp
	ret


.global outl
outl:
	push ebp
	mov esp, ebp

	xor eax, eax
	xor dx, dx
	mov dx, [ebp+8]
	out dx, eax

	mov ebp, esp
	pop ebp
	ret
