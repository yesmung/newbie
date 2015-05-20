// c++���� ��ü ���� ��å
// 1. ���� ����
// ���� �Ҵ�� ��ü
// 2. ���� ���� (������ ���� �������� �ʴ´�)
// new ���꿡 ���� ����� ũ��
// �纻�� ����� ���� -> �޸� ��뷮 ����
// 3. ������ ����
// move constructor
// 4. �������
// ���������, ���Կ�����(overloading)�� private ������ �д�
// 5. ���� ���
// static ���� �����, thread�� safety ������ ����
// ��, heap ������ ������ �ȴ�.

#include <iostream>
#include <cstring>
using namespace std;

class Person
{
	char* name;
	int age;
	int* refs;

public :
	Person(const char* n, int a) : age(a), refs(new int(1))
	{
		name = new char[strlen(n) + 1];
		strcpy(name, n);
		// refs = new int(1);
	}

	Person(const Person& c) : age(c.age), name(c.name), refs(c.refs)
	{
		cout << "refs = " << *refs << endl;
		(*refs)++;
	}

	~Person()
	{
		if (--(*refs) == 0)
		{
			delete[] name;
			delete refs;
		}
		/*
		if ((*refs) == 0)
		{
			delete[] name;
			delete refs;
		}
		else
		{
			(*refs)--;
		}
		*/
	}
};
+
void main()
{
	Person p1("kkk", 10);
	Person p2 = p1;
	getchar();
}