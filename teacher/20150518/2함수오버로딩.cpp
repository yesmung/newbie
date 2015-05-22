
// 컴파일러가 오버로딩 된 함수를 찾는 방법
#include <iostream>
using namespace std;

class Float
{
public:
	float val;
	explicit Float(float f) : val(f) {}
};

// void foo(float f)  { cout << "foo(float)" << endl; }	    // 1. 완전일치: 타입이 완벽하게 일치된 함수를 찾음
// void foo(double f) { cout << "foo(double)" << endl; }	// 2. 승격: 데이터의 손실이 없는 타입으로 변환
// void foo(int f)    { cout << "foo(int)" << endl; }		// 3. 기본 변환: 기본 타입의 경우, 데이터의 손실이 발생하더라도 변환 후 실행
void foo(Float f)     { cout << "foo(Float)" << endl; }      // 4. 사용자 정의 변환:
// void foo(...)      { cout << "foo(...)" << endl; }		// 5. 가변 인자

int main()
{
	foo(3.14f);	// ?
}
