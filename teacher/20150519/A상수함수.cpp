
#if 0
// 상수 함수의 오버로딩
#include <iostream>
using namespace std;

class Point
{
public:
	int x, y;

	Point(int a = 0, int b = 0) : x(a), y(b) {}

	void display()			// void display(Point* const this)
	{
		printf("Point(%d, %d)\n", x, y);

		this = 0;		// ERROR
		this->x = 0;	// OK
	}
	
	void display() const	// void display(const Point* const this)
	{
		printf("Point(%d, %d)\n", x, y);

		this = 0;		// ERROR
		this->x = 0;	// ERROR
	}
};

// 상수 멤버 함수와 일반 멤버 함수는 시그니처가 다르므로
// 오버로딩이 가능합니다.

void main()
{
	Point p;
	p.display();
}
#endif




#include <iostream>
using namespace std;

class Point
{
public:
	int x, y;
	Point(int a = 0, int b = 0) : x(a), y(b) {}

	// void display()       { printf("called display()\n"); }
	void display() const { printf("called display() const\n"); }
};



void main()
{
	const Point p1;
	p1.display();	// 상수 함수 호출: 상수 객체는 상수 함수만 호출 가능

	Point p2;
	p2.display();	// 일반 함수 호출 
					// 기본적으로 일반 함수를 호출
					// 만약 일반 함수가 없을 경우, 차선책으로
					// 상수 함수를 호출 한다.
}