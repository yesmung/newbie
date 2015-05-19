#if 0
// �ζ��� �Լ�
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
// �ζ��� �Լ� : �Լ��� ȣ�� �ڵ尡 �Լ��� ���� �ڵ�� ġȯ�Ǵ� �Լ�
int add(int a, int b) { return a + b;  }
inline int inline_add(int a, int b) { return a + b; }

void main()
{
	int n = add(1, 1);
	int m = inline_add(1, 1);
}
#endif

#if 0
// �����Ϸ��� �ζ��� ����� �����ϴ� ��� 1. �Լ� �����͸� ����ϴ� ���
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
	// call �� ������.. call	DWORD PTR _fp$[ebp], �����Ϸ��� � �Լ��� ȣ���� �� �Ǵ� �Ұ�
}
#endif

// �����Ϸ��� �ζ��� ����� �����ϴ� ��� 2. ��� ȣ���� ���
// ġȯ�� �ڵ尡 �������� ����..
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

// �����Ϸ��� �ζ��� ����� �����ϴ� ��� 3. �Լ��� ���� �ڵ尡 �ſ� ū ���, �ζ��� ����� ����