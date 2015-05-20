// c++에서 객체 복사 정책
// 1. 얕은 복사
// 동적 할당된 객체
// 2. 깊은 복사 (실제로 많이 사용되지는 않는다)
// new 연산에 대한 비용이 크다
// 사본이 만들어 진다 -> 메모리 사용량 증가
// 3. 소유권 이전
// move constructor
// 4. 복사금지
// 복사생성자, 대입연산자(overloading)을 private 영역에 둔다
// 5. 참조 계수
// static 으로 만들면, thread의 safety 보장을 못함
// 즉, heap 영역에 만들어야 된다.

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