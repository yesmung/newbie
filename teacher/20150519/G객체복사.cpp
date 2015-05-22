
#if 0
// 객체의 복사 정책
// 1. 얕은 복사(shallow copy)
#include <iostream>
using namespace std;

class Point
{
public:
	int x, y;

	Point(int a = 0, int b = 0) : x(a), y(b) {}

	// 얕은 복사 정책: C++ 언어의 기본 정책
	Point(const Point& p) : x(p.x), y(p.y)
	{
		cout << "Point(Point)" << endl;
	}
};

int main()
{
	Point p1(1, 1);
	Point p2 = p1;	// Point p2(p1); -> 기본 복사 생성자의 정책: 얕은 복사
}
#endif


#if 0
// 얕은 복사의 문제점
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
// 얕은 복사의 문제점 -> 깊은 복사 
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

	// 깊은 복사 정책의 복사 생성자
	Person(const Person& p) : age(p.age)	// 기본 타입의 경우, 얕은 복사
	{
		// 동적 할당된 객체는 깊은 복사
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



// 소유권 이전 방식
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

	// 소유권 이전 정책의 복사 생성자
	// 인자의 const는 제거해야 합니다.
	Person(Person& p) : age(p.age), name(p.name)	// 얕은 복사 수행 후
	{
		// 소유권 포기
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


