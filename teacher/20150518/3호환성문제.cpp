
#if 0
// 호환성 문제
#include <stdio.h>

int main()
{
	// C++: C언어를 포함하고 있는 언어이므로
	// C 언어로 작성된 함수 호출 가능
	printf("hello, world\n");	// C function
}
#endif

// 3.cpp

#include "square.h"

int main()
{
	int x = square(2);
}

// 함수 오버로딩 정리
// 1. 매개 변수의 종류나 갯수가 다를 경우, 동일한 함수의 이름을 사용 가능
// 2. 함수 오버로딩의 원리는 컴파일러가 컴파일 타임에 함수의 이름을 변경(name mangling)
// -> 컴파일 타임에 결정되므로 성능 상의 오버헤드가 없다.
// 3. "C" 지시자는 C++ 컴파일러에게 선언된 함수의 이름 변경을 금지해달라고 요청
// 4. __cplusplus 매크로를 사용하여 C/C++ 모두를 지원하도록 설계해야 합니다.
// -> __cplusplus 매크로는 산업 표준입니다.
