// �����Ϸ��� �����ε� �� �Լ��� ã�� ���
# include <iostream>
using namespace std;

// void foo(float f) { cout << "foo(float)" << endl; }		// 1. ������ġ : Ÿ���� �Ϻ��ϰ� ��ġ�� �Լ��� ã��
// void foo(double f) { cout << "foo(double)" << endl; }	// 2. �°�(promotion) : �����Ϸ��� �������� �ս��� ���� Ÿ������ ��ȯ
// void foo(int f) { cout << "foo(int)" << endl; }			// 3. �⺻ ��ȯ : �⺻ Ÿ���� ���, �������� �ս��� �߻��ϴ��� ��ȯ �� ����
// void foo(Float f) 6.cpp�� explicit�� ����					// 4. ����� ���� ��ȯ
// void foo(...) { cout << "foo(...)" << endl; }			// 5. ��������

class Float
{
public : 
	float val;
	Float(float f) : val(f) {}
	//explicit Float(float f) : val(f) {}
};

void foo(Float f) { cout << "foo(Float)" << endl; }

int main()
{
	foo(3.14f); // foo(Float(3.14f)) �� �ȴ�... 
}