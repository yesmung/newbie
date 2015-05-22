
// weak pointer 이야기

#include <iostream>
#include <memory>
using namespace std;

struct Node
{
	int data;
	// Node* next;
	//shared_ptr<Node> next;

	// 참조 계수를 관리하지 않는 스마트 포인터
	weak_ptr<Node> next;
	~Node() { cout << "~Node()" << endl; }
};

void main()
{
	shared_ptr<Node> node1(new Node);
	shared_ptr<Node> node2(new Node);

	// 상호 참조의 문제 발생
	node1->next = node2;
	node2->next = node1;

	//Node* node1 = new Node;
	//Node* node2 = new Node;
}
	//shared_ptr<Node> next;
