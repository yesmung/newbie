
#if 0
// 템플릿 이야기
#include <iostream>
using namespace std;

//int Max(int a, int b) { return a > b ? a : b; }
//double Max(double a, double b) { return a > b ? a : b; }
#define MAKE_MAX(T)		T Max(T a, T b) { return a > b ? a : b; }

// 함수 템플릿: 함수의 기계어 코드를 컴파일러가 생성해내는 문법
template<typename T>	T Max(T a, T b) { return a > b ? a : b; }

void main()
{
	cout << Max(1, 2) << endl;			// 2
	cout << Max(1.1, 2.2) << endl;		// 2.2
}
#endif


#include <iostream>
#include <cstdlib>
using namespace std;

#define MAKE_MAX(T)		T Max(T a, T b) { return a > b ? a : b; }
// MAKE_MAX(int)

template<typename T>	T Max(T a, T b) { return a > b ? a : b; }

void main()
{
	cout << Max<int>(1, 'A') << endl;			// 
	// cout << Max(1.1, 2.2) << endl;		// 

	char* p = static_cast<char*>(malloc(sizeof(char)));
}

// 컴파일러가 제공하는 캐스팅 연산자는 연산자이기 전에
// 근본적으로는 함수 템플릿 입니다.

// 안드로이드에서는 바인더 타입을 인터페이스 타입으로 변경할 때,
// 내부 구현을 감추기 위해 interface_cast(함수 템플릿)을 제공합니다.


sp<IBinder> binder = defaultServiceManager()->getService("LedService");
// sp<ILedService> ledService = ILedService::asInterface(binder);
sp<ILedService> ledService = interface_cast<ILedService>(binder);