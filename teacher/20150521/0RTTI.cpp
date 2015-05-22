
#if 0
// RTTI: RunTime Type Information
// -  ���� �ð��� ������ ������ � Ÿ�������� �˾Ƴ��� ���
// ����: Ŭ������ �ּ��� �ϳ� �̻��� ���� �Լ��� �����ϰ� �־�� ��
#include <iostream>
using namespace std;

class Animal { public: virtual ~Animal() {} };
class Dog : public Animal { public:  virtual ~Dog() {} };

int main()
{
	Animal* p1 = new Animal;	// ��Ÿ�� ����

	// RTTI�� ����ϱ� ���� �����Ϸ��� typeid��� �����ڸ� �����մϴ�.
	// typeid�� ���� Ÿ���� ��Ȯ�ϰ� �˾Ƶμ��� 
	const type_info& t1 = typeid(p1);	// typeid(������);
	cout << t1.name() << endl;

	Dog* p2 = new Dog;
	const type_info& t2 = typeid(p2);
	cout << t2.name() << endl;

	// ��� ����
	// Animal* p3 = static_cast<Animal*>new Dog;	// upcasting
	Animal* p3 = new Dog;	// ��� ���迡 �ִ� ��� ĳ������ ���� ����
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
	//Animal* p1 = new Animal;	// ��Ÿ�� ����

	//const type_info& t1 = typeid(p1);
	//cout << t1.name() << endl;	// Animal*

	//Animal a;
	//const type_info& t2 = typeid(a);	// ������ ������ �Ϲ� ��ü�� ������ ��� ����
	//cout << t2.name() << endl;	// Animal

	Animal* pAnimal = new Dog;
	const type_info& t3 = typeid(*pAnimal);
	const type_info& t4 = typeid(Dog);	// Ÿ��
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

	// dynamic_cast�� ����ϸ� �����ϰ� ������ �ٿ�ĳ������ �� �� �ֽ��ϴ�.
	// dynamic_cast�� ����� ���, �ݵ�� ���� Ÿ���� �����ؼ� ����ؾ� �մϴ�.
	// �ٿ� ĳ���ÿ� ������ ���, ���� ��ȯ�Ǳ� �����Դϴ�.
	Dog* pDog = dynamic_cast<Dog*>(pAnimal);
	if (pDog)
		cout << "pAnimal is Dog" << endl;
	else
		cout << "pAnimal is not Dog" << endl;

	


}