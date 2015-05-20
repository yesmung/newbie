// �������̽� �̾߱�

#include <iostream>
using namespace std;

#define interface struct

// �ڹٵ� ���߻��.. �ٸ� ���������� ������ ���һ�
// public class Dog extends Animal implements crying, eating, sleeing {}


// ��� �ڵ����� ���Ѿ� �� ��Ģ�� Ŭ������ �����մϴ�.
// C++ ������ �������̽��� �ĺ��ϱ� ���� Ű���带 �������� �ʽ��ϴ�.
// �׷��� ���ξ�� I(Interface)�� ���̴� ���ʰ� �ֽ��ϴ�.
interface IPhone
{
public : 
	// ���� ���� �Լ��� �ڽĿ��� ��� ����� ������ �����ϵ��� �ϱ� ���� ������ ��ġ�Դϴ�.
	virtual void call(const char*) = 0;
};

// ���� class�� �� ���°�... �⺻ public �ε�
/*
struct Phone // �ȵ���̵�� �������̽��� Ŭ������ ����մϴ�.
{
	// ���� ���� �Լ��� �ڽĿ��� ��� ����� ������ �����ϵ��� �ϱ� ���� ������ ��ġ�Դϴ�.
	virtual void call(const char*) = 0;
};
*/

// ���� �ڵ����� �����ϴ� ����� phone�� ��ӹޱ�� ����մϴ�.
class AnyCall : public IPhone
{
public :
	// void send(const char* n) { printf("calling %s with AnyCall\n", n); }
	void call(const char* n) { printf("calling %s with AnyCall\n", n); }
};

class Cyon : public IPhone
{
public :
	void call(const char* n) { printf("calling %s with Cyon\n", n); }
};

class Person
{
public :
	// tightly coupling -> loosely coupling
	
	// ��ü ���� �з����� -> ������ Ŭ�������� ���� !!!�� �ƴ϶� �켱�� ������ ���� �������̽��� ���谡 �ʿ�
	// �������� -> �˰��� �Լ� ����

	// void use_phone(Cyon* c, const char* n) { c->call(n); }
	// void use_phone(AnyCall* c, const char* n) { c->call(n); } 
	void use_phone(IPhone* c, const char* n) { c->call(n); } // upcasting ��

};

void main()
{
	Person p;
	Cyon c;
	p.use_phone(&c, "000-0000-0000");

	AnyCall a;
	p.use_phone(&a, "000-0000-0000");
}