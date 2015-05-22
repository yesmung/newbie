#include <iostream>
#include "..\io_ipc.h"
using namespace std;


class ProcessState
{
	int mDriverFD;

	ProcessState() : mDriverFD(open_driver()) {}
	ProcessState(const ProcessState&);
	void operator=(const ProcessState&);

	static ProcessState* pInstance;
public:
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

// step3. ���δ��� ����� �ϱ� ���� Ŭ������ �����մϴ�.
// binder_call �Լ��� ����� ��Ʊ� �����Դϴ�.

class IPCThreadState
{
	ProcessState* mProcess;

public :
	// self�� �����Լ��̱� ������ �̱����� ��𿡼��� ��ü�� ������ �� �ִٴ� ������ �ִ�.
	IPCThreadState() : mProcess(ProcessState::self()) {}
	
	// �Ʒ��� �Լ��� binder_call�� ���� �Լ� �Դϴ�.
	int transact(int handle, int code, int* msg, int* reply)
	{
		int driver = mProcess->getDriver();
		return binder_call(driver, msg, reply, handle, code);
	}
}; 

int main()
{
	ProcessState* proc(ProcessState::self());
	IPCThreadState* ipc(new IPCThreadState);

	int msg, reply;

	// ���񽺸� �����ϰų�, ���񽺸� ������ ��� ��, ���� �Ŵ����� �����ϴ� ����̴�. 
	int handle = binder_call(proc->getDriver(),
		(int*)"LedService",
		&reply,
		BINDER_SERVICE_MANAGER,
		SVC_MGR_CHECK_SERVICE);
	while (1)
	{
		// binder_call : ���δ� ����̹��� �����͸� �ۼ����Ѵ�.
		// getchar(); binder_call(proc->getDriver(), &msg, &reply, handle, 1);
		getchar(); ipc->transact(handle, 1, &msg, &reply);
		// getchar(); binder_call(proc->getDriver(), &msg, &reply, handle, 2);
		getchar(); ipc->transact(handle, 2, &msg, &reply);
	}

	return 0;
}