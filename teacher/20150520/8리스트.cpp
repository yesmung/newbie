
// ���� ����Ʈ �����̳ʸ� ������ ���ô�.
#include <iostream>
#include <vector>
using namespace std;

template<typename T> struct Node
{
	T data;
	Node* next;
	Node(int d, Node* n) : data(d), next(n) {}
};

// �����̳� ���� ��Ҹ� ����Ű�� ����Ʈ �����͸� ������ ���ϴ�.
template<typename T> class slist_iterator
{
	Node<T>* cur;

public:
	slist_iterator(Node<T>* p = 0) : cur(p) {}

	// ++obj; -> obj.operator++();
	inline slist_iterator& operator++()
	{
		cur = cur->next;
		return *this;
	}

	inline T& operator*() { return cur->data; }

	// o1 != o2; -> o1.operator!=(o2);
	inline bool operator!=(const slist_iterator& o) { return cur != o.cur; }
	inline bool operator==(const slist_iterator& o) { return cur == o.cur; }
};


template<typename T> class slist
{
	Node<T>* head;

public:
	slist() : head(0) {}
	void insert_front(T data) { head = new Node<T>(data, head); }

	// ��� �����̳� �����ڴ� �����̳� ������ �ڷᱸ���� ���۰�
	// ������ ���� ��ġ�� ��ȯ�ϴ� ������ �Լ��� �����ϱ�� ����մϴ�.

	typedef slist_iterator<T> iterator;
	iterator begin() { return iterator(head); }
	iterator end()   { return iterator(0); }
	
	//slist_iterator<T> begin() { return slist_iterator<T>(head); }
	//slist_iterator<T> end()   {	return slist_iterator<T>(0); }
};

template<typename T1, typename T2> T1 xfind(T1 first, T1 last, T2 ch)
{
	while (first != last && *first != ch)
		++first;
	return first;
}

void main()
{
	slist<int> s;
	for (int i = 1; i <= 10; i++)
		s.insert_front(i * 10);

	// slist_iterator<int> p1 = xfind(s.begin(), s.end(), 50);
	slist<int>::iterator p1 = xfind(s.begin(), s.end(), 50);
	if (p1 == s.end())
		cout << "not found" << endl;
	else
		cout << "found" << endl;

	// slist�� ������ �ִ� ��� �����͸� ����ϴ� �ڵ带 ������ ������
	slist<int>::iterator p2 = s.begin();
	while (p2 != s.end())
	{
		cout << *p2 << endl;
		++p2;
	}
}