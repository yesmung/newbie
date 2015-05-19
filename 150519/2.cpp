// �̱��� �̾߱�
#include <iostream>
using namespace std;

class Cursor
{
	// 1. ����ڷκ��� ��ü�� ���� ������ ���� ���Ͽ� �����ڸ� private ������ �����ؾ� �Ѵ�.
	Cursor() {}
	static Cursor* pInstance;
	// Cursor(const Cursor& c) {}
	// Cursor& operator=(const Cursor& c) {}
	
	// ���� ���� ���� �ذ�
	// Cursor(const Cursor& c);
	// Cursor& operator=(const Cursor& c);

	// C++11���� ���ο� ���� ���� (Ÿ���� ���⿡ �̰��� SINGLETON���� �� �� �ִ�)
	Cursor(const Cursor& c) = delete;
	Cursor& operator=(const Cursor& c) = delete;

public :
	// 2. ������ ��ü�� ��ȯ�ϱ� ���� ���� �������̽��� �����ؾ� �Ѵ�.
	// ��, namespace �ȿ� ���� �ִ� ���� �Լ��̴�.
	static Cursor& getInstance() // �����͸����ϸ�, �ٸ� ������ delete�� ���輺�� �ִ�. �׷��� ��������
	{
		// static Cursor cursor; // getInstance scope������ ���� ������ ���� ����
		// return cursor;
		
		// static Cursor* pInstance; �� �°� ����

		if (pInstance == 0)
		{
			pInstance = new Cursor;
		}
		return *pInstance;
	}
	
	// ���ο��� ���� ����..������
	// ������ Ÿ�ӿ��� ���� �� ������, ��ŷŸ�ӿ� ���� �� �ִ�. ��, ��ŷ..���ε��� ��, ������ �߻���Ű�� �ǰڴ�.
	// ��, ���ε��� ���� �ϴ� �ڵ带 ������... 
	// �ٽ� ����, private�� ������ �͵��� ���� �ع�����.
	//void foo()
	//{
	//	Cursor c;
	//	Cursor c2 = c;
	//	Cursor c3;
	//	c3 = c;
	//}
};
Cursor* Cursor::pInstance = 0;


int main()
{
	// Cursor c1;
	// Cursor c2;
    // ���⼭�� android singleton pattern�� �����Ѵ�. - ����, heap�� ����.
	Cursor& c1 = Cursor::getInstance(); // Cursor.getInstance()
	Cursor& c2 = Cursor::getInstance(); 

	// ���� �ڵ��� ����!!!!
	// Cursor c3 = c1;
	// cout << "&c3 = " << &c3 << endl;

	// ��ü��
	// ����� ���� �ʱ�ȭ -> ���� ������ ȣ���
	// ���� �� �ʱ�ȭ -> ���� �����ڰ� ȣ���
	// ��, ����, ���� �����ڰ� ȣ����� �ʵ��� ���ƾ� �� -> private ������ �ξ�� ��!!!

	cout << "&c1 = " << &c1 << endl;
	cout << "&c2 = " << &c2 << endl;

	// c1.foo();
}

// Singleton �� ����� ��� (�ڵ�ȭ)
// 1. META CODE (macro) 
// 2. CRTP (templete)
// => 3.cpp