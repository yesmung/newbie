#if 0
// Decay
#include <iostream>
using namespace std;

void init_int(int a) { a = 0; }
void init_arr(int a[]) { a[0] = 0; }

void main()
{
	int i = 10;
	int a[10] = { 20 }; // int[1] Ÿ�� : �迭���� Ÿ���� ������

	init_int(i);			// init_int(10);
	cout << i << endl;

	// ������ ������ ���� : ���ü�� Ÿ�� * ������
	int* pInt = &i;

	getchar();

	init_arr(a);			// init_int(0x12FF60)
	cout << a[0] << endl;

	// int* pArr[1] = a;		// ERROR, ������ٰ� ����� ���� �� ����. ��, *�� ���� �ؼ��ǵ��� �ٲ�� �Ѵ�.
	// int(*pArr)[1] = a;		// ERROR,
	int* pArr = a;				// decay : �迭�� Ÿ���� �迭 ��ü���� ù ��° ������ Ÿ������ ��� �Ǵ� ��ȭ�� ���� �ǹ�

	cout << sizeof a << endl;   // �迭�� �̸��� ù��° ���Ҹ� ����Ű�� ������, ��, 4����Ʈ�� ���;� ������, sizeof �����ڴ� decay�� �߻����� �ʴ´�.
	cout << sizeof pArr << endl;
}
#endif

#if 0
// Argument decay (template arg�� decay�� �߻��Ѵ�.)
#include <iostream>
using namespace std;

// ��(T)�� ���ڷ� ���� ��� : �迭�� �Լ��� decay�� �߻�
template<typename T> void type_check_val(T t)
{
	cout << typeid(T).name() << endl;
}

// ����(T&)�� ���ڷ� ���� ���, ��Ȯ�� Ÿ������ ���޵� 
template<typename T> void type_check_ref(T& t)
{
	cout << typeid(t).name() << endl;
}

void main()
{
	int i = 0;
	type_check_val(i); // ?
	type_check_val<int>(i); // ?
	type_check_ref(i);	// ?
	
	int arr[3];
	type_check_val(arr);	// int *
	type_check_ref(arr);	// int [3]

	// char * s = " hello"  : ��� ������, ��, �̷� Ÿ���� �Լ� ������ ���� ������,const�� �׻� �ٿ�����
	// char a[] = "hello" : �迭
	type_check_val("hello");	// const char *
	type_check_ref("hello");	// const char [6]

	type_check_val(main);	// �Լ� ������
	type_check_ref(main);	// ��Ȯ�� �Լ� Ÿ������ �� (�Լ��ּ� & �ٿ��� ���� �� �ִ�)

	//int
	//int
	//int
	//int *
	//int[3]
	//char const *
	//char const[6]
	//int(__cdecl*)(void)
	//int __cdecl(void)
}
#endif

#if 1
#include <stdio.h>

template<typename T> void foo(T a, T b) { printf("called foo(T)\n"); }
template<typename T> void goo(T& a, T& b) { printf("called goo(T&)\n"); }
void goo(const char* a, const char *b) { printf("called goo(char*)\n"); }

// �����Ϸ��� �Ϲ� �Լ��� �Լ� ���ø��� �����ε� �� ���, ã�� ����
// 1. �Ϲ� �Լ�
// 2. �Լ� ���ø�

void main()
{
	foo("kkk", "hhhh"); // void foo(const char*, const char*) //signature
    goo("kkk", "hhhh"); // void goo(const 
	goo("kkk111", "hhhh222222"); // void goo(const 
}
#endif