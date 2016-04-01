.intel_syntax noprefix

.global do_int_69
.section .text
do_int_69:
    int 69
    ret
