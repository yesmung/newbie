// ����Ʈ ������ �̾߱�
#include <iostream>
using namespace std;

class Car
{
public :
	Car() { cout << "Car()" << endl; }
	~Car() { cout << "~Car()" << endl; }

	void go() { cout << "go! go! go!" << endl; }
};

// �ڿ� ������ ���� Ŭ������ �����غ��ô�.
// ������ Ÿ���� ó���ϱ� ���� ���ø��� �����մϴ�.
// ����Ʈ �����Ͷ� ������ ��ü�� �ٸ� Ÿ���� ������ ������ �ϴ� ��
template <typename T>
class Sptr
{
	T* pObj;
public :
	Sptr(T* p) : pObj(p) {}
	~Sptr() { delete pObj;  }

	// ����Ʈ �����ʹ� �Ʒ��� �� �����ڸ� �������ؾ� �մϴ�.
	T* operator->() { return pObj; }
	T& operator*() { return *pObj; } // Car ���� ��, �ӽ� ��ü�� �����ǹǷ� ������ �����Ѵ�. ���⼭�� RVO �� �ƴ� ������ ����
};

// ����Ʈ �����ʹ� ��¥ �����Ͱ� �ƴ� ��ü�̹Ƿ� ����ڰ� ��ü��
// ����/����/����/�Ҹ��� ������ ������ �� �ִ�.
// ��ǥ���� Ȱ�� ��) �Ҹ��ڸ� ����� �ڵ�����

void main()
{
	{
		Sptr<Car> pCar = new Car;
		pCar->go();	// pCar.operator->()go; (x) �Լ� ȣ�� () �� ������ ; �;ߵǳ�, ��ū�� �پ���.
					// �ᱹ �̷��� �����Ϸ��� �ٲ�, (pCar.operator->())->go(); 

		(*pCar).go();	// ������ �����ڵ� �����ؾ���
						// pCar.operator*().go()

		Sptr<int> pInt = new int;
		*pInt = 10;
		cout << "pInt = " << *pInt << endl;
	}
	getchar();
}


//void main()
//{
//	// �� ��ü�� �Ҹ��ڰ� ȣ��Ǵ� ������ delete �����ڸ� ����� ��
//	// ��, delete ������ ���� ���� ��� ��ü�� ������ ���� ����
//	// -> �޸� ���� ����
//	{
//		Car* pCar = new Car;
//		// delete pCar;
//		// �޸� ���� �߻�
//	}
//	getchar();
//}

//void main()
//{
//	// ���� ������ ����
//	{
//		Car c;	// ���� ��ü and �ڵ� ����
//				// ���� ��ü�� �޸𸮰� �ڵ����� �����ǰ� �����ڰ� ȣ��
//				// ���� ��ü�� �Ҹ��ڴ� ��ü�� ���ÿ��� �ı��� �� ȣ��
//	}
//	getchar();
//}