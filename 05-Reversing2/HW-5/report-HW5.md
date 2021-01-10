# Report. HW5.

~~미세먼지 팁. 파일명에 샵(`#`)을 붙여서 이걸 링크로 걸려고 하면 링크에는 `%23`으로 바꿔줘야 한다. 그대로 놔 두면 얘가 "야 이거 없는데에에에에에에" 하는 이야기를 해 준다.~~

여기서는 둘 다 `radare2`를 사용하여 해결하였다.

## easyelf

1. 문장이 뭐가 있죠

우선 string들이 무엇이 있는지 확인해 보았다. ![`rabin2 -z easyelf` content](./easyelf_screenshots/easyelf_%231.png) `Correct!\n`, `Wrong\n`, 그리고 `Reversing.Kr Easy ELF\n\n` 이렇게 3개만 존재하는데, 앞의 2개는 각각 비밀번호를 맞췄을 떄와 틀렸을 때, 맨 뒤는 그냥 프로그램을 실행할 때 나오는 문장이다.

2. 함수가 뭐가 있죠

디버거 모드로 `radare2`를 킨 다음, `aa`("analyze all")를 치고 그 다음... 함수 리스트를 `afl`을 통하여 뽑아보았다. !['r2 -d easyelf'; aa; afl](.easyelf_screenshots/easyelf%232.png) `main` 함수가 있으니, 여기서부터 시작을 해 보자. `dcu main`("continue until `main`) 다음 `Vpp` 명령어를 주면... 아래와 같은 코드가 나온다.

!['dcu main'; Vpp](.easyelf_screenshots/easyelf%234.png)

## bomb - 노란 선 제거
