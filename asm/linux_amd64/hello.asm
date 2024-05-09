
;# Install
; sudo apt-get install nasm
; sudo apt-get install build-essential

;# Test
; nasm -f elf64 hello.asm -o hello.o
; ld hello.o -o out-hello-asm
; ./out-hello-asm


section .data
    hello db 'Hello, World!', 0xA  ; 'Hello, World!' followed by a newline character
    helloLen equ $ - hello          ; Length of the 'Hello, World!' string

section .text
    global _start                   ; Entry point for the program

_start:
    ; sys_write (write to stdout)
    mov rax, 1                      ; syscall number for sys_write
    mov rdi, 1                      ; file descriptor 1 is stdout
    mov rsi, hello                  ; address of the string to output
    mov rdx, helloLen               ; number of bytes
    syscall                         ; make the syscall

    ; sys_exit (exit the program)
    mov rax, 60                     ; syscall number for sys_exit
    xor rdi, rdi                    ; exit code 0
    syscall                         ; make the syscall

