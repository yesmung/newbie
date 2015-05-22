#include <iostream>
#include "..\io_ipc.h"
using namespace std;

// step 1. ���δ� ����̹� ������ ���� �� �����ϱ� ���Ͽ� Ŭ������ �����Ѵ�.
class ProcessState
{
	int mDriverFD;
public :
	// ProcessState() { mDriverFD = binder_open(128 * 1024); }
	// ProcessState() { mDriverFD = open_driver();  }
	ProcessState() : mDriverFD(open_driver()) {}
	int getDriver() { return mDriverFD; }

	// ��� �����ڸ� ���� ������ API
	int open_driver() { return binder_open(128 * 1024); }
};


int main()
{
    // const int fd = binder_open(128*1024);
	// ProcessState proc; // stack overflow ���ɼ� ����, heap ���ٰ� ������
	// ProcessState* proc = new ProcessState;
	// == ���� ��ġ, �ٸ� �ȵ���̵� �ҽ� �ڵ�� �Ʒ��� ����
	ProcessState* PROC(new ProcessState);


    int msg, reply;

	// BINDER_SERVICE_MANAGER ��� ���񽺰� �ִ��� üũ�ش޶�
	// �׸��� "LedService"��� ���ڿ��� ���� �޴������Կ��� �����޶�
	// BINDER_SERVICE_MANAGER�� SVC_MGR_CHECK_SERVICE��
	// ȣ��� ���� �Ķ���� ("LedService")
	// ���ϰ��� &replay
	// fd�� ���δ� ����̹��� �����ϱ� ���� ��(���� binder_open)
	// fd -> proc.getDriver()
	// proc->getDriver()
    int handle = binder_call(proc->getDriver(),
                            (int*)"LedService",
                            &reply,
                            BINDER_SERVICE_MANAGER,
                            SVC_MGR_CHECK_SERVICE);
    while(1)
    {
		// 1 : led on, 2: led off
		getchar(); binder_call(proc->getDriver(), &msg, &reply, handle, 1);
		getchar(); binder_call(proc->getDriver(), &msg, &reply, handle, 2);
    }

    return 0;
}