
// 상수 함수 이야기
#include <iostream>
using namespace std;

class Point
{
public:
	int x, y;

	Point(int a = 0, int b = 0) : x(a), y(b) {}

	void display() const
	{
		printf("Point(%d, %d)\n", x, y);
//		x = 0;
	}
};

void display(const Point& p)
{
	p.display();
}

void main()
{
	Point p(1, 1);
	display(p);	// p.display();
}

// 결론: 멤버를 변경하지 않는 멤버 함수의 경우에는 반드시
// const 키워드를 붙이자!
// -> 상수 객체는 상수 함수만 호출할 수 있기 때문