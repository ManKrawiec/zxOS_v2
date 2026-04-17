; isr.inc
; Written by TechEverything

;
; Macros for isr.asm
;

%ifndef ISR_INC
%define ISR_INC

%macro isr_noerr 1

isr%1:
    push 0  ; err_no
    push %1  ; int_no
    jmp isr_common

%endmacro

%macro isr_err 1

isr%1:
    push %1  ; int_no
    jmp isr_common

%endmacro

%endif