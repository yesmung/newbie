
#if 0
// 스마트 포인터 이야기
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
	// 힙 객체의 소멸자가 호출되는 시점은 delete 연산자를 사용
	// 즉, delete 연산을 하지 않을 경우 객체는 여전히 힙에 존재
	// -> 메모리 누수 현상
	{
		Car* pCar = new Car;
		// delete pCar;
	}	// 메모리 누수 발생
	getchar();
}

//
//void main()
//{
//	// 스택 프레임 생성
//	{
//		Car c;	// 지역 객체 and 자동 변수
//				// 지역 객체는 메모리가 자동으로 생성되고 생성자가 호출
//				// 지역 객체의 소멸자는 객체가 스택에서 파괴될 때 호출
//	}
//	getchar();
//}
#endif

// 스마트 포인터 이야기
#include <iostream>
using namespace std;

class Car {
public:
	Car() { cout << "Car()" << endl; }
	~Car() { cout << "~Car()" << endl; }
	void go() { cout << "go! go! go!" << endl; }
};

// 자원 관리를 위한 클래스를 도입해봅시다.
// 임의의 타입을 처리하기 위해 템플릿을 도입해 보세여 :D

// 스마트 포인터란 임의의 객체가 다른 타입의 포인터 역할을 하는 것
template<typename T>
class Sptr {
	T* pObj;
public:
	Sptr(T* p) : pObj(p) {}
	~Sptr() { delete pObj; }

	// 스마트 포인터는 아래의 두 연산자를 재정의해야 합니다.
	T* operator->() { return pObj; }
	T& operator*() { return *pObj; }

};

// 스마트 포인터는 진짜 포인터가 아닌 객체이므로 사용자가 객체의 
// 생성/복사/대입/소멸의 과정을 제어할 수 있다.
// 대표적인 활용 예) 소멸자를 사용한 자동 삭제

void main() {
	{
		// Car* pCar = new Car;
		Sptr<Car> pCar = new Car;
		pCar->go();	// pCar.operator->()go(); -> (pCar.operator->())->go();
		(*pCar).go();	// pCar.operator*().go();
	}
	getchar();
}

