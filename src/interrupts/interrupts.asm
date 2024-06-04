%macro isrnoerror 1
global isr%1
isr%1:
    push dword 0
    push %1
    jmp isr_common
%endmacro

%macro isrerror 1
global isr%1
isr%1:
    push %1
    jmp isr_common
%endmacro

extern ISR_handler

isr_common:
    push rax
    push rbx
    push rcx
    push rdx
    push rsi
    push rdi
    push rbp
    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15

    cld
    xor rbp, rbp
    call ISR_handler

    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8
    pop rbp
    pop rdi
    pop rsi
    pop rdx
    pop rcx
    pop rbx
    pop rax
    add rsp, 16 ; Error code and interrupt number
    iretq

isrnoerror 0
isrnoerror 1
isrnoerror 2
isrnoerror 3
isrnoerror 4
isrnoerror 5
isrnoerror 6
isrnoerror 7
isrerror   8
isrnoerror 9
isrerror   10
isrerror   11
isrerror   12
isrerror   13
isrerror   14
isrnoerror 15
isrnoerror 16
isrerror   17
isrnoerror 18
isrnoerror 19
isrnoerror 20
isrnoerror 21
isrnoerror 22
isrnoerror 23
isrnoerror 24
isrnoerror 25
isrnoerror 26
isrnoerror 27
isrnoerror 28
isrnoerror 29
isrerror   30
isrnoerror 31



; IRQs (i hate 'em)

; isrnoerror 32
; isrnoerror 33
; isrnoerror 34
; isrnoerror 35
; isrnoerror 36
; isrnoerror 37
; isrnoerror 38
; isrnoerror 39
; isrnoerror 40
; isrnoerror 41
; isrnoerror 42
; isrnoerror 43
; isrnoerror 44
; isrnoerror 45
; isrnoerror 46
; isrnoerror 47