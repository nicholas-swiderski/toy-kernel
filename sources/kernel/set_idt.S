.intel_syntax noprefix

idtr: .word 0 # For limit storage
      .long 0 # For base storage

.global set_idt
.section .text
set_idt:
   mov   eax, [esp + 4]
   mov   [idtr + 2], eax
   mov   ax, [esp + 8]
   mov   [idtr], ax
   lidt  [idtr]
   ret

