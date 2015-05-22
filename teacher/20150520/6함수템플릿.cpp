
#if 0
// ���ø� �̾߱�
#include <iostream>
using namespace std;

//int Max(int a, int b) { return a > b ? a : b; }
//double Max(double a, double b) { return a > b ? a : b; }
#define MAKE_MAX(T)		T Max(T a, T b) { return a > b ? a : b; }

// �Լ� ���ø�: �Լ��� ���� �ڵ带 �����Ϸ��� �����س��� ����
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

// �����Ϸ��� �����ϴ� ĳ���� �����ڴ� �������̱� ����
// �ٺ������δ� �Լ� ���ø� �Դϴ�.

// �ȵ���̵忡���� ���δ� Ÿ���� �������̽� Ÿ������ ������ ��,
// ���� ������ ���߱� ���� interface_cast(�Լ� ���ø�)�� �����մϴ�.


sp<IBinder> binder = defaultServiceManager()->getService("LedService");
// sp<ILedService> ledService = ILedService::asInterface(binder);
sp<ILedService> ledService = interface_cast<ILedService>(binder);