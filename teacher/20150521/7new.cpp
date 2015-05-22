
#if 0
// new ������
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





// new �����ڰ� �ϴ� ��
// 1. �޸� ���� �Ҵ�
// 2. �����Ҵ翡 �����ϰ� �ǿ����ڰ� ����� ���� Ÿ���̶�� ������ ȣ��
// 3. lvalue Ÿ������ �� ��ȯ

//void main()
//{
//	Point* p1 = static_cast<Point*>(malloc(sizeof(Point)));	// ������ ȣ�� �ȵ�
//	Point* p2 = new Point();	// ������ ȣ��?
//}

#endif

#if 0
// new �������� ���� ���
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
	int* p1 = new int;	// ���ڰ� 1���� new �Լ�
	int* p2 = new(__FILE__) int;			// ���ڰ� 2���� new �Լ�
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
//	p.~Point();	// �Ҹ��ڴ� ��������� ȣ�� ����
//	getchar();
//}
#endif


// �������� ����� ȣ���� �ʿ��� ���
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

	// �Ʒ��� �ڵ尡 ���۵ǵ��� ������ ������ :D
	// Point* p2 = new Point[10];

	// 1. ���� ������ ���� �޸𸮸� �Ҵ�
	Point* p = static_cast<Point*>(malloc(sizeof(Point) * 10));

	// 2. 10���� ��ü�� �����ڸ� ȣ��
	for (int i = 0; i < 10; i++)
		new(p + i) Point(0, 0);
}
