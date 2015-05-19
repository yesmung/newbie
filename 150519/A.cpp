#if 0
// ��� �Լ��� �����ε�
#include <iostream>
using namespace std;

class Point
{
public :
	int x, y;

	Point(int a = 0, int b = 0) : x(a), y(b) {}
	void display() // void display(Point* const this) // thiscall���
	{ 
		printf("- Point(%d, %d)\n", x, y); 
		// this = 0; //ERROR
		// this->x = 0; //OK
	}
	void display() const // void display(const Point* const display)
	{ 
		printf("Point(%d, %d)\n", x, y); 
		// this = 0; // ERROR
		// this->x = 0; // ERROR
	}
};

// ��� �Լ��� �Ϲ� ��� �Լ��� �ñ״�ó�� �ٸ��Ƿ�
// �����ε��� �����մϴ�.

void main()
{
	Point p;
	p.display();
}
#endif

#include <iostream>
using namespace std;

class Point
{
public:
	int x, y;

	Point(int a = 0, int b = 0) : x(a), y(b) {}
	// void display() { printf("called display\n"); }
	void display() const { printf("called display() const\n"); }
};

void main()
{
	const Point p1;
	p1.display();	// ��� �Լ� ȣ�� : ��� ��ü�� ��� �Լ��� ȣ�� ����

	Point p2;
	p2.display();	// �Ϲ� �Լ� ȣ�� : �Ϲ� ��ü�� �Ϲ� �Լ�, ��� �Լ� ��� ȣ�� ����
					// �⺻������ �Ϲ� �Լ��� ȣ��
					// ���� �Ϲ� �Լ��� ���� ���, ����å���� ��� �Լ��� ȣ��
}








