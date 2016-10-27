; --------------------------------------------------------------------------- ;
; ports.asm
; --------------------------------------------------------------------------- ;

global portreadb        ; 
global portreadw        ;
global portwriteb       ;
global portwritew       ;

; stack:
;   [esp + 4] the I/O port
;   [esp] return address
portreadb:
    mov dx, [esp+4]
    in al, dx
    ret

; stack:
;   [esp + 4] the I/O port
;   [esp] return address
portreadw:
    mov dx, [esp+4]
    in ax, dx
    ret

; stack:
;   [esp + 8] the data byte
;   [esp + 4] the I/O port
;   [esp] return address
portwriteb:
    mov al, [esp+8]
    mov dx, [esp+4]
    out dx, al
    ret

; stack:
;   [esp + 8] the data byte
;   [esp + 4] the I/O port
;   [esp] return address
portwritew:
    mov ax, [esp+8]
    mov dx, [esp+4]
    out dx, ax
    ret