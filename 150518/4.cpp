#if 0
// ��� �̴ϼȶ�����
#include <iostream>
using namespace std;

class Circle
{
public :
	int r;
	const double PI; // = 3.14;

	// ��� �̴ϼȶ������� ��� ������ ��¥ �ʱ�ȭ�ϱ� ���� ����
	// Ŭ���� ������ �ɺ��� ��� �Ǵ� ���۷����� �����Ѵٸ� �ݵ��
	// ��� �̴ϼȶ������� ���� �ʱ�ȭ�ؾ� �Ѵ�.
	Circle(int radius = 0) : PI(3.14)
	{
		// �Ʒ� �� �ڵ�� �ʱ�ȭ�� �ƴ� ����
		r = radius;		// OK
		// PI = 3.14	// ERROR
	}
};

int main()
{
	Circle c(2);
	cout << c.PI << endl;
}
#endif

#if 0
// IS-A 
#include <iostream>
using namespace std;

class Point
{
public :
	int x, y;

	Point() { cout << "Point" << endl; }
	Point(int a, int b) { cout << "Point(int a, int b)" << endl; }
};

class Point3D : public Point
{
public :
	int z;

	// ��� ���迡���� �ڽ� �����ڰ� ȣ��ȴ�. �ٸ� �⺻ �����ڷ� ����, �θ� �����ڰ� ���� ȣ��Ǵ� ��ó�� ���δ�.
	// ��� �̴ϼȶ������� ����, ��������� �����ؼ� �θ��� �����ڸ� ȣ�����ش�.
	// ��� �̴ϼȶ������� ����ϸ� �θ��� �����ڵ� �����ؼ� ȣ���� �� �ִ�.
	// ��� ���迡 �ִ� �� Ŭ�������� ��ü�� ������ ��, �ڽ��� �����ڰ� ȣ��� ��,
	// �θ��� �����ڰ� ȣ��ȴ�.
	Point3D() : Point(1, 1)
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
// HAS-A
#include <iostream>
using namespace std;

class Point
{
public:
	int x, y;
	Point(int a, int b) { cout << "Point(int a, int b)" << endl; }
};

class Point3D
{
public:
	Point pos;
	int z;

	// ��� �̴ϼȶ������� ����ϸ� ����� ���� Ÿ���� ����� �����ڵ�
	// �����Ͽ� ȣ���� �� �ִ�.
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

// ��� �̴ϼȶ����� ��� ��, ������ ��
#include <iostream>
using namespace std;

class Point
{
public : 
	int x, y;
	// ����� �ʱ�ȭ�� ��� �̴ϼȶ������� ������ �ƴ�
	// ����� ����� ������� �ʱ�ȭ�ȴ�.
	Point() : y(0), x(y) {}
};

void main()
{
	Point p;
	cout << p.x << endl;
	cout << p.y << endl;
}

// ��� �̴ϼȶ����� ����
// 1. �����ڿ����� ��� �Ҵ��� �ʱ�ȭ�� �ƴ϶� ����
// 2. ��� ������ �ʱ�ȭ�Ϸ��� ��� �̴ϼȶ������� ����ؾ� ��
// 3. ��� �̴ϼȶ������� ��� ��ü�� Ư�� �����ڸ� ��������� ������ �� ����.
// 4. ��� �̴ϼȶ������� �θ� Ŭ������ Ư�� �����ڸ� ��������� ������ �� ����.
// 5. ��� ������ �ʱ�ȭ ������ ��� �̴ϼȶ������� �����ʹ� ��� ����, ����� ������� �ʱ�ȭ �ȴ�.