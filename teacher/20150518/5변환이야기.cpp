
#if 0
// ĳ���� �̾߱�
#include <stdlib.h>

// ���� �ڵ��� Ȯ���ڸ� .cpp�� �����غ����� :D

void main()
{
	// 1. static_cast: �̼����� �� ��ȯ�� �� �� ���
	char* p = static_cast<char*>(malloc(sizeof(char)));

	// 2. reinterpret_cast: �� �̼����� �� ��ȯ�� �� �� ���
	// C ���� ���� ��κ��� �� ��ȯ�� �����մϴ�.
	int n = 1;
	// char* p1 = (char*)&n;	// �����Ϸ��� Ÿ�� üũ ����� ���� �˴ϴ�.
	// char* p1 = static_cast<char*>(&n);
	char* p1 = reinterpret_cast<char*>(&n);
}
#endif


#include <iostream>
using namespace std;

void main()
{
	const int n = 20;

	// 3. const_cast: ��ü�� ���� ������� ���� ��ü�� ĳ����
	// ������ ����� �����ϴ� ���� ����
	// int* p = (int*)&n;
	// int* p = reinterpret_cast<int*>(&n);
	int* p = const_cast<int*>(&n);
	*p = 0;
}