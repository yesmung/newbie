
// ���� ����Ʈ �����̳ʸ� ������ ���ô�.
#include <iostream>
#include <vector>
using namespace std;

template<typename T> struct Node
{
	T data;
	Node* next;
	Node(T d, Node* n) : data(d), next(n) {}
};


/*
template<typename T1, typename T2>
T1* xfind(T1* first, T1* last, T2 ch)
{
// �迭 ���¿��� ��� ����... ���� ���, List ���� ���� ++������ �ȵ�
while (first != last && *first != ch)
++first;
return first;
}
���⼭ *, ++ �����Ǿ����
*/

// �����̳� ���� ��Ҹ� ����Ű�� ����Ʈ �����͸� ������ ���ô�
template <typename T> class slist_iterator
{
	Node<T>* cur;
public:
	slist_iterator(Node<T>* p = 0) : cur(p) {}
	// ++obj; -> obj.operator++
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
	// Node* head; Node�� ���̻� Ÿ���� �ƴ�. ���ø���
	Node<T>* head;
public :

	slist() : head(0){}

	void insert_front(T data)
	{
		head = new Node<T>(data, head);
		// Node* temp = new Node;
		// temp->data = data;
		// temp->next = head;
		// head = temp;
	}

	void display()
	{
		Node<T>* node;

		system("cls");
		for (node = head; node != 0; node = node->next)
			printf("->[%d]", node->data);

		getchar();
	}

	// ����ڿ��� ierator ��� �ϰ��� Ÿ������ ����
	// �����̳� �����ڵ� ����ϰ� �ڵ� ������ �����ϴ�. 
	typedef slist_iterator<T> iterator;
	iterator begin() { return iterator(head); }
	iterator end() { return iterator(0); } // ������ ������Ҵ� null 

	// ��� �����̳� �����ڴ� �����̳� ������ �ڷᱸ���� ���۰�
	// ������ ���� ��ġ�� ��ȯ�ϴ� ������ �Լ��� �����ϱ�� ����մϴ�.
	// �˻��� ���ϰ� �ϱ� ����... 
	// slist_iterator<T> begin() { return slist_iterator<T>(head); }
	// slist_iterator<T> end() { return slist_iterator<T>(0); } // ������ ������Ҵ� null 
};

template<typename T1, typename T2> T1 xfind(T1 first, T1 last, T2 ch)
{
	// �迭 ���¿��� ��� ����... ���� ���, List ���� ���� ++������ �ȵ�
	// �Ʒ��� �ڵ�� ������ ��������... first ���� ��ü.. �Ʒ� ++�� �����ڰ� �ƴ� �Լ�(�����ε�)..�Լ� ȣ���� ����� ŭ -> inline ����
	while (first != last && *first != ch)
		++first;
	return first;
}

int main()
{
	slist<int> s;
	for (int i = 1; i <= 10; i++)
	{
		s.insert_front(i*10);
	}
	// slist_iterator<int> p1 = xfind(s.begin(), s.end(), 50);
	slist<int>::iterator p1 = xfind(s.begin(), s.end(), 50);

	if (p1 == s.end())
		cout << "not found" << endl;
	else
		cout << "found" << endl;

	// slist�� ������ �ִ� ��� �����͸� ����ϴ� �ڵ带 ������ ������.
	p1 = s.begin();
	while (p1 != s.end())
	{
		cout << *p1 << endl;
		p1++;
	}

	// slist<int>::iterator
}

