http://androidxref.com/5.1.0_r1/xref/frameworks/rs/cpp/util/TypeHelpers.h 

// trivial : 자명하다 -> 아무 일도 하지 않는다.
#include <iostream>
#include <cstdlib>
using namespace std;

#if 0
// trivial 생성자 : 자명한 생성자 -> 아무런 일도 하지 않는 생성자
// 1. 부모가 없거나 부모의 생성자가 자명하거나
// 2. 객체형 멤버가 없거나 객체형 멤버의 생성자가 자명하거나
// 3. 가상함수가 없거나
// 4. 사용자가 만든 생성자가 없다면
// 생성자가 자명하다고 할 수 있다.

class Test
{
public :
	int a;
	virtual void foo() {}
	// virtual ~Test() {}
	// 기본 생성자
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

// T가 자명한지 어떻게 알 수 있나?
// 빌트인 타입. 복사 생성자가 없다. 복사 생성자가 자명하다.
// 빌트인 타입이 아니면 복사 생성자가 자명하다고 할 수 없다.
// 부분의~부분의~부분타입... 부분전문화가 아닌 전문화에 가깝겠다.
// 따라서, 타입 트레이트 시, 빌트인타입(특정타입)에 대해서는 자명하므로 참을 반환 -> 전문화를 사용
// 그 외에 대해서는 거짓을 반환

// 모든 타입의 복사 생성자, 생성자, 소멸자는 자명하지 않다.
template<typename T> struct trait_trivial_copy { enum { value = false }; };
template<typename T> struct trait_trivial_ctor { enum { value = false }; };
template<typename T> struct trait_trivial_dtor { enum { value = false }; };

// 다만 기본 타입은 자명하다.
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
BASIC_TYPE_TRAIT(Point); // 이렇게 자명한 것에 대해서는 basic type으로 해둔다.

template<typename T> void copy_type(T* dst, T* src, size_t sz)
{
	// 복사 생성자가 자명하지 않으므로, 복사 생성자를 사용해서 복사한다.
	if (trait_trivial_copy<T>::value == false)
	{
		while (sz--)
		{
			new(dst)T(*src);
			++dst, ++src;
		}
		
	}
	// 복사 생성자가 자명하다면 복사 생성자를 호출할 필요가 없으므로 그냥 복사한다.
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