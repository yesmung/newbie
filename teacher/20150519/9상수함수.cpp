
// 상수 함수의 문제 - 변하는 것과 변하지 않는 것의 분리
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

	// const char* to_str(const Point* const this);
	const char* to_str() const
	{
		if (pCache->cached == false)
		{
			sprintf(pCache->str, "Point(%d, %d)", x, y);
			pCache->cached = true;
		}
		return pCache->str;
	}
};


void main()
{
	Point p(1, 1);
	cout << p.to_str() << endl;
}

