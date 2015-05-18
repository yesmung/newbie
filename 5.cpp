#if 0
// ĳ���� �̾߱�
#include <stdlib.h>

void main()
{
	// char* p = (char*)malloc(sizeof(char)); ������ C++������ ...
	
	// 1. static_cast : �̼����� �� ��ȯ�� �� �� ���
	char* p = static_cast<char*>(malloc(sizeof(char)));

	// 2. reinterpret_cast : �� �̼����� �� ��ȯ�� �� �� ���
	// C���� ���� ��κ��� �� ��ȯ�� �����Ѵ�.
	// ex. big-little endian �Ǵ� ��, �Ʒ��� ���� �� �̼����� �ڵ带 ����ϱ⵵ �� ...
	// long x = 1;
	// char* p = &x
	int n = 1;
	// char* p1 = &n; // �����Ϸ��� Ÿ�� üũ ����� ���Ե�
	// char* p1 = static_cast<char*>(&n); // C++ �����Ϸ��� ���� ��, �̰� ���� �ȵ� ... dereferece �Ұ���
	char* p1 = reinterpret_cast<char*>(&n);
}
#endif

#include <iostream>
using namespace std;

int main()
{
	// 3. const_cast : ��ü�� ���� ������� ���� ��ü�� ĳ����
	// ������ ����� �����ϴ� ���� ����
	const int n = 20;
	// int *p = (int *)&n; // �����Ϸ��� Ÿ�� üũ ����� ����
	// int *p = reinterpret_cast<int *>(&n);  // �̰͵� ���� ���� ...
	int *p = const_cast<int *>(&n); // const_cast�� ����Ѵٴ� ����, ���谡 �߸��Ǿ��ٴ� ���� �߷��̱� ��
	*p = 0;
}