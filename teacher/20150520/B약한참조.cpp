
// weak pointer �̾߱�

#include <iostream>
#include <memory>
using namespace std;

struct Node
{
	int data;
	// Node* next;
	//shared_ptr<Node> next;

	// ���� ����� �������� �ʴ� ����Ʈ ������
	weak_ptr<Node> next;
	~Node() { cout << "~Node()" << endl; }
};

void main()
{
	shared_ptr<Node> node1(new Node);
	shared_ptr<Node> node2(new Node);

	// ��ȣ ������ ���� �߻�
	node1->next = node2;
	node2->next = node1;

	//Node* node1 = new Node;
	//Node* node2 = new Node;
}
	//shared_ptr<Node> next;
