
// ��� �Լ� ������ �̾߱�
#include <iostream>
using namespace std;

void func() { printf("calle func()\n"); }

class Clazz
{
public:
	void member_func()	// void member_func(Clazz* const this)
	{
		printf("called member_func()\n");
	}
	
	static void static_func() { printf("called static_func()\n"); }
};

int main()
{
	void(*fp1)() = func;
	void(*fp2)() = &Clazz::static_func;

	// ��� �Լ� �������� ���� ���
	// ����Ÿ��(Ŭ������::*������)([�Ű�����,...]);
	void(Clazz::*fp3)() = &Clazz::member_func;	// ��� �Լ��� �Ͻ������� �����ͷ� �ؼ����� �ʱ�
												// ������ ��������� &�����ڸ� ����ؾ� �մϴ�.

	// ��� �Լ� �����͸� ����Ͽ� �Լ��� ȣ���� ���: .*, ->* �����ڸ� ���
	// .*: ������ �ǿ����ڰ� ��ü�� ���
	// ->*: ������ �ǿ����ڰ� �������� ���
	Clazz obj;
	// obj.fp3();
	// fp3(&obj);	push 0x12ff60
	(obj.*fp3)();

	Clazz* pObj = new Clazz;
	(pObj->*fp3)();
}



//int main()
//{
//	// ������ ������ ���� ���: Ÿ�� * ������;
//	// �Լ� ������ ������ ������ �Լ��� Ÿ���� �ƴ� �ñ״�ó�� ����ؼ� �����ؾ� �մϴ�.
//	// void * fp();	// �Լ��� ����
//
//	// 1. �Լ� ������ ������ ����: ����Ÿ��(*������)([�Ű�����, ...]);
//	void(*fp)();
//
//	// 2. �Լ��� �̸��� �Լ��� �����ͷ� �ؼ���
//	fp = func;
//
//	// 3. �Լ� ȣ�� �����ڸ� ���
//	fp();
//
//}