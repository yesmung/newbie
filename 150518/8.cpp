// ��� �Լ� ������ �̾߱�
#include <iostream>
using namespace std;

void func() { printf("called func()"); }

class Clazz
{
public :
	void member_func() { printf("called member_func()\n"); } // void member_func(Clazz* const this)
	static void static_func() { printf("called static_func()\n"); }
};


int main()
{
	void(*fp1)() = func;
	void(*fp2)() = &Clazz::static_func; // Clazz��� namespace�� �ִ�.
	// & �Ⱥٿ��� ������, ��Ģ�� �ٿ��� �ȴ�.
	// Ŭ���� ���ʿ� �ִ� ����Լ��̱⶧����, �Ͻ��� ��ȯ�� �ȵǳ�
	// c������ ����ȣȯ�� ������ ����Ǵ� ���̴�.
	
	fp2();

	// ��� �Լ� �������� ���� ���
	// void(*fp3)() = &Clazz::member_func; ERROR
	// ����Ÿ��(Ŭ������::*������)([�Ű�����,...]);

	void(Clazz::*fp3)() = &Clazz::member_func; // ��� �Լ��� �Ͻ������� �����ͷ� �ؼ����� �ʱ� ������
												// ���������� &�����ڸ� ����ؾ� �Ѵ�.
	
	// ��� �Լ� �����͸� ����Ͽ� �Լ��� ȣ���� ��� : .*, ->* �����ڸ� ���
	// .* : ������ �ǿ����ڰ� ��ü�� ���
	// ->* : ������ �ǿ����ڰ� �������� ���
	Clazz obj; // ����
	// obj.fp3() ERROR
	// fp3(&obj); ERROR, ��ü�� �ּҴ� ���뷹�����ͷ� ��
	(obj.*fp3)(); // .* ������ ����

	// �ǿ����ڰ� �������� ���(�� ����)
	Clazz* pObj = new Clazz;
	// (pObj.*fp3)();
	(pObj->*fp3)();
}

/*
int main()
{
	// ������ ������ ���� ��� : Ÿ�� * ������
	// �Լ� ������ ������ ������ �Լ��� Ÿ���� �ƴ�, �ñ״�ó�� ����ؼ� �����ؾ� �Ѵ�.
	// �Լ��� �ñ״�ó : ����Ÿ�� + �Ű�����
	// �ñ״�ó * ������

	// void() * fp;
	// void * fp() = func; // ERROR, �Լ� �����͸� ������ ���� �ƴ϶�, �Լ��� ������ ���̴�...
	// �����Ϸ��� �ؼ��� ��, �ɺ��� �ں��� �ؼ��ϱ� ������, ()�� �ؼ��ϸ�, �Լ���� �����ϰ� �ȴ�.
	// ����, �Ʒ��� ���� �Լ� �����͸� ������ �� �ִ�.
	// ��ȣ�� �켱���� ������ ����, (*)�� ���
	
	// 1. �Լ� ������ ������ ���� : ����Ÿ��(*������)([�Ű�����,...]);
	void(*fp)(); 

	// 2. �Լ��� �̸��� �Լ��� �����ͷ� �ؼ���
	// call by address : array, function
	fp = func;

	// 3. �Լ��� ȣ���ϱ� ���� �����ڸ� ���
	fp();
}
*/