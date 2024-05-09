; Installation:
;     sudo apt install nasm
; 
; Run:
;     nasm -f elf64 adder.asm
;     ld adder.o          # a.out is the executable file
;
; There is no print out.

section .data
    ; Define variables
    num1 dd 10       ; First number to add
    num2 dd 20       ; Second number to add
    result dd 0      ; Variable to store the result

section .text
    global _start    ; Entry point for the program

_start:
    ; Load the first number into a register
    mov eax, [num1]

    ; Add the second number to the first number
    add eax, [num2]

    ; Store the result in the result variable
    mov [result], eax

    ; Exit the program
    mov eax, 60      ; syscall number for sys_exit
    xor edi, edi     ; exit code 0
    syscall          ; invoke syscall

