
#include <iostream>
using namespace std;

template<typename T> class Sptr {
	T* pObj;
	int* ref;
public:
	Sptr(T* p) : pObj(p), ref(new int(1)) {}
	Sptr(const Sptr& o) : pObj(o.pObj), ref(o.ref) { ++(*ref); }
	template<typename U>Sptr(const Sptr<U>& o) : pObj(o.pObj), ref(o.ref) {}

	template<typename U> friend class Sptr;

	~Sptr() {
		if (--(*ref) == 0)
		{
			delete pObj;
			delete ref;
		}
	}

	T* operator->() { return pObj; }
	T& operator*() { return *pObj; }
};

class Animal 
{
public :
	virtual void cry() = 0;
};
class Dog : public Animal 
{
public :
	void cry() { cout << "Dog::cry()" << endl; }
};

void main()
{
	Animal* p1 = new Dog;		// ok.. 자동으로 업캐스팅해줌
	p1->cry();

	Sptr<Animal> p2 = new Dog;	// ok, // but. 스마트포인터 = 진짜 포인터..

	Sptr<Dog> p3 = new Dog;
	Sptr<Animal> p4 = p3; // 복사생성자가 호출됨, 즉, 다른 타입도 복사되도록.. 멤버함수템플릿사용한 복사생성자필요!
	p4->cry();
}
