// 템플릿 이야기
#include <iostream>
using namespace std;

#if 0
// int Max(int a, int b) { return a > b ? a : b; }
// doueble Max(double a, double b) { return a > b ? a : b; }
#define MAKE_MAX(T)	T Max(T a, T b) { return a > b ? a : b; }

// 함수 템플릿 : 함수의 기계어 코드를 컴파일러가 생성해내는 문법
template<typename T>	T Max(T a, T b) { return a > b ? a : b; } // 컴파일러.. 디버거 사용 가능

MAKE_MAX(int)
MAKE_MAX(double)

void main()
{
	cout << Max(1, 2) << endl; 
	cout << Max(1.1, 2.2) << endl;
}
#endif

// #define MAKE_MAX(T)	T Max(T a, T b) { return a > b ? a : b; }
// MAKE_MAX(int)
template<typename T>	T Max(T a, T b) { return a > b ? a : b; } // 컴파일러.. 디버거 사용 가능, 암시적 추론

void main()
{
	float f = 3.14;
	cout << Max<int>(1, 'A') << endl;
	//cout << Max(1.1, 2.2) << endl;

	char* p = static_cast<char *>(malloc(sizeof(char))); // static_cast 등은 컴파일러가 제공하는 연산자..이기 전에 근본적으로는 함수 템플릿이다. ()가 있지 않은가
	// interface_cat 란게 있다 .
}

// 컴파일러가 제공하는 캐스팅 연산자는 연산자이기 전에
// 근복적으로 함수 템플릿 입니다.

// 안드로이드에서는 바인더 타입을 인터페이스 타입으로 변경할 때,
// 내부 구현을 감추기 위해 interface_cat(함수 템플릿)을 제공합니다.

sp<IBinder> binder = defaultServiceManager()->getService("LedService");
// sp<ILedService> ledService = ILedService::asInterface(binder);
sp<ILedService> ledService = interface_cast<ILedService>(binder); // 구글개발자가 요롷게 바꿈.. interface_cast를 통해 원하는 타입(ILedService)를 받을 수 있다.
http://androidxref.com/5.1.0_r1/xref/frameworks/native/include/binder/IInterface.h