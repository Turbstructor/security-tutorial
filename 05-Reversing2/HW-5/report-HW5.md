# Report. HW5.

~~미세먼지 팁. 파일명에 샵(`#`)을 붙여서 이걸 링크로 걸려고 하면 링크에는 `%23`으로 바꿔줘야 한다. 그대로 놔 두면 얘가 "야 이거 없는데에에에에에에" 하는 이야기를 해 준다.~~

여기서는 둘 다 `radare2`를 사용하여 해결하였다.

## easyelf

1. 문장이 뭐가 있죠

우선 string들이 무엇이 있는지 확인해 보았다. ![`rabin2 -z easyelf` content](./easyelf_screenshots/easyelf_%231.png) `Correct!\n`, `Wrong\n`, 그리고 `Reversing.Kr Easy ELF\n\n` 이렇게 3개만 존재하는데, 앞의 2개는 각각 비밀번호를 맞췄을 떄와 틀렸을 때, 맨 뒤는 그냥 프로그램을 실행할 때 나오는 문장이다.

2. 함수가 뭐가 있죠

디버거 모드로 `radare2`를 킨 다음, `aa`("analyze all")를 치고 그 다음... 함수 리스트를 `afl`을 통하여 뽑아보았다. !['r2 -d easyelf'; aa; afl](./easyelf_screenshots/easyelf_%232.png) `main` 함수가 있으니, 여기서부터 시작을 해 보자. `dcu main`("continue until `main`) 다음 `Vpp` 명령어를 주면... 아래와 같은 코드가 나온다.

3. 흐름

!['dcu main'; Vpp](./easyelf_screenshots/easyelf_%234.png)

    0x0004853b      call sym.imp.write

이 부분까지는 처음 "Reversing.Kr Easy ELF\n\n"을 출력하는 부분이고... 바로 아래의

    0x00048540      call 0x8048434
    
부분으로 넘어가면...

![scanf](./easyelf_screenshots/easyelf_%236.png)

의

    0x08048434      push ebp
    0x08048435      mov ebp, esp
    0x08048437      sub esp, 0x18
    0x0804843a      mov eax, 0x8048650
    0x0804843f      mov dword [esp + 4], 0x804a020
    0x08048447      mov dword, [esp], eax
    0x0804844a      call sym.imp.__isoc99_scanf
    0x0804844f      leave
    0x08048450      ret
    
... 코드들이 나온다. 문자열을 입력받아 그 내용을 주소 `0x0804a020`...를 시작위치로 하여 넣는다. 그렇게 다시 main 부분으로 돌아오면 다시 바로 아래의 

    0x08048545      call 0x8048451
    
... 부분이 나오는데, 여기로 가면...

![check?](./easyelf_screenshots/easyelf_%238.png)

의 각 글자들을 비교하는 코드가 나온다. 여기서 맨 마지막 글자의 위치가 5번째 글자, 그러니까 `0x0804a024` 주소란 걸 알 수 있는데, 이건...

    0x080484a8      movzx eax, byte [0x804a025]
    0x080484af      test al, al
    0x080484b1      je 0x80484ba
    
...의 부분에서 `test` 명령어를 통하여 `0x804a025` 부분에 `NULL` 문자(0)이 들어가 있는지를 체크하기 때문이다.

여기서 글자가 각 위치에서 확인하려는 것과 일치하는 경우엔 다음 글자를 확인하는 과정으로 넘어가지만, 만약 아니라면... 항상,

    0x080484**      mov eax, 0
    0x080484**      jmp 0x80484f5
    
...의, eax를 0으로 바꾸고 `"Wrong"`을 출력하는 코드로 넘어간다.

우선 코드상으로는 두번째 글자부터 확인을 하는데, (이 문자'만' 숫자라서 그런 게 아닐까 싶다)

    0x08048454      movzx eax, byte [0x804a021]
    0x0804845b      cmp al, 0x31
    0x0804845d      je 0x8048469
    
주소 `0x0804a021`에 들어가 있는 1바이트 문자가 `0x31` (`1`)인지 체크한다.

5번째 글자도 바로 알아볼 수 있는데,

    0x08048496  movzx eax, byte [0x804a024]
    0x0804849d  cmp al, 0x58
    0x0804849f  je 0x80484a8
    
... 부분에서 (XOR 암호화 없이) 바로 문자와 비교를 진행하기 때문이다.

나머지 3개 글자들의 XOR 암호화가 진행되는데,

    0x080484**      movzx eax, byte [0x804a02*]
    0x080484**      xor eax, 0x**
    0x080484**      mov byte [0x804a02*], al

...으로 각 글자들을 XOR 연산을 취한 다음 바뀐 글자를 원래 글자가 있는 위치에 넣어두고,

    0x080484**      movzx eax, byte [0x804a02*]
    0x080484**      cmp al, 0x**
    0x080484**      je 0x80484**
    
... 이 바뀐 3글자들이 각각 특정 코드와 일치하는지를 본다. 이걸 표로 정리하면...

|---|---|---|---|
|index|address|xor-encoded code to compare|code to xor-encode|xor-decoded code|
|0|||`0x34`|`L`|
|2|||`0x32`|`N`|
|3|||`0xffffff88`|`U`|

가 나온다.

## bomb - 노란 선 제거
