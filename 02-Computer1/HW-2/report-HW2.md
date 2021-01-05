# HW2 Report.

### 데이터 세그먼트
- `.text` : 텍스트 세그먼트. 함수 등 실행하는 코드들이 존재하는 부분. 읽기 전용.
- `.data` : 데이터 세그먼트. 처음에 초기화된 값과 그에 대응하는 이름들(?)이 존재하는 부분.
- `global _start` : `_start` 부분을 프로그램의 엔트리로 정함. = `_start` 부분에 있는 코드부터 시작.

### `syscall` : 사전에 정해진 명령어를 수행함.
-  그 중 밑 두 파일들에서 사용하는 `sys_write`.
    - `%rax` : `syscall`로 실행할 명령의 코드. `sys_write`를 위해선 `rax`에 1을 줘야 한다.
    - `%rdi` : write를 하게 될 파일의 위치(`unsigned int fd`). `1`일 경우 stdout - 표준 출력 - 으로 보낸다.
    - `%rsi` : write를 할 메세지(`const char *buf`).
    - `%rdx` : write를 할 메세지의 길이(`NULL`도 포함되는 것 같다). 고정값으로 줄 수도 있지만... 출력하려는 메세지가 이것보다 짧으면 몰라도 이것보다 길면 더러운 문자가 출력된다.
- `sys_exit` : 프로그램 종료
    - `%rdi` : 에러 코드(`int error_code`).

## 코드 분석(?)
- 각각 코드에 대한 주석으로 대체함.
### hello.asm
    global _start

    section .text
    _start:
        ; Print message "Hello World"
        mov rax, 1
        mov rdi, 1
        mov rsi, message
        mov rdx, 13 ; Length of "Hello World" (including NULL)
        syscall

        ; Terminate program
        mov rax, 60
        xor rdi, rdi
        syscall

    section .data
    message:
        db "Hello, World", 10 ; Array of bytes containing "Hello, World" and LF character(ASCII Code 10 in decimal).

### strlen.asm
    BITS 64 ; Target 프로세서 환경을 64비트로 설정.

    section .text
    global _start

    strlen: ; Codes calculating the length of string %rdi.
        mov rax,0 ; rax used as 'cnt': counting letters.
    .looplabel:
        cmp byte [rdi],0 ; Check if current letter that rdi is pointing is NULL.
        je  .end ; If the condition above satisfies then end the loop and goto .end immediately
        inc rdi ; increase rdi to point the next letter
        inc rax ; increase letter count
        jmp .looplabel ; go back to the start of .looplabel - loop.
    .end:
        ret ; return to where it was called.

    _start:
        mov   rdi, msg ; load msg "hello" to rdi which is a parameter of strlen.
        call  strlen ; call function 'strlen'
        
        add   al, '0' ; small part of rax. Add '0' so that the number is now expressed as an ASCII character, but word/sentence that is longer than 10 will NOT work with this code.
        mov  [len],al ; load/store the character-expressed length 'al' to 'len'
        
        ; Print the length of msg 'hello'
        mov   rax, 1           
        mov   rdi, 1          
        mov   rsi, len       
        mov   rdx, 2        
        syscall           
        
        ; Terminate program
        mov   rax, 60    
        mov   rdi, 0    
        syscall        

    section .data
        msg db "hello",0xA,0 ; msg containing array of bytes "hello", including LF character and NULL.
        len db 0,0xA ; len containing NULL character and LF character.
