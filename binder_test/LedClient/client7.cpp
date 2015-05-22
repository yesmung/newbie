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


// step7. 핸들을 관리하는 모든 클래스를 하나의 타입으로 처리하기 위하여
// 부모 클래스(상속)을 도입합니다.
// 순수 가상 함수를 갖고 있다 -> 이것은 클래스의 개념이 아닌 인터페이스의 개념이다. 즉, I를 붙여.
class IBinder
{
	// 자식의 속성을 반드시 부모로부터 와야 된다.
	// 즉, 자식의 인터페이스는 반드시 부모가 제공해야 한다.
public :
	// virtual : static binding -> dynamic binding 위함
	// 자식이 반드시 구현해야됨 pure virtual function
	virtual int transact(int code, int* msg, int* reply) = 0; 
};

// 이제 핸들을 관리하는 클래스는 IBinder 클래스를 상속 받기로 약속 합니다.
class BpBinder : public IBinder
{
	int mHandle;
public:
	BpBinder(int handle) : mHandle(handle) {}

	int transact(int code, int* msg, int* reply) {
		return IPCThreadState::self()->transact(mHandle, code, msg, reply);
	}
};

// BpBinder를 만들어서 리턴하자
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