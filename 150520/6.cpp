// ���ø� �̾߱�
#include <iostream>
using namespace std;

#if 0
// int Max(int a, int b) { return a > b ? a : b; }
// doueble Max(double a, double b) { return a > b ? a : b; }
#define MAKE_MAX(T)	T Max(T a, T b) { return a > b ? a : b; }

// �Լ� ���ø� : �Լ��� ���� �ڵ带 �����Ϸ��� �����س��� ����
template<typename T>	T Max(T a, T b) { return a > b ? a : b; } // �����Ϸ�.. ����� ��� ����

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
template<typename T>	T Max(T a, T b) { return a > b ? a : b; } // �����Ϸ�.. ����� ��� ����, �Ͻ��� �߷�

void main()
{
	float f = 3.14;
	cout << Max<int>(1, 'A') << endl;
	//cout << Max(1.1, 2.2) << endl;

	char* p = static_cast<char *>(malloc(sizeof(char))); // static_cast ���� �����Ϸ��� �����ϴ� ������..�̱� ���� �ٺ������δ� �Լ� ���ø��̴�. ()�� ���� ������
	// interface_cat ���� �ִ� .
}

// �����Ϸ��� �����ϴ� ĳ���� �����ڴ� �������̱� ����
// �ٺ������� �Լ� ���ø� �Դϴ�.

// �ȵ���̵忡���� ���δ� Ÿ���� �������̽� Ÿ������ ������ ��,
// ���� ������ ���߱� ���� interface_cat(�Լ� ���ø�)�� �����մϴ�.

sp<IBinder> binder = defaultServiceManager()->getService("LedService");
// sp<ILedService> ledService = ILedService::asInterface(binder);
sp<ILedService> ledService = interface_cast<ILedService>(binder); // ���۰����ڰ� ���� �ٲ�.. interface_cast�� ���� ���ϴ� Ÿ��(ILedService)�� ���� �� �ִ�.
http://androidxref.com/5.1.0_r1/xref/frameworks/native/include/binder/IInterface.h