; --------------------------------------------------------------------------- ;
; loader.asm
; --------------------------------------------------------------------------- ;

global loader                   ; define the entry point

MAGIC_NUMBER equ 0x1BADB002     ; a magic number
FLAGS        equ 0x0            ; multi boot flags
CHECKSUM     equ -MAGIC_NUMBER  ; magic number + checksum + flags == 0

section .text
align 4
    dd MAGIC_NUMBER
    dd FLAGS
    dd CHECKSUM

loader:
    mov eax, 0xCAFEBABE         ; our little signal that things went well
    jmp $                       ; loop forever
