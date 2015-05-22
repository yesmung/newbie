
// 상수 함수의 문제
#include <iostream>
using namespace std;

class Point
{
public:
	int x, y;
	mutable char str[32];
	mutable bool cached;

	Point(int a = 0, int b = 0) : x(a), y(b), cached(false) {}

	// 현재 객체를 문자열로 변경하기 위한 to_str 함수를 구현합니다.
	const char* to_str() const	// char* to_str(Const Point* const this)
	{
		if (cached == false)
		{
			sprintf(str, "Point(%d, %d)", x, y);
			cached = true;
		}
		return str;
	}
	// 상수 함수는 멤버 변수의 포인터나 참조를 반환할 수 없습니다.
};


void main()
{
	Point p(1, 1);
	cout << p.to_str() << endl;
}

