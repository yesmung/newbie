#include <iostream>
using namespace std;

// 함수 오버로딩

// 함수 오버로딩의 원리 : C++ 컴파일러가 함수의 이름을 변경(name mangling)하기 때문
// 이름을 변경하는 방법 : 함수의 이름 + 파라미터 조합
// name mangling : C/C++ 라이브러리 설계의 기본

// 어셈블리어 코드 생성 방법
// windows : cl.exe 파일명.cpp -FAs (/FAs)	-> 파일명.asm
// linux : g++ -S 파일명.cpp					-> 파일명.s

int square(int x) { return x*x; }
double square(double x) { return x*x; }

void main()
{
	int a = square(3);
	double b = square(3.3);

	cout << a << endl;
	cout << b << endl;
}