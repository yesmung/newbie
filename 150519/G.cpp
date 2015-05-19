// ��ü�� ���� ��å

#if 0
// 1. ���� ����(shallow copy)
#include <iostream>
using namespace std;

class Point
{
public :
	int x, y;
	Point(int a = 0, int b = 0) : x(a), y(b) {}

	// ���� ���� ��å : C++ ����� �⺻ ��å
	Point(const Point& p) : x(p.x), y(p.y)
	{
		cout << "Point(const Point* p)" << endl;
	}
};

int main()
{
	Point p1(1, 1);
	Point p2 = p1; // Point p2(p1) ; -> �⺻ ���� �������� ��å : ���� ����
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
	int age;
public : 
	
	Person(const char* n, int a) : age(a)
	{
		name = new char[strlen(n) + 1]; // +1�� NULL ����
		strcpy(name, n);
	}

	// ���� ���� ��å�� ���� ������
	Person(const Person& p) : age(p.age) // �⺻ Ÿ���� ��� ���� ����
	{
		// ���� �Ҵ�� ��ü�� ���� ����
		name = new char[strlen(p.name) + 1];
		strcpy(name, p.name);
	}

	~Person() 
	{ 
		cout << "~Person()" << endl;
		delete[] name;
	}
};

void main()
{
	Person p1("kkk", 20);
	Person p2 = p1;
	getchar();
}
#endif

#include <iostream>
#include <cstring>
using namespace std;

class Person
{
	char* name;
	int age;
public:

	Person(const char* n, int a) : age(a)
	{
		name = new char[strlen(n) + 1]; // +1�� NULL ����
		strcpy(name, n);
	}

	// ������ ���� ��å�� ���� ������
	// ������ const�� �����ؾ� �մϴ�.
	// c++11 ���� move constructor ���� �ִ�.
	Person(Person& p) : age(p.age), name(p.name) // ���� ���� ����
	{
		// ������ ����
		p.name = 0;
		p.age = 0;
	}

	~Person()
	{
		cout << "~Person()" << endl;
		delete[] name;
	}
};

void main()
{
	Person p1("kkk", 20);
	Person p2 = p1;
	getchar();
}