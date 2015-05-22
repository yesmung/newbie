
#if 0
// new 연산자
#include <iostream>
#include <cstdlib>
using namespace std;

class Point
{
public:
	Point() { cout << "Point()" << endl; }
	~Point() { cout << "~Point()" << endl; }
};

void main()
{
	Point* p = static_cast<Point*>(operator new(sizeof(Point)));
	cout << p << endl;
}





// new 연산자가 하는 일
// 1. 메모리 동적 할당
// 2. 동적할당에 성공하고 피연산자가 사용자 정의 타입이라면 생성자 호출
// 3. lvalue 타입으로 형 변환

//void main()
//{
//	Point* p1 = static_cast<Point*>(malloc(sizeof(Point)));	// 생성자 호출 안됨
//	Point* p2 = new Point();	// 생성자 호출?
//}

#endif

#if 0
// new 연산자의 실제 모양
#include <iostream>
#include <cstdlib>
using namespace std;

void* operator new(size_t sz)
{
	cout << "new" << endl;
	return malloc(sz);
}

void* operator new(size_t sz, const char* fname)
{
	cout << "new: " << fname << endl;
	return malloc(sz);
}

void* operator new(size_t sz, const char* fname, int line)
{
	cout << "new: " << fname << "(" << line << ")" << endl;
	return malloc(sz);
}

void main()
{
	int* p1 = new int;	// 인자가 1개인 new 함수
	int* p2 = new(__FILE__) int;			// 인자가 2개인 new 함수
	int* p3 = new(__FILE__, __LINE__) int;	// 인자가 3개인 new 함수
}
#endif

#if 0
#include <iostream>
#include <cstdlib>
using namespace std;

class Point
{
public:
	Point() { cout << "Point()" << endl; }
	~Point() { cout << "~Point()" << endl; }
};

//void* operator new(size_t sz, void* p)
//{
//	return p;
//}

void main()
{
	Point* p = static_cast<Point*>(malloc(sizeof(Point)));
	new(p) Point;
}

//
//void main()
//{
//	Point p;
//
//	p.Point();	// ?
//	p.~Point();	// 소멸자는 명시적으로 호출 가능
//	getchar();
//}
#endif


// 생성자의 명시적 호출이 필요한 경우
#include <iostream>
using namespace std;

class Point
{
public:
	Point(int a, int b) { printf("Point(%d, %d)\n", a, b); }
};

void main()
{
	// Point* p1 = new Point(0, 0);

	// 아래의 코드가 동작되도록 구현해 보세요 :D
	// Point* p2 = new Point[10];

	// 1. 먼저 생성자 없이 메모리만 할당
	Point* p = static_cast<Point*>(malloc(sizeof(Point) * 10));

	// 2. 10개의 객체에 생성자를 호출
	for (int i = 0; i < 10; i++)
		new(p + i) Point(0, 0);
}
