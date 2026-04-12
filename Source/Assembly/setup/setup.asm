; setup.asm
; Written by TechEverything

;
; Sets up stack and calls C++ code
;

%include "setup.inc"

section .text
global _start
extern KernelMain
bits 32

_start:
    cli
    mov esp, stack_bottom + STACK_SIZE  ; esp points to stack_top

    push ebx
    push eax

    call KernelMain ; C++ function

    jmp _end
_end:
    cli
    hlt
    jmp _end

section .bss
stack_bottom: 
    resb STACK_SIZE