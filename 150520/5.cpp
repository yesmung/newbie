// 참조 계수 기반의 스마트 포인터는 이미 표준에서 지원합니다.
#include <iostream>
#include <memory>
using namespace std;

// 이것은 안드로이드의 스마트 포인터
template <typename T>
class sp
{
	T* pObj;
public:
	sp(T* p) : pObj(p) { if (pObj) pObj->incStrong(); }
	~sp() { if (pObj) pObj->decStrong(); }
	sp(const T& c) : pObj(c.pObj)	{ if (pObj) pObj->incStrong(); }

	T* operator->() { return pObj; }
	T& operator*() { return *pObj; }
};

int main()
{
	// shared_ptr<int> p1 = new int; // shared_ptr<int> p1 = shared_ptr<int>(new int) 암시적 사용x 인자가 하나인 생성자는 변환생성자.. explict 되어 있음
	shared_ptr<int> p1(new int);
	*p1 = 20;
	cout << *p1 << endl;
	shared_ptr<int> p2(p1);
	cout << sizeof(shared_ptr<int>) << endl; // 참조 계수를 갖고 있다. 

	sp<int> p3 = new int; // ERROR, RefBase의 자식만 됨.. 빌트인 타입은 안됨
}

// shared_ptr의 장점 : 모든 타입을 처리할 수 있습니다.
//				단점 : 메모리의 낭비가 있습니다.
// sp의 장점 : 메모리의 낭비가 없습니다.
//		단점 : RefBase의 서브 클래스만 관리될 수 있습니다.