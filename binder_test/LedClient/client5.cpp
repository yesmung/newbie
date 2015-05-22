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

// step5. 서비스의 핸들을 보관 및 관리하는 클래스를 도입합니다.
class BpBinder
{
	int mHandle;
public :
	BpBinder(int handle) : mHandle(handle) {}

	// 서비스에게 데이터 전송에 대한 인터페이스를 간단히 할 수 있도록
	// 아래의 래퍼 함수를 제공합니다.
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
	
	// handle = -1; // bind_call 동작 시, 에러가 날 거임... 어떡하지 ? 클래스를 도입하고 내부 멤버 변수로 갖고 있자.

	// 이제 핸들을 생성하고 관리하는 객체를 생성합니다.
	BpBinder* binder = new BpBinder(handle);

	while (1)
	{
		// 항상 handle을 넘김..	
		// 어차피 bpBinder가 memeber로 갖고 있으니, transact를 다시 한버 wrapping 하여, 인자를 줄이자.. 
		// getchar(); IPCThreadState::self()->transact(handle, 1, &msg, &reply);
		// getchar(); IPCThreadState::self()->transact(handle, 2, &msg, &reply);
		getchar(); binder->transact(1, &msg, &reply);
		getchar(); binder->transact(2, &msg, &reply);
	}

	return 0;
}