
#if 0
// trivial: �ڸ��ϴ� -> �ƹ� �ϵ� ���� �ʴ´�.
#include <iostream>
#include <cstdlib>
using namespace std;

// trivial ������: �ڸ��� ������ -> �ƹ��� �ϵ� ���� �ʴ� ������
// 1. �θ� ���ų� �θ��� �����ڰ� �ڸ��ϰų�
// 2. ��ü�� ����� ���ų� ��ü�� ����� �����ڰ� �ڸ��ϰų�
// 3. ���� �Լ��� ���ų�
// 4. ����ڰ� ���� �����ڰ� ���ٸ�
// �����ڰ� �ڸ��ϴٶ�� �� �� �ִ�.

class Test
{
public:
	virtual void foo() {}
	Test() {}
};

int main()
{
	Test* p = static_cast<Test*>(malloc(sizeof(Test)));
	p->foo();
}
#endif


#include <cstring>
#include <iostream>
using namespace std;

// ��� Ÿ���� ���� �����ڴ� �ڸ����� �ʴ�
template<typename T> struct trait_trivial_copy { enum { value = false }; };
template<typename T> struct trait_trivial_ctor { enum { value = false }; };
template<typename T> struct trait_trivial_dtor { enum { value = false }; };

// �ٸ� �⺻ Ÿ���� �ڸ��ϴ�
#define BASIC_TYPE_TRAITS(T)		\
	template<> struct trait_trivial_copy<T> { enum { value = true }; };		\
	template<> struct trait_trivial_ctor<T> { enum { value = true }; };		\
	template<> struct trait_trivial_dtor<T> { enum { value = true }; };

BASIC_TYPE_TRAITS(char)
BASIC_TYPE_TRAITS(short)
BASIC_TYPE_TRAITS(int)
BASIC_TYPE_TRAITS(long)
BASIC_TYPE_TRAITS(float)
BASIC_TYPE_TRAITS(double)

// androidxref.com -> copy_type �˻�


struct Point
{
	int x, y;
};

BASIC_TYPE_TRAITS(Point)

template<typename T> void copy_type(T* dst, T* src, size_t sz)
{
	if (trait_trivial_copy<T>::value == false)	// ���� �����ڰ� �ڸ����� �����Ƿ� ���� �����ڸ� ����Ͽ� �����Ѵ�.
	{
		while (sz--)
		{
			new(dst)T(*src);
			++dst, ++src;
		}
	}
	else    // ���� �����ڰ� �ڸ��ϴٸ� ���� �����ڸ� ȣ���� �ʿ䰡 �����Ƿ� �׳� �����Ѵ�.
	{
		memcpy(dst, src, sizeof(T)*sz);
	}
}

int main()
{
	char src[10] = "hello";
	char dst[10];

	copy_type(dst, src, 10);

	cout << dst << endl;

	// strcpy(dst, src);
}