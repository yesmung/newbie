#include <iostream>
#include "..\io_ipc.h"
using namespace std;

// step 2. ���δ� ���� ���� Ŭ������ 2�� �̻��� ��ü�� ������ ������ �����ϴ�.
// �׷��Ƿ� �̱��� ���·� �����Ǿ�� �մϴ�.

class ProcessState
{
	int mDriverFD;

	// 1. �����ڴ� private ������ ����
	ProcessState() : mDriverFD(open_driver()) {}

	// 3. ����� ������ �����ϱ� ���� ���� �����ڿ� ���� ������ �Լ��� private ������ ����
	ProcessState(const ProcessState&);
	void operator=(const ProcessState&);
	
	static ProcessState* pInstance;
public:
	// 2. ������ ��ü�� ��ȯ�ϱ� ���� ���� �������̽��� ����
	static ProcessState* self()
	{
		if (pInstance == 0)
			pInstance = new ProcessState;
		return pInstance;
	}

	int getDriver() { return mDriverFD; }
	int open_driver() { return binder_open(128 * 1024); }
};
ProcessState* ProcessState::pInstance = 0;


int main()
{
	// Singleton ���·� ���� �ʿ䰡 �ִ�.
	// ProcessState* PROC(new ProcessState);
	ProcessState* proc(ProcessState::self());

	int msg, reply;

	int handle = binder_call(proc->getDriver(),
		(int*)"LedService",
		&reply,
		BINDER_SERVICE_MANAGER,
		SVC_MGR_CHECK_SERVICE);
	while (1)
	{
		// 1 : led on, 2: led off
		getchar(); binder_call(proc->getDriver(), &msg, &reply, handle, 1);
		getchar(); binder_call(proc->getDriver(), &msg, &reply, handle, 2);
	}

	return 0;
}