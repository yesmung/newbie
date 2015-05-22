
#if 0
// ����Ʈ ������ �̾߱�
#include <iostream>
using namespace std;

class Car
{
public:
	Car() { cout << "Car()" << endl; }
	~Car() { cout << "~Car()" << endl; }

	void go() { cout << "go! go! go!" << endl; }
};

void main()
{
	// �� ��ü�� �Ҹ��ڰ� ȣ��Ǵ� ������ delete �����ڸ� ���
	// ��, delete ������ ���� ���� ��� ��ü�� ������ ���� ����
	// -> �޸� ���� ����
	{
		Car* pCar = new Car;
		// delete pCar;
	}	// �޸� ���� �߻�
	getchar();
}

//
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
#endif

// ����Ʈ ������ �̾߱�
#include <iostream>
using namespace std;

class Car {
public:
	Car() { cout << "Car()" << endl; }
	~Car() { cout << "~Car()" << endl; }
	void go() { cout << "go! go! go!" << endl; }
};

// �ڿ� ������ ���� Ŭ������ �����غ��ô�.
// ������ Ÿ���� ó���ϱ� ���� ���ø��� ������ ������ :D

// ����Ʈ �����Ͷ� ������ ��ü�� �ٸ� Ÿ���� ������ ������ �ϴ� ��
template<typename T>
class Sptr {
	T* pObj;
public:
	Sptr(T* p) : pObj(p) {}
	~Sptr() { delete pObj; }

	// ����Ʈ �����ʹ� �Ʒ��� �� �����ڸ� �������ؾ� �մϴ�.
	T* operator->() { return pObj; }
	T& operator*() { return *pObj; }

};

// ����Ʈ �����ʹ� ��¥ �����Ͱ� �ƴ� ��ü�̹Ƿ� ����ڰ� ��ü�� 
// ����/����/����/�Ҹ��� ������ ������ �� �ִ�.
// ��ǥ���� Ȱ�� ��) �Ҹ��ڸ� ����� �ڵ� ����

void main() {
	{
		// Car* pCar = new Car;
		Sptr<Car> pCar = new Car;
		pCar->go();	// pCar.operator->()go(); -> (pCar.operator->())->go();
		(*pCar).go();	// pCar.operator*().go();
	}
	getchar();
}

