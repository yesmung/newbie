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


// step7. �ڵ��� �����ϴ� ��� Ŭ������ �ϳ��� Ÿ������ ó���ϱ� ���Ͽ�
// �θ� Ŭ����(���)�� �����մϴ�.
// ���� ���� �Լ��� ���� �ִ� -> �̰��� Ŭ������ ������ �ƴ� �������̽��� �����̴�. ��, I�� �ٿ�.
class IBinder
{
	// �ڽ��� �Ӽ��� �ݵ�� �θ�κ��� �;� �ȴ�.
	// ��, �ڽ��� �������̽��� �ݵ�� �θ� �����ؾ� �Ѵ�.
public :
	// virtual : static binding -> dynamic binding ����
	// �ڽ��� �ݵ�� �����ؾߵ� pure virtual function
	virtual int transact(int code, int* msg, int* reply) = 0; 
};

// ���� �ڵ��� �����ϴ� Ŭ������ IBinder Ŭ������ ��� �ޱ�� ��� �մϴ�.
class BpBinder : public IBinder
{
	int mHandle;
public:
	BpBinder(int handle) : mHandle(handle) {}

	int transact(int code, int* msg, int* reply) {
		return IPCThreadState::self()->transact(mHandle, code, msg, reply);
	}
};

// BpBinder�� ���� ��������
// int getService(const char* name)
BpBinder* getService(const char* name)
{
	int reply;
	int handle = binder_call(ProcessState::self()->getDriver(),
		(int*)name,
		&reply,
		BINDER_SERVICE_MANAGER,
		SVC_MGR_CHECK_SERVICE);

	// return handle;
	return new BpBinder(handle);
}

int main()
{
	ProcessState* proc(ProcessState::self());
	
	// int handle = getService("LedService");
	// BpBinder* binder = new BpBinder(handle);
	// BpBinder* binder = getService("LedService");
	IBinder* binder = getService("LedService");

    while (1)
	{
		int msg, reply;
		getchar(); binder->transact(1, &msg, &reply);
		getchar(); binder->transact(2, &msg, &reply);
	}

	return 0;
}