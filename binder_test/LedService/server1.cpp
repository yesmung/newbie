
// step 1
// #include <iostream>
// #include "..\io_ipc.h"
// using namespace std;
#include "../Android.h"


// step 1. ������ �ڵ嵵 IBinder�� �ڽ����� ������ �մϴ�.
// ���� ������ ������ ����Ŭ������ BBinder�� �����մϴ�.
class BBinder : public IBinder
{
protected : 
	// ���� callback �ܺ� �������̵� ���� ����
	// ���� �Ʒ��� ���� protected�� �� ���� medthod ȣ��
	int transact(int code, int* msg, int* reply)
	{
		return onTransact(code, msg, reply);
	}
public :
	virtual int onTransact(int code, int* msg, int* reply) { return 0; }
};

// ���� �����ڴ� BBinder�� ��� �ް�, onTransact�� �������̵� �ϸ� �˴ϴ�.
class LedService : public BBinder
{
	// callback �Լ�
	int onTransact(int code, int* msg, int* reply)
	{
		switch (code)
		{
		case 1: cout << "LED ON" << endl; break;
		case 2: cout << "LED OFF" << endl; break;
		}
		return 0;
	}
};


int main()
{   
    // 1. ���δ� ����̹� ����
    // struct binder_state* bs;
    int fd = binder_open(128*1024); // 128KB

    // 2. ���� �Ŵ������� ���� �̸� ���
    int reply;
    binder_call(fd,
               (int*)"LedService",
               &reply,
               BINDER_SERVICE_MANAGER,
               SVC_MGR_ADD_SERVICE);

    // 3. Ŭ���̾�Ʈ�� ��û ���
    binder_loop(fd, myHandler);
    return 0;
}