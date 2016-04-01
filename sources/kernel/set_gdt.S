.intel_syntax noprefix

gdtr: .word 0 # For limit storage
      .long 0 # For base storage

.global set_gdt
.section .text
set_gdt:
   mov   eax, [esp + 4]
   mov   [gdtr + 2], eax
   mov   ax, [esp + 8]
   mov   [gdtr], ax
   lgdt  [gdtr]
   ret
