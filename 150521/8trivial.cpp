http://androidxref.com/5.1.0_r1/xref/frameworks/rs/cpp/util/TypeHelpers.h 

// trivial : �ڸ��ϴ� -> �ƹ� �ϵ� ���� �ʴ´�.
#include <iostream>
#include <cstdlib>
using namespace std;

#if 0
// trivial ������ : �ڸ��� ������ -> �ƹ��� �ϵ� ���� �ʴ� ������
// 1. �θ� ���ų� �θ��� �����ڰ� �ڸ��ϰų�
// 2. ��ü�� ����� ���ų� ��ü�� ����� �����ڰ� �ڸ��ϰų�
// 3. �����Լ��� ���ų�
// 4. ����ڰ� ���� �����ڰ� ���ٸ�
// �����ڰ� �ڸ��ϴٰ� �� �� �ִ�.

class Test
{
public :
	int a;
	virtual void foo() {}
	// virtual ~Test() {}
	// �⺻ ������
	Test() { }
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

// T�� �ڸ����� ��� �� �� �ֳ�?
// ��Ʈ�� Ÿ��. ���� �����ڰ� ����. ���� �����ڰ� �ڸ��ϴ�.
// ��Ʈ�� Ÿ���� �ƴϸ� ���� �����ڰ� �ڸ��ϴٰ� �� �� ����.
// �κ���~�κ���~�κ�Ÿ��... �κ�����ȭ�� �ƴ� ����ȭ�� �����ڴ�.
// ����, Ÿ�� Ʈ����Ʈ ��, ��Ʈ��Ÿ��(Ư��Ÿ��)�� ���ؼ��� �ڸ��ϹǷ� ���� ��ȯ -> ����ȭ�� ���
// �� �ܿ� ���ؼ��� ������ ��ȯ

// ��� Ÿ���� ���� ������, ������, �Ҹ��ڴ� �ڸ����� �ʴ�.
template<typename T> struct trait_trivial_copy { enum { value = false }; };
template<typename T> struct trait_trivial_ctor { enum { value = false }; };
template<typename T> struct trait_trivial_dtor { enum { value = false }; };

// �ٸ� �⺻ Ÿ���� �ڸ��ϴ�.
#define BASIC_TYPE_TRAIT(T)	\
	template<> struct trait_trivial_copy<T> { enum { value = true }; };	\
	template<> struct trait_trivial_ctor<T> { enum { value = true }; };	\
	template<> struct trait_trivial_dtor<T> { enum { value = true }; };

BASIC_TYPE_TRAIT(char)
BASIC_TYPE_TRAIT(short)
BASIC_TYPE_TRAIT(int)
BASIC_TYPE_TRAIT(long)
BASIC_TYPE_TRAIT(float)
BASIC_TYPE_TRAIT(double)

struct Point
{
	int x, y;
};
BASIC_TYPE_TRAIT(Point); // �̷��� �ڸ��� �Ϳ� ���ؼ��� basic type���� �صд�.

template<typename T> void copy_type(T* dst, T* src, size_t sz)
{
	// ���� �����ڰ� �ڸ����� �����Ƿ�, ���� �����ڸ� ����ؼ� �����Ѵ�.
	if (trait_trivial_copy<T>::value == false)
	{
		while (sz--)
		{
			new(dst)T(*src);
			++dst, ++src;
		}
		
	}
	// ���� �����ڰ� �ڸ��ϴٸ� ���� �����ڸ� ȣ���� �ʿ䰡 �����Ƿ� �׳� �����Ѵ�.
	else
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