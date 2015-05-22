
#if 0
// ��ü�� ���� ��å
// 1. ���� ����(shallow copy)
#include <iostream>
using namespace std;

class Point
{
public:
	int x, y;

	Point(int a = 0, int b = 0) : x(a), y(b) {}

	// ���� ���� ��å: C++ ����� �⺻ ��å
	Point(const Point& p) : x(p.x), y(p.y)
	{
		cout << "Point(Point)" << endl;
	}
};

int main()
{
	Point p1(1, 1);
	Point p2 = p1;	// Point p2(p1); -> �⺻ ���� �������� ��å: ���� ����
}
#endif


#if 0
// ���� ������ ������
#include <iostream>
#include <cstring>
using namespace std;

class Person
{
	char* name;
	int   age;

public:
	// "hello" -> h e l l \0
	Person(const char* n, int a) : age(a)
	{
		name = new char[strlen(n) + 1];
								//  ^--- NULL
		strcpy(name, n);
	}

	~Person() { delete[] name; }
};

void main()
{
	Person p1("kkk", 20);
	Person p2 = p1;
	getchar();
}
#endif


#if 0
// ���� ������ ������ -> ���� ���� 
#include <iostream>
#include <cstring>
using namespace std;

class Person
{
	char* name;
	int   age;

public:
	Person(const char* n, int a) : age(a)
	{
		name = new char[strlen(n) + 1];
		strcpy(name, n);
	}

	// ���� ���� ��å�� ���� ������
	Person(const Person& p) : age(p.age)	// �⺻ Ÿ���� ���, ���� ����
	{
		// ���� �Ҵ�� ��ü�� ���� ����
		name = new char[strlen(p.name) + 1];
		strcpy(name, p.name);
	}

	~Person() { delete[] name; }
};

void main()
{
	Person p1("kkk", 20);
	Person p2 = p1;
	getchar();
}

#endif



// ������ ���� ���
#include <iostream>
#include <cstring>
using namespace std;

class Person
{
	char* name;
	int   age;

public:
	Person(const char* n, int a) : age(a)
	{
		name = new char[strlen(n) + 1];
		strcpy(name, n);
	}

	// ������ ���� ��å�� ���� ������
	// ������ const�� �����ؾ� �մϴ�.
	Person(Person& p) : age(p.age), name(p.name)	// ���� ���� ���� ��
	{
		// ������ ����
		p.name = 0;
		p.age = 0;
	}

	~Person() { delete[] name; }
};

void main()
{
	Person p1("kkk", 20);
	Person p2 = p1;
	getchar();
}


