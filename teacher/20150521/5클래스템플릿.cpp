
#if 0
// 클래스 템플릿
#include <iostream>

template<typename T> class Stack
{
public:
	// 생성자
	// Stack<T>() {}	// 1. 비표준
	// Stack() {}		// 2. 표준

	// 복사 생성자
	//Stack(const Stack& s) {}		// 1. 클래스 내부에서만 사용
	//Stack(const Stack<T>& s); // {}		// 2. 정확한 문법

	// 멤버 함수
	void push(const T& data);

	// 다른 타입을 인자로 하는 함수는 멤버 함수 템플릿으로 선언
	template<typename U> void foo(const U& u);

};
// 멤버 함수의 외부 정의
template<typename T> void Stack<T>::push(const T& data) {}

// 멤버 함수 템플릿의 정의를 구현
template<typename T> template<typename U> void Stack<T>::foo(const U& u) {}
void main()
{
	Stack<int> s;
}
#endif


// 멤버 함수 템플릿이 필요한 경우
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
