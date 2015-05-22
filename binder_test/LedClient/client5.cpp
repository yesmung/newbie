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

class IPCThreadState
{
	ProcessState* mProcess;

	IPCThreadState() : mProcess(ProcessState::self()) {}
	static IPCThreadState* pInstance;

	IPCThreadState(const IPCThreadState&);
	void operator=(const IPCThreadState&);

public:
	static IPCThreadState* self()
	{
		if (pInstance == 0)
			pInstance = new IPCThreadState;
		return pInstance;
	}

	int transact(int handle, int code, int* msg, int* reply)
	{
		int driver = mProcess->getDriver();
		return binder_call(driver, msg, reply, handle, code);
	}
};
IPCThreadState* IPCThreadState::pInstance = 0;

// step5. ������ �ڵ��� ���� �� �����ϴ� Ŭ������ �����մϴ�.
class BpBinder
{
	int mHandle;
public :
	BpBinder(int handle) : mHandle(handle) {}

	// ���񽺿��� ������ ���ۿ� ���� �������̽��� ������ �� �� �ֵ���
	// �Ʒ��� ���� �Լ��� �����մϴ�.
	int transact(int code, int* msg, int* reply) {
		return IPCThreadState::self()->transact(mHandle, code, msg, reply);
	}
};

int main()
{
	ProcessState* proc(ProcessState::self());
	IPCThreadState* ipc(IPCThreadState::self());

	int msg, reply;

	int handle = binder_call(proc->getDriver(),
		(int*)"LedService",
		&reply,
		BINDER_SERVICE_MANAGER,
		SVC_MGR_CHECK_SERVICE);
	
	// handle = -1; // bind_call ���� ��, ������ �� ����... ����� ? Ŭ������ �����ϰ� ���� ��� ������ ���� ����.

	// ���� �ڵ��� �����ϰ� �����ϴ� ��ü�� �����մϴ�.
	BpBinder* binder = new BpBinder(handle);

	while (1)
	{
		// �׻� handle�� �ѱ�..	
		// ������ bpBinder�� memeber�� ���� ������, transact�� �ٽ� �ѹ� wrapping �Ͽ�, ���ڸ� ������.. 
		// getchar(); IPCThreadState::self()->transact(handle, 1, &msg, &reply);
		// getchar(); IPCThreadState::self()->transact(handle, 2, &msg, &reply);
		getchar(); binder->transact(1, &msg, &reply);
		getchar(); binder->transact(2, &msg, &reply);
	}

	return 0;
}