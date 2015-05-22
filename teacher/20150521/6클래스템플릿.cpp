
#include <iostream>
using namespace std;

template<typename T> class Sptr {
	T* pObj;
	int* ref;
public:
	Sptr(T* p) : pObj(p), ref(new int(1)) {}
	Sptr(const Sptr& o) : pObj(o.pObj), ref(o.ref) { ++(*ref); }

	~Sptr() {
		if (--(*ref) == 0)
		{
			delete pObj;
			delete ref;
		}
	}
	T* operator->() { return pObj; }
	T& operator*() { return *pObj; }

	template<typename U> Sptr(const Sptr<U>& o) : pObj(o.pObj), ref(o.ref) { ++(*ref); }
	template<typename U> friend class Sptr;
};

class Animal
{
public:
	virtual void cry() = 0;
};
class Dog : public Animal
{
public:
	void cry() { cout << "Dog::cry()" << endl; }
};

void main()
{
	//Animal* p1 = new Dog;		// OK
	//Sptr<Animal> p2 = new Dog;	// OK

	//Animal* p1 = new Dog;		// OK
	//p1->cry();
	//getchar();

	Sptr<Dog> p3 = new Dog;
	// 아래의 코드가 수행될 수 있도록 스마트 포인터를 변경해 보세요 ;D
	Sptr<Animal> p4 = p3;
	p4->cry();
}
