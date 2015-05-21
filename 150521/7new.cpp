// new ������
#if 0
#include <iostream>
using namespace std;

class Point
{
public :
	Point() { cout << "Point()" << endl; }
	~Point() { cout << "~Point()" << endl; }
};

void main()
{
	Point* p = static_cast<Point*>(operator new(sizeof(Point))); // new�����ڵ� �������̱� ����, �Լ� �̴� ()
	cout << p << endl;
}

// new �����ڰ� �ϴ� ��
// 1. �޸� ���� �Ҵ�
// 2. �����Ҵ翡 �����ϰ� �ǿ����ڰ� ����� ���� Ÿ���̶�� ������ ȣ��
// 3. lvalue Ÿ������ �� ��ȯ

//void main()
//{
//	Point* p1 = static_cast<Point*>(malloc(sizeof(Point)));		// ������ ȣ�� �ȵ�
//	Point* p2 = new Point();	// ������ ȣ��
//}
#endif

#if 0
// new �������� ���� ���
#include <iostream>
using namespace std;

void* operator new(size_t sz)
{
	cout << "operator new(size_t)" << endl;
	return malloc(sz);
}

void* operator new(size_t sz, const char* fname)
{
	cout << "operator new(size_t, const char*)" << endl;
	return malloc(sz);
}

void* operator new(size_t sz, const char* fname, int line)
{
	cout << "operator new(size_t, const char*, int)" << endl;
	return malloc(sz);
}


void main()
{
	int* p1 = new int;	// ���ڰ� 1���� new �Լ�
	int* p2 = new(__FILE__) int;	// ���ڰ� 2���� new �Լ�
	int* p3 = new(__FILE__, __LINE__) int;	// ���ڰ� 3���� new �Լ�
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

// void* operator new(size_t sz, Point* p)
// void* operator new(size_t sz, void* p) // ���뼺 -> void* �� ����, �̹� ����, ������ �ʿ� ����. �� ǥ�ؿ��� �����Ұ�? ������ �ʿ伺�� �и��� �ִ�.
//{
//	return p;
//}

void main()
{
	Point* p = static_cast<Point*>(malloc(sizeof(Point)));
	new(p) Point;
}

//void main()
//{
//	Point p;
//	p.Point(); // ����? No, Error
//	getchar();
//	p.~Point();	// �Ҹ��ڴ� ��������� ȣ�� ����
//}
#endif

// �������� ����� ȣ���� �ʿ��� ���
// #include <stdio.h>
#include <iostream>
#include <cstdlib>
using namespace std;

class Point
{
public:
	Point(int a, int b) { printf("Point(%d, %d)\n", a, b); }
};

void main()
{
	// Point* p1 = new Point(0, 0);

	// Point* p2 = new Point[10]; // �̰��� �⺻ �����ڸ� ȣ���ϴ� ���̴�. Point* p2 = new Point[10](0,0) �̷� ������ ����.
	// Point* p2 = new Point[10];
	// 1. ���� ������ ���� �޸𸮸� �Ҵ�
	Point* p = static_cast<Point*>(malloc(sizeof(Point) * 10));

	// 2. 10���� ��ü�� �����ڸ� ȣ��
	for (int i = 0; i < 10; i++)
	{
		new(p + i) Point(0, 0);
	}
}