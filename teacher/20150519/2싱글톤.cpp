
// �̱��� �̾߱�
#include <iostream>
using namespace std;

class Cursor
{
	// 1. ����ڷκ��� ��ü�� ���� ������ ���� ���Ͽ� �����ڸ�
	// private ������ �����ؾ� �մϴ�.
	Cursor() {}

	// 3. ����� ������ �����ϱ� ���� ���� �����ڿ� ���� ������ �Լ���
	// private ������ �����ؾ� �մϴ�.
	Cursor(const Cursor&);			// = delete;
	void operator=(const Cursor&);	// = delete;
	static Cursor* pInstance;
	
public:
	// 2. ������ ��ü�� ��ȯ�ϱ� ���� ���� �������̽��� �����ؾ�
	// �մϴ�.
	static Cursor& getInstance()
	{
		if (pInstance == 0)
			pInstance = new Cursor;
		return *pInstance;
	}
};
Cursor* Cursor::pInstance = 0;

int main()
{
	Cursor& c1 = Cursor::getInstance();	// Cursor.getInstance();
	Cursor& c2 = Cursor::getInstance();
	
	cout << "&c1 = " << &c1 << endl;
	cout << "&c2 = " << &c2 << endl;

	//Cursor c3 = c1;
	//cout << "&c3 = " << &c3 << endl;
}