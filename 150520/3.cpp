#include <iostream>
using namespace std;

template <typename T>
class Sptr
{
	T* pObj;
	int* refs;
public:
	Sptr(T* p) : pObj(p), refs(new int(1)) { }
	~Sptr() 
	{ 
		if (--(*refs) == 0) 
		{
			delete pObj;
			delete refs;
		}
	}

	Sptr(const T& c) : pObj(c.pObj), refs(c.refs)
	{
		(*refs)++;
	}

	T* operator->() { return pObj; }
	T& operator*() { return *pObj; }
};

void main()
{
	cout << sizeof(int*) << endl; // ?
	//{
	//	int* p1 = new int;
	//	*p1 = 10;
	//	cout << *p1 << endl;
//
//		getchar();
//
//		int* p2 = p1;
//		*p2 = 20;
//		cout << *p1 << endl;
//	}
//	{
//		Sptr<int> p1 = new int;
//		*p1 = 20;
//		cout << "p1 = " << *p1 << endl;
//		getchar();
//
//		Sptr<int> p2 = p1;
//		*p2 = 0;
//		cout << "p1 = " << *p1 << endl;
//		cout << "p2 = " << *p2 << endl;
//		getchar();
//	}
	
}