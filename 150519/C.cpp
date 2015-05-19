#if 0
// 인라인 함수
# include <iostream>
using namespace std;

int add(int a, int b)
{
	int result = a + b;
	return result;	// mov eax, result
					// ret
}

void main()
{
	int result;
	add(1, 2);	// push 2
				// push 1
				// call add
	
	__asm { mov result, eax }
	
	cout << result << endl;
}
#endif

#if 0
// 인라인 함수 : 함수의 호출 코드가 함수의 기계어 코드로 치환되는 함수
int add(int a, int b) { return a + b;  }
inline int inline_add(int a, int b) { return a + b; }

void main()
{
	int n = add(1, 1);
	int m = inline_add(1, 1);
}
#endif

#if 0
// 컴파일러가 인라인 명령을 무시하는 경우 1. 함수 포인터를 사용하는 경우
#include <iostream>
using namespace std;

int foo() { return 0; }
inline int goo() { return 0; }

void main()
{
	int n = foo();
	int m = goo();

	int(*fp)();
	int k;
	
	cin >> n;
	if (n == 0)
		fp = foo;
	else
		fp = goo;

	fp(); // call or inline
	// call 임 무조건.. call	DWORD PTR _fp$[ebp], 컴파일러가 어떤 함수를 호출할 지 판단 불가
}
#endif

// 컴파일러가 인라인 명령을 무시하는 경우 2. 재귀 호출의 사용
// 치환된 코드가 남아있지 않음..
#include <iostream>
using namespace std;

int fact(int n)
{
	if (n <= 1)
		return 1;
	return fact(n) * fact(n - 1);
}

void main()
{
	int result = fact(5);	// 
}

// 컴파일러가 인라인 명령을 무시하는 경우 3. 함수의 기계어 코드가 매우 큰 경우, 인라인 명령을 무시