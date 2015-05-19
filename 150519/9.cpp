// 상수 함수의 문제

#include <iostream>
using namespace std;

struct Cache
{
	char str[32];
	bool cached;

	Cache() : cached(false) {}
};

class Point
{
public:
	int x, y;
	Cache* pCache;
	Point(int a = 0, int b = 0) : x(a), y(b), pCache(new Cache) {}

	// char* to_str(const Point* const this)
	const char* to_str() const 
	{
		if (pCache->cached == false) {
			sprintf(pCache->str, "Point(%d, %d)", x, y);
			pCache->cached = true;
		}
		return pCache->str;
	}
};

void main()
{
	Point p1(1, 1);
	cout << p1.to_str() << endl;
}