
#if 0
// RTTI: RunTime Type Information
// -  실행 시간에 포인터 변수가 어떤 타입인지를 알아내는 기술
// 조건: 클래스가 최소한 하나 이상의 가상 함수를 포함하고 있어야 함
#include <iostream>
using namespace std;

class Animal { public: virtual ~Animal() {} };
class Dog : public Animal { public:  virtual ~Dog() {} };

int main()
{
	Animal* p1 = new Animal;	// 런타임 문법

	// RTTI를 사용하기 위해 컴파일러가 typeid라는 연산자를 제공합니다.
	// typeid의 리턴 타입을 정확하게 알아두세요 
	const type_info& t1 = typeid(p1);	// typeid(변수명);
	cout << t1.name() << endl;

	Dog* p2 = new Dog;
	const type_info& t2 = typeid(p2);
	cout << t2.name() << endl;

	// 상속 관계
	// Animal* p3 = static_cast<Animal*>new Dog;	// upcasting
	Animal* p3 = new Dog;	// 상속 관계에 있는 경우 캐스팅은 생략 가능
	const type_info& t3 = typeid(p3);
	cout << t3.name() << endl;

	
}
#endif



#include <iostream>
using namespace std;

class Animal { public: virtual ~Animal() {} };
class Dog : public Animal { public:  virtual ~Dog() {} };

int main()
{
	//Animal* p1 = new Animal;	// 런타임 문법

	//const type_info& t1 = typeid(p1);
	//cout << t1.name() << endl;	// Animal*

	//Animal a;
	//const type_info& t2 = typeid(a);	// 포인터 변수와 일반 객체의 변수도 사용 가능
	//cout << t2.name() << endl;	// Animal

	Animal* pAnimal = new Dog;
	const type_info& t3 = typeid(*pAnimal);
	const type_info& t4 = typeid(Dog);	// 타입
//	cout << t3.name() << endl;	// Dog
//	cout << t4.name() << endl;	// Dog
	if (t3 == t4)
		cout << "pAnimal is Dog" << endl;
	else
		cout << "pAnimal is not Dog" << endl;

	if (typeid(*pAnimal) == typeid(Dog))
		cout << "pAnimal is Dog" << endl;
	else
		cout << "pAnimal is not Dog" << endl;

	// dynamic_cast를 사용하면 간단하게 안전한 다운캐스팅을 할 수 있습니다.
	// dynamic_cast를 사용할 경우, 반드시 리턴 타입을 조사해서 사용해야 합니다.
	// 다운 캐스팅에 실패할 경우, 널이 반환되기 때문입니다.
	Dog* pDog = dynamic_cast<Dog*>(pAnimal);
	if (pDog)
		cout << "pAnimal is Dog" << endl;
	else
		cout << "pAnimal is not Dog" << endl;

	


}