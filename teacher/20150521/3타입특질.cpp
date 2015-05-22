






#if 0
// type traits
#include <iostream>
using namespace std;

template<typename T> T Max(T a, T b)
{
	if (T is pointer)
		return *a > *b ? *a : *b;
	else
		return a > b ? a : b;
}

int main()
{
	int a = 10, b = 20;
	Max(&a, &b);
}
#endif


// type traits: Ÿ�� Ư�� �Ǵ� Ÿ�� Ư��
// ������ Ÿ�ӿ� Ÿ���� Ư���̳� �Ǵ� Ư���� �˾Ƴ��� ���

#if 0
// �� ���ø��� ������ ��ȯ
// �κ� ����ȭ�� ���� ��ȯ

#include <stdio.h>
// is_pointer Ŭ���� �Ǵ� ����ü�� ������ ������ ;D

// �� ���ø�: ��� Ÿ���� �����Ͱ� �ƴ϶�� ����
template<typename T> struct is_pointer {
	enum { value = false };
};

// �κ� ����ȭ: Ư�� Ÿ���� �����Ͷ�� ����
template<typename T> struct is_pointer < T* > {
	enum { value = true };
};

template<typename T> void foo(const T& a)
{
	if (is_pointer<T>::value)
		printf("pointer\n");
	else
		printf("no pointer\n");
}

// ���� �ڵ�� �Ʒ��� �ڵ�� ����˴ϴ�.
// void foo(int a) { printf("no pointer\n"); }
// void foo(int* a) { printf("pointer\n"); }




void main()
{
	int  i; foo(i);	// no pointer
	int* p; foo(p);	// pointer
}

#endif


#if 0
// ���� ���� ;D
#include <stdio.h>

// is_array ����ü�� �����غ����� :D
template<typename T> struct is_array {
	enum { size = -1 };
	enum { value = false };
};

template<typename T, int N> struct is_array < T[N] > {
	enum { size = N };
	enum { value = true };
};

// �迭�� ���̵� ����غ����� ;D
template<typename T> void foo(const T& a)
{
	if (is_array<T>::value)
		printf("array: %d\n", is_array<T>::size);
	else
		printf("no array\n");
}

int main()
{
	int i;		foo(i);	// no array
	int a[5];	foo(a);	// array


}

#endif


// type traits ����� �̹� ǥ�ؿ��� �����ϰ� �ֽ��ϴ�.
#include <iostream>
using namespace std;

int *p;
int arr[3];

int main()
{
	if (is_pointer<int*>::value) cout << "pointer" << endl;
	if (is_array<int[3]>::value) cout << "array" << endl;
}