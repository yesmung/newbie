// type traits(Ÿ��Ư��, Ÿ��Ư��)
// ������ Ÿ�ӿ� Ÿ���� Ư���̳� �Ǵ� Ư���� �˾Ƴ��� ���

// �� ���ø��� ������ ��ȯ
// �κ� ����ȭ�� ���� ��ȯ

#if 0
// #include <iostream>
#include <stdio.h>

template<typename T> T Max(T a, T b)
{
	return a > b ? a : b;
}

// is_pointer �����ϱ�
// �� ���ø� : ��� Ÿ���� �����Ͱ� �ƴ϶�� ����
template<typename T> struct is_pointer
{
	enum { value = false };
};
// �κ� ����ȭ : Ư�� Ÿ���� �����Ͷ�� ����
template<typename T> struct is_pointer<T*>
{
	enum { value = true };
};

// �����Ϸ��� ���� Ÿ�� �ʴ� �ڵ�(dead code, unreachable code)�� ����ȭ�� ���ؼ� �����.
template<typename T> void foo(const T& a)
{
	if (is_pointer<T>::value)
		printf("pointer\n");
	else
		printf("not pointer\n");
}

// void foo(int a) { printf("no pointer\n"); }
// void foo(int* a) { printf("pointer\n"); }

/*
Animal* p1 = new Animal; // ��Ÿ�� ����
const type_info& t1 = typeid(p1);
*/
int main()
{
	int i = 20;
	int* j = &i;
	foo(i);
	foo(j);
}
#endif

#if 0

// ���� ���� 
#include <stdio.h>

// is_array ����ü�� �����غ�����. + �迭�� ���� 
template<typename T> struct is_array
{
	enum { value = false, size };
};

// template<typename T> struct is_array<T[1]> // argument decay �߻� nono, foo���� &(reference)�� ����,
template<typename T, int N> struct is_array<T[N]>
{
	enum { value = true, size = N };
};
template<typename T>void foo(const T& a)
{
	if (is_array<T>::value)
	{
		printf("array\n");
		printf("size = %d\n", is_array<T>::size);
	}
	else
		printf("not array\n");
}

int main()
{
	int i; 
	foo(i); //no array
	int a[1]; // int[1]
	foo(a); //array
	int b[10];
	foo(b);
}
#endif

// type trait ����� �̹� ǥ�ؿ��� �����ϰ� �ֽ��ϴ�.
#include <iostream>
using namespace std;

int *p;
int arr[3];

int main()
{
	if (is_pointer<int*>::value) cout << "pointer" << endl;
	if (is_array<int[3]>::value) cout << "array" << endl; // std ������ array���� size ���ϴ� ���� ����.

	// �����ڰ� �ڸ����� ���� �� �� �ִ�.
}