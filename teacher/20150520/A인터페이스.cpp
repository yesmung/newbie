
// �������̽� �̾߱�
#include <iostream>
using namespace std;

// public class Dog extends Animal implements crying, eating, sleeping {}

#define interface struct
// ��� �ڵ����� ���Ѿ� �� ��Ģ�� Ŭ������ �����մϴ�.
// C++ ������ �������̽��� �ĺ��ϱ� ���� Ű���带 �������� �ʽ��ϴ�.
// �׷��� ���ξ�� I(Interface)�� ���̴� ���ʰ� �ֽ��ϴ�.
interface IPhone	// �ȵ���̵�� �������̽��� Ŭ������ ����մϴ�.
{
	// ���� ���� �Լ��� �ڽĿ��� ��� ����� ������ �����ϵ���
	// �ϱ� ���� �������� ��ġ�Դϴ�.
	virtual void call(const char*) = 0;
};

// ���� �ڵ����� �����ϴ� ����� Phone�� ��ӹޱ�� ����մϴ�.
class AnyCall : public IPhone
{
public:
	void call(const char* n) { printf("calling %s with Anycall\n", n); }
};

class Cyon : public IPhone
{
public:
	void call(const char* n) { printf("calling %s with Cyon\n", n); }
};

class Person
{
public:
	void use_phone(IPhone* c, const char* n) { c->call(n); }
	// void use_phone(AnyCall* c, const char* n) { c->call(n); }

};

void main()
{
	Person p;

	Cyon c;
	p.use_phone(&c, "000-0000-0000");

	AnyCall a;
	p.use_phone(&a, "000-0000-0000");

}