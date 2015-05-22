
// 참조 계수 기반의 스마트 포인터는 이미 표준에서 지원합니다.
#include <iostream>
#include <memory>
using namespace std;

template<typename T> class sp
{
	T* pObj;
public:
	sp(T* p) : pObj(p) { if (pObj) pObj->incStrong(); }
	sp(const sp& o) : pObj(o.pObj) { if (pObj) pObj->incStrong(); }
	~sp() { if (pObj) pObj->decStrong(); }

	T* operator->() { return pObj; }
	T& operator*() { return *pObj; }
};

int main()
{
	shared_ptr<int> p1(new int);	// shared_ptr<int> p1 = shared_ptr<int>(new int);
	*p1 = 20;
	cout << *p1 << endl;
	shared_ptr<int> p2(p1);
	cout << sizeof(shared_ptr<int>) << endl;

	sp<int> p3 = new int;	// ERROR
}

// shared_ptr의 장점: 모든 타입을 처리할 수 있습니다.
//				단점: 메모리의 낭비가 있습니다.

// sp의 장점: 메모리의 낭비가 없습니다.
//      단점: RefBase의 서브 클래스만 관리될 수 있습니다.