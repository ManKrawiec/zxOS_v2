; isr.asm
; Written by TechEverything

;
; Interrupt service routines for x86
;

%include "isr.inc"

section .text
global isr_common
global isr_table
extern ISRHandler
isr_common:
    pushad   ; save registers
    ; push selectors
    push ds 
    push es
    push fs
    push gs

    mov ax, 0x10    ; kernel data selector
    mov ds, ax
    mov es, ax

    mov eax, esp
    push eax
    call ISRHandler ; call C++

    add esp, 4

    pop gs
    pop fs
    pop es
    pop ds
    popad
    add esp, 8
    iret

isr_noerr 0
isr_noerr 1
isr_noerr 2
isr_noerr 3
isr_noerr 4
isr_noerr 5
isr_noerr 6
isr_noerr 7
isr_err 8
isr_noerr 9
isr_err 10
isr_err 11
isr_err 12
isr_err 13
isr_err 14
; 15 is reserved for Intel
isr_noerr 16
isr_err 17
isr_noerr 18
isr_noerr 19
isr_noerr 20
isr_err 21
isr_noerr 32
isr_noerr 33
isr_noerr 34
isr_noerr 35
isr_noerr 36
isr_noerr 37
isr_noerr 38
isr_noerr 39
isr_noerr 40
isr_noerr 41
isr_noerr 42
isr_noerr 43
isr_noerr 44
isr_noerr 45
isr_noerr 46
isr_noerr 47

isr_table:
    dd isr0
    dd isr1
    dd isr2
    dd isr3
    dd isr4
    dd isr5
    dd isr6
    dd isr7
    dd isr8
    dd isr9
    dd isr10
    dd isr11
    dd isr12
    dd isr13
    dd isr14
    dd 0
    dd isr16
    dd isr17
    dd isr18
    dd isr19
    dd isr20
    dd isr21
    times 10 dd 0
    dd isr32
    dd isr33
    dd isr34
    dd isr35
    dd isr36
    dd isr37
    dd isr38
    dd isr39
    dd isr40
    dd isr41
    dd isr42
    dd isr43
    dd isr44
    dd isr45
    dd isr46
    dd isr47