[bits 64]

global crash_me

crash_me:       ; division by zero MWAHAHAHAHAHA
    mov eax, 0
    div eax
    ret