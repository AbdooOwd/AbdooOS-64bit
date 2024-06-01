[bits 64]

extern ISR_handler

; cpu pushes to the stack: ss, rsp, rflags, cs, rip

%macro ISR_NOERRORCODE 1
global isr%1
isr%1:
    push 0              ; push dummy error code
    push %1             ; push interrupt number
    jmp isr_common
%endmacro

%macro ISR_ERRORCODE 1
global isr%1
isr%1:
    ; cpu pushes an error code to the stack
    push %1             ; push interrupt number
    jmp isr_common
%endmacro

%include "src/interrupts/isr_gen.inc"

isr_common:
    ; Save general-purpose registers
    push rax
    push rcx
    push rdx
    push rbx
    push rsi
    push rdi
    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15
    push rbp

    ; Pass pointer to stack to C, so we can access all the pushed information
    mov rdi, rsp
    call ISR_handler

    ; Restore general-purpose registers
    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8
    pop rdi
    pop rsi
    pop rbp
    pop rbx
    pop rdx
    pop rcx
    pop rax

    add rsp, 16        ; Remove error code and interrupt number
    iretq              ; Will pop: rip, cs, rflags, rsp, ss