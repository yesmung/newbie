
#if 0
// Ŭ���� ���ø�
#include <iostream>

template<typename T> class Stack
{
public:
	// ������
	// Stack<T>() {}	// 1. ��ǥ��
	// Stack() {}		// 2. ǥ��

	// ���� ������
	//Stack(const Stack& s) {}		// 1. Ŭ���� ���ο����� ���
	//Stack(const Stack<T>& s); // {}		// 2. ��Ȯ�� ����

	// ��� �Լ�
	void push(const T& data);

	// �ٸ� Ÿ���� ���ڷ� �ϴ� �Լ��� ��� �Լ� ���ø����� ����
	template<typename U> void foo(const U& u);

};
// ��� �Լ��� �ܺ� ����
template<typename T> void Stack<T>::push(const T& data) {}

// ��� �Լ� ���ø��� ���Ǹ� ����
template<typename T> template<typename U> void Stack<T>::foo(const U& u) {}
void main()
{
	Stack<int> s;
}
#endif


// ��� �Լ� ���ø��� �ʿ��� ���
#include <iostream>
using namespace std;

template<typename T> class Complex
{
	T real, image;

public:
	Complex(T r, T i) : real(r), image(i) {}

	template<typename U> Complex(const Complex<U>& o) : real(o.real), image(o.image) {}

	template<typename U> friend class Complex;
};


void main()
{
	Complex<int> c2(1, 1);			
	Complex<double> c1 = c2;	// Complex<double> c1(Complex<int>);
}


//
//void main()
//{
//	Complex<double> c1(1.0, 1.0);	// 1+1i
//	Complex<int> c2(1, 1);			// 1+1i
//}
