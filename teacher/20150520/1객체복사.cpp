
// ��ü ����

// 5. ���� ��� ����� ���� ��å
#include <iostream>
#include <cstring>
using namespace std;

class Person
{
	char* name;
	int   age;
	int*  ref;

public:
	Person(const char* n, int a) : age(a), ref(new int(1))
	{
		name = new char[strlen(n) + 1];
		strcpy(name, n);
	}

	// ���� ��� ����� ���� ��å
	Person(const Person& o) : name(o.name), age(o.age), ref(o.ref) // ���� ���� ���� ��,
	{
		// ���� ��� ����
		++(*ref);
	}

	// ���� ��� ����� �Ҹ���
	~Person()
	{
		if (--(*ref) == 0)
		{
			delete[] name;
			delete ref;
		}
	}
};

void main()
{
	// ���� ���α׷��� ��Ÿ�� ������ ���� �ʵ���
	// �ڵ带 ������ ������ :D
	Person p1("kkk", 10);
	Person p2 = p1;
	getchar();
}