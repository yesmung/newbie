
#if 0
#include <iostream>
using namespace std;

// �Ʒ��� ����Ʈ �����͸� ���� ��� ����� ��å���� ������ ������ ;D
template<typename T>
class Sptr {
	T* pObj;
	int* ref;
public:
	Sptr(T* p) : pObj(p), ref(new int(1)) {}
	
	Sptr(const Sptr& o) : pObj(o.pObj), ref(o.ref)
	{
		++(*ref);
	}

	~Sptr() {
		if (--(*ref) == 0)
		{
			delete pObj;
			delete ref;
		}
	}

	T* operator->() { return pObj; }
	T& operator*() { return *pObj; }
};

void main()
{
	//{
	//	int* p1 = new int;
	//	*p1 = 10;
	//	cout << *p1 << endl;

	//	getchar();

	//	int* p2 = p1;
	//	*p2 = 20;
	//	cout << *p1 << endl;
	//}

	
	Sptr<int> p1 = new int;
	*p1 = 20;
	cout << *p1 << endl;

	getchar();

	Sptr<int> p2 = p1;
	*p2 = 0;
	cout << *p2 << endl;
	
	getchar();

}
#endif


// ����Ʈ �������� ����
#include <iostream>
using namespace std;

template<typename T> class Sptr {
	T* pObj;
	int* ref;
public:
	Sptr(T* p) : pObj(p), ref(new int(1)) {}
	Sptr(const Sptr& o) : pObj(o.pObj), ref(o.ref) { ++(*ref); }
	~Sptr() {
		if (--(*ref) == 0)
		{
			delete pObj;
			delete ref;
		}
	}
	T* operator->() { return pObj; }
	T& operator*() { return *pObj; }
};

void main()
{
	cout << sizeof(int*) << endl;		// 4
	cout << sizeof(Sptr<int>) << endl;	// ?
}