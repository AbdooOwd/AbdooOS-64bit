%macro interrupt_handler_noerror 1
global interrupt_handler_%1
interrupt_handler_%1:
    push dword 0
    push %1
    jmp common_interrupt_handler
%endmacro

%macro interrupt_handler_error 1
global interrupt_handler_%1
interrupt_handler_%1:
    push %1
    jmp common_interrupt_handler
%endmacro

global idt_load

extern interrupt
; extern idtr
; 
; idt_load:
;     lidt [idtr]
;     ret

common_interrupt_handler:
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
    call interrupt

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

interrupt_handler_noerror 0
interrupt_handler_noerror 1
interrupt_handler_noerror 2
interrupt_handler_noerror 3
interrupt_handler_noerror 4
interrupt_handler_noerror 5
interrupt_handler_noerror 6
interrupt_handler_noerror 7
interrupt_handler_error 8
interrupt_handler_error 10
interrupt_handler_error 11
interrupt_handler_error 12
interrupt_handler_error 13
interrupt_handler_error 14
interrupt_handler_noerror 16
interrupt_handler_error 17
interrupt_handler_noerror 18
interrupt_handler_noerror 32
interrupt_handler_noerror 33
interrupt_handler_noerror 34
interrupt_handler_noerror 35
interrupt_handler_noerror 36
interrupt_handler_noerror 37
interrupt_handler_noerror 38
interrupt_handler_noerror 39
interrupt_handler_noerror 40
interrupt_handler_noerror 41
interrupt_handler_noerror 42
interrupt_handler_noerror 43
interrupt_handler_noerror 44
interrupt_handler_noerror 45
interrupt_handler_noerror 46
interrupt_handler_noerror 47