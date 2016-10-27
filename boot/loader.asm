; --------------------------------------------------------------------------- ;
; loader.asm
; --------------------------------------------------------------------------- ;

global loader                   ; define the entry point
extern kmain                   ; define the C entry point

MAGIC_NUMBER equ 0x1BADB002     ; a magic number
FLAGS        equ 0x0            ; multi boot flags
CHECKSUM     equ -MAGIC_NUMBER  ; magic number + checksum + flags == 0
KERNEL_STACK_SIZE equ 4096      ; size of stack in bytes

section .text
align 4
    dd MAGIC_NUMBER
    dd FLAGS
    dd CHECKSUM

loader:
    mov esp, kernel_stack + KERNEL_STACK_SIZE   ; point esp to the start of the
                                                ; stack (end of memory area)
    jmp kmain

section .bss
align 4                         ; align at 4 bytes
kernel_stack:                   ; label points to beginning of memory
    resb KERNEL_STACK_SIZE      ; reserve stack for the kernel
