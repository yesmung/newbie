
// �����Ϸ��� �����ε� �� �Լ��� ã�� ���
#include <iostream>
using namespace std;

class Float
{
public:
	float val;
	explicit Float(float f) : val(f) {}
};

// void foo(float f)  { cout << "foo(float)" << endl; }	    // 1. ������ġ: Ÿ���� �Ϻ��ϰ� ��ġ�� �Լ��� ã��
// void foo(double f) { cout << "foo(double)" << endl; }	// 2. �°�: �������� �ս��� ���� Ÿ������ ��ȯ
// void foo(int f)    { cout << "foo(int)" << endl; }		// 3. �⺻ ��ȯ: �⺻ Ÿ���� ���, �������� �ս��� �߻��ϴ��� ��ȯ �� ����
void foo(Float f)     { cout << "foo(Float)" << endl; }      // 4. ����� ���� ��ȯ:
// void foo(...)      { cout << "foo(...)" << endl; }		// 5. ���� ����

int main()
{
	foo(3.14f);	// ?
}
