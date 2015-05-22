
#if 0
// Decay
#include <iostream>
using namespace std;

void init_int(int a)   { a = 0; }
void init_arr(int a[]) { a[0] = 0; }

void main()
{
	int i = 10;
	int a[10] = { 20, };	// int[1] 

	init_int(i);	// init_int(10);
	cout << i << endl;

	// ������ ������ ����: ���ü�� Ÿ�� * ������;
	int* pInt = &i;

	getchar();

	init_arr(a);	// init_int(0x12FF60);
	cout << a[0] << endl;

	// int(* pArr)[1] = a;
	int* pArr = a;	// decay: �迭�� Ÿ���� �迭 ��ü���� ù ��° ������ Ÿ������ ��� �Ǵ� ��ȭ

	cout << sizeof a << endl;	// sizeof �����ڴ� decay�� �߻����� �ʽ��ϴ�.
}
#endif


#if 0
// Argument Decay
#include <iostream>
using namespace std;


// ��(T)�� ���ڷ� ���� ���: �迭�� �Լ��� Decay�� �߻�
template<typename T> void type_check_val(T t) {
	cout << typeid(t).name() << endl;
}

// ����(T&)�� ���ڷ� ���� ���: ��Ȯ�� Ÿ������ ���޵�
template<typename T> void type_check_ref(T& t) {
	cout << typeid(t).name() << endl;
}


void main()
{
	int i = 0;
	type_check_val(i);	// int
	type_check_ref(i);	// int

	int arr[3];
	type_check_val(arr);	// int *
	type_check_ref(arr);	// int [3]

	type_check_val("hello");	// const char*
	type_check_ref("hello");	// const char[6]

	type_check_val(main);		// 1
	type_check_ref(main);		// 2


}
#endif


#include <stdio.h>

template<typename T> void foo(T a, T b) { printf("called foo(T)\n"); }
template<typename T> void goo(T& a, T& b) { printf("called goo(T&)\n"); }
void goo(const char* a, const char *b) { printf("called goo(char*)\n"); }

// �����Ϸ��� �Ϲ� �Լ��� �Լ� ���ø��� �����ε� �� ���, ã�� ����
// 1. �Ϲ� �Լ�
// 2. �Լ� ���ø�

void main()
{
	foo("kkk", "hhhh");	// void foo(const char*, const char*);

	// �Ʒ��� �Լ��� ȣ��ǵ��� �ڵ带 ������ ������ ;D
	goo("kkk", "hhhh");
	goo("kkfdasfak", "fadsfasfdsafdasfashhhh");
}