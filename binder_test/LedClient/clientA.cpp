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

// �Ʒ��� �ڵ�� �ȵ���̵� �����ӿ�ũ�� �����ϴ� �ڵ��Դϴ�.
// ------------------------------------------------------
// �Ʒ��� �ڵ�� ���� �����ڰ� �����ϴ� �ڵ��Դϴ�.


// step 10. ���Ͻÿ� ���񽺰� �ݵ�� �����ؾ� �� �Լ��� �����ϱ� ���Ͽ�
// �������̽��� �����մϴ�.
class ILedService
{
public :
	virtual void ledOn() = 0;
	virtual void ledOff() = 0;

	// 1. IBinder Ÿ���� �ڽ��� Ÿ������ ��ȯ�ϴ� �Լ��� �����ؾ� �մϴ�.
	// �� ��, �Լ��� ���� �մϴ�. �Ʒ��� asInterface �� �����Ѵ�.
	static ILedService* asInterface(IBinder* p);
	//static ILedService* asInterface(IBinder* p)
	//{
		// return new BpLedService(p); �ڽ�Ŭ������ �̸��� �� �� ����. 
	//}
	
	// IBinder* asBinder() { return this; } 
	// => ILedService�� Ibinder�� �������谡 ����.�ᱹ IBinder�� ������ ���� �ִ� BpRefbase�� �����ؾ���. �ᱹ �ڽ����� �����ϸ��
	// 2. asBinder�� ���� �����Լ��� �����ؾ� �մϴ�.
	// virtual IBinder* asBinder() = 0; // �ݵ�� �����ؾ��� // null object �� �̷��� �ϸ� �ȵȴ�. null code�� ��ȸ�� �� �ֵ��� �ؾ� ��
	// ��, asBinder�� �ƴ�, onAsBinder�� ���� ���� �Լ��� �Ǿ�� ��
	IBinder* asBinder()
	{
		return this ? onAsBinder() : 0;
	}
	virtual IBinder* onAsBinder() = 0;
};

// ���� ���ν� �����ڴ� BpRefBase Ŭ���� �Ӹ��� �ƴ϶� ILedService Ŭ������
// ��ӹ޾ƾ� �ϴ� ��Ģ�� �߻��մϴ�.
class BpLedService : public BpRefBase, public ILedService
{
	int msg, reply;
public:
	// IBinder* asBinder() { return remote(); }
	IBinder* onAsBinder() { return remote(); } // ������ؾߵ� .. 

	BpLedService(IBinder* p) : BpRefBase(p) {}

	void ledOn() { remote()->transact(1, &msg, &reply); }
	void ledOff() { remote()->transact(2, &msg, &reply); }
};
// ���Ͻ� �����ڴ� �Ʒ��� �Լ��� �����ؾ��ϴ� ��Ģ�� �߻��մϴ�. 
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
	// ledService->remote() �� ����Ǹ� �ȵȴ�. �� ledon off�� ����Ʈ�ص� �����ڿ��� ����Ǿ�� �ϸ�
	// ��, ILecService h �� �ָ� ��.

	// �ᱹ�� IBinder -> ILedService �� �Ǵ� ��.
	// ��, asinterface �����ؾߵ�
	// ILedService* ledService = ILedService*(new BpLedService(binder)); �ᱹ BpLedService �� �����. �����ؾߵ�
	ILedService* ledService = ILedService::asInterface(binder);


	while (1)
	{
		getchar(); ledService->ledOn();
		getchar(); ledService->ledOff();
	}

	return 0;
}