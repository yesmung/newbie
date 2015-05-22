
#if 0
// 멤버 이니셜라이저
#include <iostream>
using namespace std;

class Circle
{
public:
	int r;
	const double PI;	// = 3.14;
		
	// 멤버 이니셜라이저란 멤버 변수를 진짜 초기화하기 위한 문법
	// 클래스 내부에 심볼릭 상수 또는 레퍼런스가 존재한다면 반드시
	// 멤버 이니셜라이저를 통해 초기화해야 합니다. 
	Circle(int radius = 0) : PI(3.14)
	{
		// 아래의 두 코드는 초기화가 아니라 대입
		r = radius;		// OK
		// PI = 3.14;	// ERROR
	}
};

int main()
{
	Circle c(2);
	cout << c.PI << endl;
}

#endif


#if 0
#include <iostream>
using namespace std;

class Point
{
public:
	int x, y;

	Point(int a, int b) { cout << "Point" << endl; }
};

class Point3D : public Point
{
public:
	int z;

	// 멤버 이니셜라이저를 사용하면 부모의 생성자도 선택해서 호출할 수 있습니다.
	// 상속 관계에 있는 두 클래스에서 객체가 생성될 때, 자식의 생성자가 호출된 후
	// 부모의 생성자가 호출됩니다.
	Point3D() : Point(1,1)
	{
		cout << "Point3D" << endl;
	}
};

void main()
{
	Point3D p3d;
}
#endif


#if 0
#include <iostream>
using namespace std;

class Point
{
public:
	int x, y;
	Point(int a, int b) { cout << "Point" << endl; }
};

class Point3D
{
public:
	Point pos;
	int z;

	// 멤버 이니셜라이저를 사용하면 사용자 정의 타입의 멤버의 생성자도
	// 선택하여 호출할 수 있습니다.
	Point3D() : pos(1,1)
	{
		cout << "Point3D" << endl;
	}
};

void main()
{
	Point3D p3d;
}
#endif


// 멤버 이니셜라이저 사용 시 주의할 점
#include <iostream>
using namespace std;

class Point
{
public:
	int x, y;
	// 멤버의 초기화는 멤버 이니셜라이저의 순서가 아닌
	// 멤버가 선언된 순서대로 초기화됩니다.
	Point() : y(0), x(y) {}
};

void main()
{
	Point p;
	cout << p.x << endl;	// ?
	cout << p.y << endl;	// ?
}

// 멤버 이니셜라이저 정리 (교재 17페이지)
// 1. 생성자에서의 멤버 할당은 초기화가 아니라 대입
// 2. 멤버 변수를 초기화하려면 멤버 이니셜라이저를 사용해야 함
// 3. 멤버 이니셜라이저는 멤버 객체의 특정 생성자를 명시적으로 선택할 수 있음
// 4. 멤버 이니셜라이저는 부모 클래스의 특정 생성자를 명시적으로 선택할 수 있음
// 5. 멤버 변수의 초기화 순서는 멤버 이니셜라이저의 순서와는 상관 없이
// 선언된 순서대로 초기화 된다.

