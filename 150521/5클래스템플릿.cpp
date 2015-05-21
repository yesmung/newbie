// 클래스 템플릿
#if 0
#include <iostream>

template<typename T> class Stack
{
public :
	// 생성자
	// Stack<T> {} // 1. 비표준
	// Stack(){} // 2. 표준

	// 복사 생성자
	//Stack(const Stack& s) {}		// 1. 클래스 내부에서만 사용 가능
	//Stack(const Stack<T>& s) {}		// 2. 정확한 문법

	// 멤버 함수
	// void push(const T& data) {}
	void push(const T& data);

	// 다른 타입을 인자로 하는 함수
	// 반드시 멤버 함수 템플릿으로 만들어야 함
	// void foo(const U& u)
	// template<typename U> void foo(const U& u) {} OK
	template<typename U> void foo(const U& u);
};

// 여기서 Stack은 타입이 아니라 템플릿 - 에러
// 컴파일러는 T가 뭔지 모름, 즉, 템플릿의 인자 T임을 알려줘야함
// void Stack::push(const T& data)
template<typename T> void Stack<T>::push(const T& data){}

// 멤버 함수 템플릿 정의하기
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
public :
	Complex(T r, T i) : real(r), image(i) { cout << "Complex(T r, T i)" << endl; }
	template<typename U> Complex(const Complex<U>& c) : real(c.real), image(c.image) { cout << "template<typename U> Complex(const Complex<U>& c)" << endl;  }

	// Complex<U> 는 다른 타입이므로 멤버에 접근이 안됨. 즉, private 접근을 위해 friend 선언
	template<typename U> friend class Complex;
};

void main()
{
	// Complex c(1.0, 1.0);
	// Complex<double> c1(1.0, 1.0); // 1+1i
	Complex<int> c2(1, 1); // i+1i
	Complex<double> c1 = c2; // ? 복사생성자 인자의 호환이 안됨 // Complex<double> c1(Complex<int>)
}

