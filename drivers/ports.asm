; --------------------------------------------------------------------------- ;
; ports.asm
; --------------------------------------------------------------------------- ;

global port_read_byte        ;
global port_read_word        ;
global port_write_byte       ;
global port_write_word       ;

; stack:
;   [esp + 4] the I/O port
;   [esp] return address
port_read_byte:
    mov dx, [esp+4]
    in al, dx
    ret

; stack:
;   [esp + 4] the I/O port
;   [esp] return address
port_read_word:
    mov dx, [esp+4]
    in ax, dx
    ret

; stack:
;   [esp + 8] the data byte
;   [esp + 4] the I/O port
;   [esp] return address
port_write_byte:
    mov al, [esp+8]
    mov dx, [esp+4]
    out dx, al
    ret

; stack:
;   [esp + 8] the data byte
;   [esp + 4] the I/O port
;   [esp] return address
port_write_word:
    mov ax, [esp+8]
    mov dx, [esp+4]
    out dx, ax
    ret
