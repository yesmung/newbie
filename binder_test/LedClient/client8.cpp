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

class IBinder
{
public:
	virtual int transact(int code, int* msg, int* reply) = 0;
};

class BpBinder : public IBinder
{
	int mHandle;
public:
	BpBinder(int handle) : mHandle(handle) {}

	int transact(int code, int* msg, int* reply) {
		return IPCThreadState::self()->transact(mHandle, code, msg, reply);
	}
};

BpBinder* getService(const char* name)
{
	int reply;
	int handle = binder_call(ProcessState::self()->getDriver(),
		(int*)name,
		&reply,
		BINDER_SERVICE_MANAGER,
		SVC_MGR_CHECK_SERVICE);

	return new BpBinder(handle);
}

// step8. 매직 넘버를 함수 호출의 인터페이스로 변경하기 위하여
// 아래의 클래스를 도입합니다.

// 분산 시스템에서 서비스의 기능을 직접 제공하는 것이 아니라 서비스의 기능을
// 대신 요청하는 개념을 프록시(proxy)라고 합니다. 안드로이드 프레임워크에서는
// 일반 클래스와 프로시 클래스를 구분하기 위하여 접두어로 Bp(Binder Proxy)를 
// 사용합니다.
// class LedService
class BpLedService
{
	IBinder* mRemote;
	int msg, reply;
public :
	BpLedService(IBinder* p) : mRemote(p) {}

	// RPC CODE - INTERACE 로 바꾸자
	void ledOn() { mRemote->transact(1, &msg, &reply); }
	void ledOff() { mRemote->transact(2, &msg, &reply); }
};

// 매크로 상수, 심볼릭 상수, 이넘
// 위도 좋지만, LED on off에 대한 함수 인터페이스를 만들어보자. 즉 함수를 만드는 것인데 ....
// 즉, LedClient와 LedService를 구현함에 있어 만들어진 인터페이스를 implement하여 구현하도록 한다.
int main()
{
	ProcessState* proc(ProcessState::self());
	IBinder* binder = getService("LedService");
	BpLedService* ledService = new BpLedService(binder);

	while (1)
	{
		// int msg, reply;
		// getchar(); binder->transact(1, &msg, &reply);
		// getchar(); binder->transact(2, &msg, &reply);
		
		// proxy : 분산시스템에서 서비스의 기능을 대신 요청해줌
		// 안드로이드 : Bp 를 붙임.. 실제 서비스의 기능을 요청하는 proxy를 나타낸다.
		getchar(); ledService->ledOn();
		getchar(); ledService->ledOff();
	}

	return 0;
}