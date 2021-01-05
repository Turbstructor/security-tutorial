global _start

section .text
strlen:
    mov rax, 0
.looplabel:
    cmp byte [rdi], 0
    je .end
    inc rdi
    inc rax
    jmp .looplabel
.end:
    ret

_start:
    ; Print "Hello World"
    mov rax, 1
    mov rdi, 1
    mov rsi, message
    mov rdx, 13
    syscall

    ; Retrieve number of arguments
    pop r8

    ; discard executing filename that has been passed as argument
    pop rsi
    dec r8 

top:
    cmp r8, 0
    jz exit

    ; Print "My name is " first
    mov rax, 1
    mov rdi, 1
    mov rsi, myname
    mov rdx, 12
    syscall

    ; Retrieve argument
    pop rsi
    mov r11, rsi ; And store the retrieved one to register

    ; Get length of the argument
    mov rdi, r11
    call strlen
    mov r12, rax ; And store the calculated length to register

    ; Print the argument next
    mov rax, 1
    mov rdi, 1
    mov rsi, r11
    mov rdx, r12
    syscall

    ; Print LF at the end
    mov rax, 1
    mov rdi, 1
    mov rsi, linefeed
    mov rdx, 1
    syscall

    dec r8
    jmp top

exit:
    mov rax, 60
    xor rdi, rdi
    syscall

section .data
message:
    db "Hello, World", 10
myname:
    db "My name is ", 0
linefeed:
    db 0x0A
