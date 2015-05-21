// 템플릿 이야기

#include <iostream>
using namespace std;

#if 0
// 1. 함수 템플릿
template<typename T> T Max(T a, T b) { return a > b ? a : b; }

// 2. 클래스 템플릿
template<typename T> class Stack
{
	T* arr;
public :
	void push_front(T data) {}
};

void main()
{
	cout << Max<int>(1, 2) << endl; // OK
	cout << Max(1, 2) << endl;		// OK, 함수 템플릿은 암시적 추론이 가능하다.

	// Stack s1;						// ERROR, 클래스 템플릿은 암시적 추론이 불가능하다.
	// s1.push_front(20);

	Stack<int> s;					// OK, 클래스 템플릿은 타입을 명시적으로 사용해야 합니다. Stack은 템플릿이다. 따라서, 아래와 같이 객체 선언 시, 타입 까지 적어줘야 한다.
	Stack<int> s2;					// 변수명(s2) 앞에는 완벽한 타입 정보(Stack<int>)가 와야 된다. 즉, Stack<int>는 고유한 타입니다.
}
#endif

#if 0
// 템플릿 버전
template<typename T> class Stack
{
public :
	void push() { cout << "T" << endl; }
};

// 부분 전문화
template<typename T> class Stack < T* >
{
public :
	void push() { cout << "T*" << endl; }
};

// 전문화 템플릿
template<> class Stack < char * >
{
public :
	void push() { cout << "char*" << endl; }
};


int main()
{
	Stack<int> s1; s1.push();
	Stack<int*> s2; s2.push();
	Stack<char*> s3; s3.push();
}
#endif


// 템플릿 메타 프로그래밍
#include <iostream>
using namespace std;

int fact(int n)
{
	// 재귀 호출은 항상 터미널 조건을 명시해줘야 함
	if (n <= 1) 
		return 1;
	else
		return n * fact(n - 1);
}

// 모든 정수 N에 대해서 처리하는 클래스를 생성합니다.
template<int N> struct Fact
{
	// 컴파일 타임 문법 : iteral, enum
	// 런타임 문법 : symbol
	enum { value = N * Fact<N-1>::value };
};

// N이 1인 경우에 대해서 처리는 전문화를 사용
template<> struct Fact<1>
{
	enum { value = 1 };
};

void main()
{
	cout << fact(5) << endl;
	cout << Fact<5>::value << endl;
}

























