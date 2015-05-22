
#if 0
#include <iostream>
using namespace std;

// ���� ����� ���ü�� �����ϵ��� �ڵ带 �����غ��ϴ�.
class Car
{
	int mCount;
public:
	// ���� ����� �����ϱ� ���� �������̽��� ����
	void incStrong() { ++mCount; }
	void decStrong() { if(--mCount == 0) delete this; }

	Car() : mCount(0) { cout << "Car()" << endl; }
	~Car() { cout << "~Car()" << endl; }
	void go() { cout << "go!" << endl; }
};

void main()
{
	Car* p1 = new Car;	// ��ü ���� ��
	p1->incStrong();	// ���� ��� ����

	Car* p2 = p1;		// ��ü ���� ��
	p2->incStrong();	// ���� ��� ����

	// ��ü�� ����� ���� ���
	p2->decStrong();	// ���� ��� ����
	p1->decStrong();	// ���� ��� ���� ��, �Ҹ�
}
// ��ü�� ���� ����� ������ ����Ʈ �����Ϳ��� �����մϴ�.

#endif


#if 0
#include <iostream>
using namespace std;

// ���� ����� ���ü�� �����ϵ��� �ڵ带 �����غ��ϴ�.
class Car
{
	int mCount;
public:
	// ���� ����� �����ϱ� ���� �������̽��� ����
	void incStrong() { ++mCount; }
	void decStrong() { if (--mCount == 0) delete this; }

	Car() : mCount(0) { cout << "Car()" << endl; }
	~Car() { cout << "~Car()" << endl; }
	void go() { cout << "go!" << endl; }
};

template<typename T> class Sptr
{
	T* pObj;
public:
	Sptr(T* p) : pObj(p) { if (pObj) pObj->incStrong();  }
	Sptr(const Sptr& o) : pObj(o.pObj) { if (pObj) pObj->incStrong(); }
	~Sptr() { if (pObj) pObj->decStrong(); }
	
	T* operator->() { return pObj; }
	T& operator*() { return *pObj; }
};

// �Ʒ��� �ڵ尡 �����ϰ� ����� �� �ֵ��� ����Ʈ �����͸�
// ������ ������ ;D
void main()
{
	Sptr<Car> p1 = new Car;
	Sptr<Car> p2 = p1;

	cout << sizeof(Sptr<Car>) << endl;
}

#endif



#include <iostream>
using namespace std;

// ���� ����� ���õ� �ڵ带 �ڵ����� ������ �� �ֵ���
// �θ� Ŭ������ �����մϴ�.
class RefBase
{
	int mCount;
public:
	RefBase() : mCount(0) {}
	virtual ~RefBase() {}
	void incStrong() { ++mCount; }
	void decStrong() { if (--mCount == 0) delete this; }
};

class Car : public RefBase
{
public:
	Car() { cout << "Car()" << endl; }
	~Car() { cout << "~Car()" << endl; }
	void go() { cout << "go!" << endl; }
};

template<typename T> class Sptr
{
	T* pObj;
public:
	Sptr(T* p) : pObj(p) { if (pObj) pObj->incStrong(); }
	Sptr(const Sptr& o) : pObj(o.pObj) { if (pObj) pObj->incStrong(); }
	~Sptr() { if (pObj) pObj->decStrong(); }

	T* operator->() { return pObj; }
	T& operator*() { return *pObj; }
};

void main()
{
	Sptr<Car> p1 = new Car;
	Sptr<Car> p2 = p1;
}

