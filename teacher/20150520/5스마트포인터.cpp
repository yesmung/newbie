
// ���� ��� ����� ����Ʈ �����ʹ� �̹� ǥ�ؿ��� �����մϴ�.
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

// shared_ptr�� ����: ��� Ÿ���� ó���� �� �ֽ��ϴ�.
//				����: �޸��� ���� �ֽ��ϴ�.

// sp�� ����: �޸��� ���� �����ϴ�.
//      ����: RefBase�� ���� Ŭ������ ������ �� �ֽ��ϴ�.