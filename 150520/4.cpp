#include <iostream>
using namespace std;

#if 0
// ���� ����� ���ü�� �����ϵ��� �ڵ带 �����غ��ô�.
class Car
{
	int mCount;
public :
	// ���� ����� �����ϱ� ���� �������̽��� ����
	void incStrong() { ++mCount;  }
	void decStrong() { if(--mCount==0) delete this;  }

	Car() : mCount(0) { cout << "Car()" << endl; }
	~Car() { cout << "~Car()" << endl; }
	void go() { cout << "go!" << endl; }
};

void main()
{
	Car* p1 = new Car; // ��ü ���� ��, ���� ��� ����
	p1->incStrong();
	Car* p2 = p1; // ��ü ���� ��, ���� ��� ����
	p2->incStrong();

	// ��ü�� ����� ���� ���
	p2->decStrong();	// ���� ��� ����
	p1->decStrong();	// ���� ��� ���� ��, �Ҹ�
}
#endif


#if 0
template <typename T>
class Sptr
{
	T* pObj;
public:
	Sptr(T* p) : pObj(p) { if (pObj) pObj->incStrong();  }
	~Sptr() { if (pObj) pObj->decStrong(); }
	Sptr(const T& c) : pObj(c.pObj)	{ if (pObj) pObj->incStrong();  }

	T* operator->() { return pObj; }
	T& operator*() { return *pObj; }
};


// ���� ����� ���ü�� �����ϵ��� �ڵ带 �����غ��ô�.
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

void main()
{
	Sptr<Car> p1 = new Car;
	Sptr<Car> p2 = p1;

	cout << sizeof(Sptr<Car>) << endl;
}
#endif

// ���� ����� ���õ� �ڵ带 �ڵ����� ������ �� �ֵ���
// �θ� Ŭ������ �����մϴ�.

class RefBase
{
	int mCount;
public:
	RefBase() : mCount(0){}
	virtual ~RefBase(){}
	void incStrong() { ++mCount; }
	void decStrong() { if (--mCount == 0) delete this; }
};

template <typename T>
class Sptr
{
	T* pObj;
public:
	Sptr(T* p) : pObj(p) { if (pObj) pObj->incStrong(); }
	~Sptr() { if (pObj) pObj->decStrong(); }
	Sptr(const T& c) : pObj(c.pObj)	{ if (pObj) pObj->incStrong(); }

	T* operator->() { return pObj; }
	T& operator*() { return *pObj; }
};

class Car : public RefBase // p57
{
public:
	Car() { cout << "Car()" << endl; }
	~Car() { cout << "~Car()" << endl; }
	void go() { cout << "go!" << endl; }
};

void main()
{
	Sptr<Car> p1 = new Car;
	Sptr<Car> p2 = p1;

	cout << sizeof(Sptr<Car>) << endl;
}