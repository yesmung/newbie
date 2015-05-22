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

class BpBinder
{
	int mHandle;
public:
	BpBinder(int handle) : mHandle(handle) {}

	int transact(int code, int* msg, int* reply) {
		return IPCThreadState::self()->transact(mHandle, code, msg, reply);
	}
};

// step 6. 서비스의 핸들을 가져오는 함수를 구현합니다.
// 원래 이 함수는 서비스 매니저의 멤버 함수이며 실제 코드는 다음과 같이 사용됩니다.
// sp<IBinder> binder = defaultServiceManger()->getService(String16("LedService"));

int getService(const char* name)
{
	int reply;
	int handle = binder_call(ProcessState::self()->getDriver(),	
		(int*)name,
		&reply,	
		BINDER_SERVICE_MANAGER,	
		SVC_MGR_CHECK_SERVICE);
	
	return handle;
}

int main()
{
	ProcessState* proc(ProcessState::self());
	// IPCThreadState* ipc(IPCThreadState::self());

	int msg, reply;

	// 서비스 핸들을 가져오는 함수.. 
	// 원래 서비스매니저의 멤버함수이다. (getServiceManger..)
	// 그러나 여기서는 그냥 전역함수로 만들겠다. (구현이 복잡하므로)
	//int handle = binder_call(proc->getDriver(),
	//	(int*)"LedService",
	//	&reply,
	//	BINDER_SERVICE_MANAGER,
	//	SVC_MGR_CHECK_SERVICE);

	int handle = getService("LedService");
	BpBinder* binder = new BpBinder(handle);

	while (1)
	{
		getchar(); binder->transact(1, &msg, &reply);
		getchar(); binder->transact(2, &msg, &reply);
	}

	return 0;
}