// Ŭ���� ���ø�
#if 0
#include <iostream>

template<typename T> class Stack
{
public :
	// ������
	// Stack<T> {} // 1. ��ǥ��
	// Stack(){} // 2. ǥ��

	// ���� ������
	//Stack(const Stack& s) {}		// 1. Ŭ���� ���ο����� ��� ����
	//Stack(const Stack<T>& s) {}		// 2. ��Ȯ�� ����

	// ��� �Լ�
	// void push(const T& data) {}
	void push(const T& data);

	// �ٸ� Ÿ���� ���ڷ� �ϴ� �Լ�
	// �ݵ�� ��� �Լ� ���ø����� ������ ��
	// void foo(const U& u)
	// template<typename U> void foo(const U& u) {} OK
	template<typename U> void foo(const U& u);
};

// ���⼭ Stack�� Ÿ���� �ƴ϶� ���ø� - ����
// �����Ϸ��� T�� ���� ��, ��, ���ø��� ���� T���� �˷������
// void Stack::push(const T& data)
template<typename T> void Stack<T>::push(const T& data){}

// ��� �Լ� ���ø� �����ϱ�
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
public :
	Complex(T r, T i) : real(r), image(i) { cout << "Complex(T r, T i)" << endl; }
	template<typename U> Complex(const Complex<U>& c) : real(c.real), image(c.image) { cout << "template<typename U> Complex(const Complex<U>& c)" << endl;  }

	// Complex<U> �� �ٸ� Ÿ���̹Ƿ� ����� ������ �ȵ�. ��, private ������ ���� friend ����
	template<typename U> friend class Complex;
};

void main()
{
	// Complex c(1.0, 1.0);
	// Complex<double> c1(1.0, 1.0); // 1+1i
	Complex<int> c2(1, 1); // i+1i
	Complex<double> c1 = c2; // ? ��������� ������ ȣȯ�� �ȵ� // Complex<double> c1(Complex<int>)
}

