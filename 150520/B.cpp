// weak pointer �̾߱�

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

	// node1->next = node2; // next�� ��¥ ������.. node2�� smart pointer, 
	node1->next = node2;
	node2->next = node1; // �Ҹ��ڰ� �Ҹ��� �ʴ´�. ��? �׸��� �׷�����. ������� Ȯ���غ���

	// ��� (�������) ����
	// node1 (1) 18
	// node2 (1) 19
	// node2 (2) 22
	// node1 (2) 23
	// �ᱹ �Ҹ�Ǵ� ������ ���� ����� �� 1�̹Ƿ�, free ���� ���� -> �̰��� ��ȣ ������ ����!

	// smart pointer 
	// shared_ptr : ������� ����
	// weak_ptr : ��������� �������� ����, �� ��ȣ�����Ǵ� �ڵ��� ��� �ݵ�� ���������� weak_ptr�� ����ؾ��� (strong pointer�� ��ȣ���� ��, ������)
	// �ȵ���̵� sp(strong pointer), wp, up

	// Node* node1 = new Node;
	// Node* node2 = new Node;
}