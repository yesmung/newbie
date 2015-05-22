
#if 0
// trivial: 자명하다 -> 아무 일도 하지 않는다.
#include <iostream>
#include <cstdlib>
using namespace std;

// trivial 생성자: 자명한 생성자 -> 아무런 일도 하지 않는 생성자
// 1. 부모가 없거나 부모의 생성자가 자명하거나
// 2. 객체형 멤버가 없거나 객체형 멤버의 생성자가 자명하거나
// 3. 가상 함수가 없거나
// 4. 사용자가 만든 생성자가 없다면
// 생성자가 자명하다라고 할 수 있다.

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

// 모든 타입의 복사 생성자는 자명하지 않다
template<typename T> struct trait_trivial_copy { enum { value = false }; };
template<typename T> struct trait_trivial_ctor { enum { value = false }; };
template<typename T> struct trait_trivial_dtor { enum { value = false }; };

// 다만 기본 타입은 자명하다
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

// androidxref.com -> copy_type 검색


struct Point
{
	int x, y;
};

BASIC_TYPE_TRAITS(Point)

template<typename T> void copy_type(T* dst, T* src, size_t sz)
{
	if (trait_trivial_copy<T>::value == false)	// 복사 생성자가 자명하지 않으므로 복사 생성자를 사용하여 복사한다.
	{
		while (sz--)
		{
			new(dst)T(*src);
			++dst, ++src;
		}
	}
	else    // 복사 생성자가 자명하다면 복사 생성자를 호출할 필요가 없으므로 그냥 복사한다.
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