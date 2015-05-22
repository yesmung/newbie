
#if 0
// ��� �̴ϼȶ�����
#include <iostream>
using namespace std;

class Circle
{
public:
	int r;
	const double PI;	// = 3.14;
		
	// ��� �̴ϼȶ������� ��� ������ ��¥ �ʱ�ȭ�ϱ� ���� ����
	// Ŭ���� ���ο� �ɺ��� ��� �Ǵ� ���۷����� �����Ѵٸ� �ݵ��
	// ��� �̴ϼȶ������� ���� �ʱ�ȭ�ؾ� �մϴ�. 
	Circle(int radius = 0) : PI(3.14)
	{
		// �Ʒ��� �� �ڵ�� �ʱ�ȭ�� �ƴ϶� ����
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

	// ��� �̴ϼȶ������� ����ϸ� �θ��� �����ڵ� �����ؼ� ȣ���� �� �ֽ��ϴ�.
	// ��� ���迡 �ִ� �� Ŭ�������� ��ü�� ������ ��, �ڽ��� �����ڰ� ȣ��� ��
	// �θ��� �����ڰ� ȣ��˴ϴ�.
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

	// ��� �̴ϼȶ������� ����ϸ� ����� ���� Ÿ���� ����� �����ڵ�
	// �����Ͽ� ȣ���� �� �ֽ��ϴ�.
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


// ��� �̴ϼȶ����� ��� �� ������ ��
#include <iostream>
using namespace std;

class Point
{
public:
	int x, y;
	// ����� �ʱ�ȭ�� ��� �̴ϼȶ������� ������ �ƴ�
	// ����� ����� ������� �ʱ�ȭ�˴ϴ�.
	Point() : y(0), x(y) {}
};

void main()
{
	Point p;
	cout << p.x << endl;	// ?
	cout << p.y << endl;	// ?
}

// ��� �̴ϼȶ����� ���� (���� 17������)
// 1. �����ڿ����� ��� �Ҵ��� �ʱ�ȭ�� �ƴ϶� ����
// 2. ��� ������ �ʱ�ȭ�Ϸ��� ��� �̴ϼȶ������� ����ؾ� ��
// 3. ��� �̴ϼȶ������� ��� ��ü�� Ư�� �����ڸ� ��������� ������ �� ����
// 4. ��� �̴ϼȶ������� �θ� Ŭ������ Ư�� �����ڸ� ��������� ������ �� ����
// 5. ��� ������ �ʱ�ȭ ������ ��� �̴ϼȶ������� �����ʹ� ��� ����
// ����� ������� �ʱ�ȭ �ȴ�.

