#if 0
// 캐스팅 이야기
#include <stdlib.h>

void main()
{
	// char* p = (char*)malloc(sizeof(char)); 쓰지마 C++에서는 ...
	
	// 1. static_cast : 이성적인 형 변환을 할 때 사용
	char* p = static_cast<char*>(malloc(sizeof(char)));

	// 2. reinterpret_cast : 비 이성적인 형 변환을 할 때 사용
	// C언어와 같이 대부분의 형 변환을 지원한다.
	// ex. big-little endian 판단 시, 아래와 같이 비 이성적인 코드를 사용하기도 함 ...
	// long x = 1;
	// char* p = &x
	int n = 1;
	// char* p1 = &n; // 컴파일러의 타입 체크 기능을 끄게됨
	// char* p1 = static_cast<char*>(&n); // C++ 컴파일러가 봤을 때, 이건 말도 안됨 ... dereferece 불가능
	char* p1 = reinterpret_cast<char*>(&n);
}
#endif

#include <iostream>
using namespace std;

int main()
{
	// 3. const_cast : 객체의 논리적 상수성을 비상수 객체로 캐스팅
	// 가급적 사용을 자제하는 것이 좋음
	const int n = 20;
	// int *p = (int *)&n; // 컴파일러의 타입 체크 기능을 꺼짐
	// int *p = reinterpret_cast<int *>(&n);  // 이것도 납득 못함 ...
	int *p = const_cast<int *>(&n); // const_cast를 사용한다는 것은, 설계가 잘못되었다는 것이 중론이긴 함
	*p = 0;
}