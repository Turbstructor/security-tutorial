
## hello.asm
'''
global _start

section .text
_start:
    mov rax, 1
    mov rdi, 1
    mov rsi, message
    mov rdx, 13
    syscall

    mov rax, 60
    xor rdi, rdi
    syscall

section .data
message:
    db "Hello, World", 10
'''

## strlen.asm
'''

BITS 64

section .text
global _start

strlen:
    mov rax,0                  
.looplabel:
    cmp byte [rdi],0           
    je  .end                    
    inc rdi                     
    inc rax                    
    jmp .looplabel            
.end:
    ret                      
    
_start:
    mov   rdi, msg              
    call  strlen
    add   al, '0'               
    mov  [len],al            
    mov   rax, 1           
    mov   rdi, 1          
    mov   rsi, len       
    mov   rdx, 2        
    syscall           
    mov   rax, 60    
    mov   rdi, 0    
    syscall        

section .data
    msg db "hello",0xA,0        
    len db 0,0xA  
'''
