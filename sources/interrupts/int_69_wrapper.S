.intel_syntax noprefix

.extern int_69_handler

.section .text
.global int_69_wrapper
int_69_wrapper:
	pushad
	call int_69_handler
	popad
	iretd
