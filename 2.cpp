// 컴파일러가 오버로딩 된 함수를 찾는 방법
# include <iostream>
using namespace std;

// void foo(float f) { cout << "foo(float)" << endl; }		// 1. 완전일치 : 타입이 완벽하게 일치된 함수를 찾음
// void foo(double f) { cout << "foo(double)" << endl; }	// 2. 승격(promotion) : 컴파일러가 데이터의 손실이 없는 타입으로 변환
// void foo(int f) { cout << "foo(int)" << endl; }			// 3. 기본 변환 : 기본 타입의 경우, 데이터의 손실이 발생하더라도 변환 후 실행
// void foo(Float f) 6.cpp의 explicit을 참고					// 4. 사용자 정의 변환
// void foo(...) { cout << "foo(...)" << endl; }			// 5. 가변인자

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
	foo(3.14f); // foo(Float(3.14f)) 가 된다... 
}