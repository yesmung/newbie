#if 0
// 호환성 문제
#include <stdio.h>

int main()
{
	// C++ : C언어를 포함하고 있는 언어이므로
	// C언어로 작성된 함수 호출 가능
	printf("hello, world\n");
}
#endif

#include <iostream>
#include "square.h"

using namespace std;

int main()
{
	int x = square(2);
	cout << x << endl;
}

// 함수 오버로딩 정리
// 1. 매개 변수의 종류나 갯수가 다를 경우, 동일한 함수의 이름을 사용 가능
// 2. 함수 오버로딩의 원리는 컴파일러가 컴파일 타임에 함수 이름을 변경(name mangling)
// => 컴파일 타임에 결정되므로, 성능 상의(런타임 대한) 오버헤드는 없다.
// 3. "C" 지시자는 C++ 컴파일러에게 선언된 함수의 이름 변경을 금지해달라고 요청
// 4. __cplusplus 매크로를 사용하여 C/C++ 모두를 지원하도록 설계 필요
// 5. __cplusplus 는 산업 표준