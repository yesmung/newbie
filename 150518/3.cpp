#if 0
// ȣȯ�� ����
#include <stdio.h>

int main()
{
	// C++ : C�� �����ϰ� �ִ� ����̹Ƿ�
	// C���� �ۼ��� �Լ� ȣ�� ����
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

// �Լ� �����ε� ����
// 1. �Ű� ������ ������ ������ �ٸ� ���, ������ �Լ��� �̸��� ��� ����
// 2. �Լ� �����ε��� ������ �����Ϸ��� ������ Ÿ�ӿ� �Լ� �̸��� ����(name mangling)
// => ������ Ÿ�ӿ� �����ǹǷ�, ���� ����(��Ÿ�� ����) �������� ����.
// 3. "C" �����ڴ� C++ �����Ϸ����� ����� �Լ��� �̸� ������ �����ش޶�� ��û
// 4. __cplusplus ��ũ�θ� ����Ͽ� C/C++ ��θ� �����ϵ��� ���� �ʿ�
// 5. __cplusplus �� ��� ǥ��