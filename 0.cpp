#include <iostream>
#include <thread>

using namespace std;

void func1() {
	cout << "my func1" << endl;
}

int main() {
	thread t(&func1);  //��� 1
	t.join();          //join()�� �����Ű�� t�� ����Ǳ� ������ ��ٸ���.

	return 0;
}