
// 이제 리스트 컨테이너를 생각해 봅시다.
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
// 배열 형태에만 사용 가능... 예를 들면, List 같은 것은 ++연산이 안됨
while (first != last && *first != ch)
++first;
return first;
}
여기서 *, ++ 구현되어야함
*/

// 컨테이너 안의 요소를 가리키는 스마트 포인터를 설계해 봅시다
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
	// Node* head; Node는 더이상 타입이 아님. 템플릿임
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

	// 사용자에게 ierator 라는 일관된 타입으로 제공
	// 컨테이너 설계자도 깔끔하게 코드 정리가 가능하다. 
	typedef slist_iterator<T> iterator;
	iterator begin() { return iterator(head); }
	iterator end() { return iterator(0); } // 마지막 마음요소는 null 

	// 모든 컨테이너 설계자는 컨테이너 내부의 자료구조의 시작과
	// 마지막 다음 위치를 반환하는 다음의 함수를 제공하기로 약속합니다.
	// 검색을 편리하게 하기 위함... 
	// slist_iterator<T> begin() { return slist_iterator<T>(head); }
	// slist_iterator<T> end() { return slist_iterator<T>(0); } // 마지막 마음요소는 null 
};

template<typename T1, typename T2> T1 xfind(T1 first, T1 last, T2 ch)
{
	// 배열 형태에만 사용 가능... 예를 들면, List 같은 것은 ++연산이 안됨
	// 아래의 코드는 성능이 떨어진다... first 등은 객체.. 아래 ++는 연산자가 아닌 함수(오버로딩)..함수 호출의 비용이 큼 -> inline 하자
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

	// slist가 가지고 있는 모든 데이터를 출력하는 코드를 구현해 보세요.
	p1 = s.begin();
	while (p1 != s.end())
	{
		cout << *p1 << endl;
		p1++;
	}

	// slist<int>::iterator
}

