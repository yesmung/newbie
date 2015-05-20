// weak pointer 이야기

#include <iostream>
#include <memory>
using namespace std;

struct Node
{
	int data;
	//Node* next;
	// shared_ptr<Node> next;
	weak_ptr<Node> next;

	~Node(){ cout << "~Node()" << endl; }
};

void main()
{
	shared_ptr<Node> node1(new Node);
	shared_ptr<Node> node2(new Node);

	// node1->next = node2; // next는 진짜 포인터.. node2는 smart pointer, 
	node1->next = node2;
	node2->next = node1; // 소멸자가 불리지 않는다. 왜? 그림을 그려보자. 참조계수 확인해보자

	// 노드 (참조계수) 라인
	// node1 (1) 18
	// node2 (1) 19
	// node2 (2) 22
	// node1 (2) 23
	// 결국 소멸되는 시점에 참조 계수가 각 1이므로, free 되지 않음 -> 이것은 상호 참조의 문제!

	// smart pointer 
	// shared_ptr : 참조계수 관리
	// weak_ptr : 참조계수를 관리하지 않음, 즉 상호참조되는 코드의 경우 반드시 내부적으로 weak_ptr을 사용해야함 (strong pointer는 상호참조 시, 문제됨)
	// 안드로이드 sp(strong pointer), wp, up

	// Node* node1 = new Node;
	// Node* node2 = new Node;
}