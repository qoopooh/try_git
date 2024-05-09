; Installation:
;     sudo apt install nasm
; 
; Run:
;     nasm -f elf64 hello_world.asm -o hello_world.o
;     ld hello_world.o    # a.out is the executable file
;
section .data
    msg db 'Hello, World!', 0x0A ; The message to print, followed by a newline character (0x0A)

section .text
    global _start                ; Entry point for the program

_start:
    ; Write syscall (sys_write)
    mov     rax, 1              ; syscall number for sys_write
    mov     rdi, 1              ; file descriptor 1 (stdout)
    mov     rsi, msg            ; address of the message to print
    mov     rdx, 14             ; length of the message
    syscall                     ; invoke syscall

    ; Exit syscall (sys_exit)
    mov     rax, 60             ; syscall number for sys_exit
    xor     rdi, rdi            ; exit code 0
    syscall                     ; invoke syscall

