%macro isrnoerror 1
global isr%1
isr%1:
    push 0
    push %1
    jmp isr_common
%endmacro

%macro isrerror 1
global isr%1
isr%1:
    push %1
    jmp isr_common
%endmacro


%macro IRQ 2
    global irq%1
    irq%1:
        cli
        push 0
        push %2
        jmp irq_common
%endmacro

%macro PUSHALL 0
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
%endmacro

%macro POPALL 0
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
%endmacro



[extern ISR_handler]
[extern PIC_eoi]

isr_common:
    PUSHALL

    mov rdi, rsp
    call ISR_handler
    mov rsp, rax

    POPALL

    add rsp, 16 ; Error code and interrupt number

    sti
    call PIC_eoi

    iretq


[extern IRQ_handler]

irq_common:
    PUSHALL

    mov rdi, rsp
    call IRQ_handler
    mov rsp, rax

    POPALL

    add rsp, 16 ; Error code and interrupt number

    iretq


isrnoerror  0
isrnoerror  1
isrnoerror  2
isrnoerror  3
isrnoerror  4
isrnoerror  5
isrnoerror  6
isrnoerror  7
isrerror    8
isrnoerror  9
isrerror    10
isrerror    11
isrerror    12
isrerror    13
isrerror    14
isrnoerror  15
isrnoerror  16
isrerror    17
isrnoerror  18
isrnoerror  19
isrnoerror  20
isrnoerror  21
isrnoerror  22
isrnoerror  23
isrnoerror  24
isrnoerror  25
isrnoerror  26
isrnoerror  27
isrnoerror  28
isrnoerror  29
isrerror    30
isrnoerror  31

isrerror    128
isrnoerror  177

IRQ         0,  32
IRQ         1,  33
IRQ         2,  34
IRQ         3,  35
IRQ         4,  36
IRQ         5,  37
IRQ         6,  38
IRQ         7,  39
IRQ         8,  40
IRQ         9,  41
IRQ         10, 42
IRQ         11, 43
IRQ         12, 44
IRQ         13, 45
IRQ         14, 46
IRQ         15, 47