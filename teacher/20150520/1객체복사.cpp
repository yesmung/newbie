
// 객체 복사

// 5. 참조 계수 기반의 복사 정책
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

	// 참조 계수 기반의 복사 정책
	Person(const Person& o) : name(o.name), age(o.age), ref(o.ref) // 얕은 복사 수행 후,
	{
		// 참조 계수 증분
		++(*ref);
	}

	// 참조 계수 기반의 소멸자
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
	// 현재 프로그램이 런타임 에러가 나지 않도록
	// 코드를 수정해 보세요 :D
	Person p1("kkk", 10);
	Person p2 = p1;
	getchar();
}