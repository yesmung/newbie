// ���ø� �̾߱�

#include <iostream>
using namespace std;

#if 0
// 1. �Լ� ���ø�
template<typename T> T Max(T a, T b) { return a > b ? a : b; }

// 2. Ŭ���� ���ø�
template<typename T> class Stack
{
	T* arr;
public :
	void push_front(T data) {}
};

void main()
{
	cout << Max<int>(1, 2) << endl; // OK
	cout << Max(1, 2) << endl;		// OK, �Լ� ���ø��� �Ͻ��� �߷��� �����ϴ�.

	// Stack s1;						// ERROR, Ŭ���� ���ø��� �Ͻ��� �߷��� �Ұ����ϴ�.
	// s1.push_front(20);

	Stack<int> s;					// OK, Ŭ���� ���ø��� Ÿ���� ��������� ����ؾ� �մϴ�. Stack�� ���ø��̴�. ����, �Ʒ��� ���� ��ü ���� ��, Ÿ�� ���� ������� �Ѵ�.
	Stack<int> s2;					// ������(s2) �տ��� �Ϻ��� Ÿ�� ����(Stack<int>)�� �;� �ȴ�. ��, Stack<int>�� ������ Ÿ�Դϴ�.
}
#endif

#if 0
// ���ø� ����
template<typename T> class Stack
{
public :
	void push() { cout << "T" << endl; }
};

// �κ� ����ȭ
template<typename T> class Stack < T* >
{
public :
	void push() { cout << "T*" << endl; }
};

// ����ȭ ���ø�
template<> class Stack < char * >
{
public :
	void push() { cout << "char*" << endl; }
};


int main()
{
	Stack<int> s1; s1.push();
	Stack<int*> s2; s2.push();
	Stack<char*> s3; s3.push();
}
#endif


// ���ø� ��Ÿ ���α׷���
#include <iostream>
using namespace std;

int fact(int n)
{
	// ��� ȣ���� �׻� �͹̳� ������ �������� ��
	if (n <= 1) 
		return 1;
	else
		return n * fact(n - 1);
}

// ��� ���� N�� ���ؼ� ó���ϴ� Ŭ������ �����մϴ�.
template<int N> struct Fact
{
	// ������ Ÿ�� ���� : iteral, enum
	// ��Ÿ�� ���� : symbol
	enum { value = N * Fact<N-1>::value };
};

// N�� 1�� ��쿡 ���ؼ� ó���� ����ȭ�� ���
template<> struct Fact<1>
{
	enum { value = 1 };
};

void main()
{
	cout << fact(5) << endl;
	cout << Fact<5>::value << endl;
}

























