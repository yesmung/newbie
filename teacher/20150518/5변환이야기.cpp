
#if 0
// 캐스팅 이야기
#include <stdlib.h>

// 현재 코드의 확장자를 .cpp로 변경해보세요 :D

void main()
{
	// 1. static_cast: 이성적인 형 변환을 할 때 사용
	char* p = static_cast<char*>(malloc(sizeof(char)));

	// 2. reinterpret_cast: 비 이성적인 형 변환을 할 때 사용
	// C 언어와 같이 대부분의 형 변환을 지원합니다.
	int n = 1;
	// char* p1 = (char*)&n;	// 컴파일러의 타입 체크 기능을 끄게 됩니다.
	// char* p1 = static_cast<char*>(&n);
	char* p1 = reinterpret_cast<char*>(&n);
}
#endif


#include <iostream>
using namespace std;

void main()
{
	const int n = 20;

	// 3. const_cast: 객체의 논리적 상수성을 비상수 객체로 캐스팅
	// 가급적 사용을 자제하는 것이 좋음
	// int* p = (int*)&n;
	// int* p = reinterpret_cast<int*>(&n);
	int* p = const_cast<int*>(&n);
	*p = 0;
}