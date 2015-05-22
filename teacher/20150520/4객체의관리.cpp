
#if 0
#include <iostream>
using namespace std;

// 참조 계수를 대상체가 관리하도록 코드를 구현해봅니다.
class Car
{
	int mCount;
public:
	// 참조 계수를 관리하기 위한 인터페이스를 제공
	void incStrong() { ++mCount; }
	void decStrong() { if(--mCount == 0) delete this; }

	Car() : mCount(0) { cout << "Car()" << endl; }
	~Car() { cout << "~Car()" << endl; }
	void go() { cout << "go!" << endl; }
};

void main()
{
	Car* p1 = new Car;	// 객체 생성 후
	p1->incStrong();	// 참조 계수 증가

	Car* p2 = p1;		// 객체 복사 후
	p2->incStrong();	// 참조 계수 증가

	// 객체의 사용이 끝난 경우
	p2->decStrong();	// 참조 계수 감소
	p1->decStrong();	// 참조 계수 감소 후, 소멸
}
// 객체의 참조 계수의 관리를 스마트 포인터에게 위임합니다.

#endif


#if 0
#include <iostream>
using namespace std;

// 참조 계수를 대상체가 관리하도록 코드를 구현해봅니다.
class Car
{
	int mCount;
public:
	// 참조 계수를 관리하기 위한 인터페이스를 제공
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

// 아래의 코드가 안전하게 종료될 수 있도록 스마트 포인터를
// 변경해 보세여 ;D
void main()
{
	Sptr<Car> p1 = new Car;
	Sptr<Car> p2 = p1;

	cout << sizeof(Sptr<Car>) << endl;
}

#endif



#include <iostream>
using namespace std;

// 참조 계수와 관련된 코드를 자동으로 생성할 수 있도록
// 부모 클래스를 설계합니다.
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

