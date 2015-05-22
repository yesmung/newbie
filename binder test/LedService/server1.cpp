
// step 1
#include <iostream>
#include "..\io_ipc.h"
using namespace std;

int myHandler( int code, int* msg, int* reply )
{
	switch( code )
	{
	case 1: cout << "LED ON" << endl; break;
	case 2: cout << "LED OFF" << endl; break;
	}
	return 0;
}


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