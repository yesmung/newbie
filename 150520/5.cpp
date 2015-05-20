// ���� ��� ����� ����Ʈ �����ʹ� �̹� ǥ�ؿ��� �����մϴ�.
#include <iostream>
#include <memory>
using namespace std;

// �̰��� �ȵ���̵��� ����Ʈ ������
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
	// shared_ptr<int> p1 = new int; // shared_ptr<int> p1 = shared_ptr<int>(new int) �Ͻ��� ���x ���ڰ� �ϳ��� �����ڴ� ��ȯ������.. explict �Ǿ� ����
	shared_ptr<int> p1(new int);
	*p1 = 20;
	cout << *p1 << endl;
	shared_ptr<int> p2(p1);
	cout << sizeof(shared_ptr<int>) << endl; // ���� ����� ���� �ִ�. 

	sp<int> p3 = new int; // ERROR, RefBase�� �ڽĸ� ��.. ��Ʈ�� Ÿ���� �ȵ�
}

// shared_ptr�� ���� : ��� Ÿ���� ó���� �� �ֽ��ϴ�.
//				���� : �޸��� ���� �ֽ��ϴ�.
// sp�� ���� : �޸��� ���� �����ϴ�.
//		���� : RefBase�� ���� Ŭ������ ������ �� �ֽ��ϴ�.