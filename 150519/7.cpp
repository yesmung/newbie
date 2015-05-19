// ��� �Լ� �̾߱�
#include <iostream>
using namespace std;

class Point
{
public :
	int x, y;

	Point(int a = 0, int b = 0) : x(a), y(b) {}

	void display() const
	{
		printf("Point(%d, %d)\n", x, y);
		// x = 0;
	}
};

void display(const Point& p)
{
	p.display(); // ��� ��ü�� ��� �Լ��� ȣ���� �� �ִ�. ���� ������ const�� �ٿ��ش�.
}

void main()
{
	Point p(1, 1);
	display(p);
	// p.display();
}

// ��� : ����� �������� �ʴ� ��� �Լ��� ��쿡�� �ݵ��
// const Ű���带 ������!
// -> ��� ��ü�� ��� �Լ��� ȣ���� �� �ֱ� �����̴�.