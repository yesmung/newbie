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

class BpRefBase
{
	IBinder* mRemote;
public:
	IBinder* remote() { return mRemote; }
	BpRefBase(IBinder* p) : mRemote(p) {}
};

// 아래의 코드는 안드로이드 프레임워크가 제공하는 코드입니다.
// ------------------------------------------------------
// 아래의 코드는 서비스 제작자가 구현하는 코드입니다.


// step 10. 프록시와 서비스가 반드시 구현해야 할 함수를 정의하기 위하여
// 인터페이스를 도입합니다.
class ILedService
{
public :
	virtual void ledOn() = 0;
	virtual void ledOff() = 0;

	// 1. IBinder 타입을 자신의 타입으로 변환하는 함수를 제공해야 합니다.
	// 이 때, 함수는 선언만 합니다. 아래의 asInterface 로 구현한다.
	static ILedService* asInterface(IBinder* p);
	//static ILedService* asInterface(IBinder* p)
	//{
		// return new BpLedService(p); 자식클래스의 이름을 알 수 없다. 
	//}
	
	// IBinder* asBinder() { return this; } 
	// => ILedService는 Ibinder와 연관관계가 없다.결국 IBinder의 정보를 갖고 있는 BpRefbase를 리턴해야함. 결국 자식한테 위임하면됨
	// 2. asBinder를 순수 가상함수로 제공해야 합니다.
	// virtual IBinder* asBinder() = 0; // 반드시 구현해야함 // null object 로 이렇게 하면 안된다. null code를 우회할 수 있도록 해야 함
	// 즉, asBinder가 아닌, onAsBinder가 순수 가산 함수가 되어야 함
	IBinder* asBinder()
	{
		return this ? onAsBinder() : 0;
	}
	virtual IBinder* onAsBinder() = 0;
};

// 이제 프로시 설계자는 BpRefBase 클래스 뿐만이 아니라 ILedService 클래스도
// 상속받아야 하는 규칙이 발생합니다.
class BpLedService : public BpRefBase, public ILedService
{
	int msg, reply;
public:
	// IBinder* asBinder() { return remote(); }
	IBinder* onAsBinder() { return remote(); } // 스펙상해야됨 .. 

	BpLedService(IBinder* p) : BpRefBase(p) {}

	void ledOn() { remote()->transact(1, &msg, &reply); }
	void ledOff() { remote()->transact(2, &msg, &reply); }
};
// 프록시 설계자는 아래의 함수를 구현해야하는 규칙이 발생합니다. 
ILedService* ILedService::asInterface(IBinder* p)
{
	return new BpLedService(p);
}

int main()
{
	ProcessState* proc(ProcessState::self());
	IBinder* binder = getService("LedService");
	// ILedService* ledService = new BpLedService(binder);
	// BpLedService* ledService = new BpLedService(binder);
	// ledService->remote() 가 노출되면 안된다. 즉 ledon off만 프론트앤드 개발자에게 노출되어야 하며
	// 즉, ILecService h 만 주면 됨.

	// 결국은 IBinder -> ILedService 가 되는 것.
	// 즉, asinterface 구현해야됨
	// ILedService* ledService = ILedService*(new BpLedService(binder)); 결국 BpLedService 가 노출됨. 래핑해야됨
	ILedService* ledService = ILedService::asInterface(binder);


	while (1)
	{
		getchar(); ledService->ledOn();
		getchar(); ledService->ledOff();
	}

	return 0;
}