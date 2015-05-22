#include "../Android.h"

// ���� �ڵ�� �ȵ���̵尡 �����ϴ� �ڵ��Դϴ�.
//----------------------------------------------------------------
// �Ʒ��� �ڵ�� ���� �����ڰ� �����ϴ� �ڵ��Դϴ�.

class LedService : public BnInterface<ILedService>
{
public:
	void ledOn() { cout << "LED ON" << endl; }
	void ledOff() { cout << "LED OFF" << endl; }

	int onTransact(int code, int* msg, int* reply)
	{
		switch (code)
		{
		case 1: cout << "LED ON" << endl; break;
		case 2: cout << "LED OFF" << endl; break;
		}
		return 0;
	}
};

int main()
{
	ProcessState* proc(ProcessState::self());

	//int fd = binder_open(128 * 1024);
	//int reply;
	//binder_call(fd,
	//	(int*)"LedService",
	//	&reply,
	//	BINDER_SERVICE_MANAGER,
	//	SVC_MGR_ADD_SERVICE);

	// defaultServiceManager()->addService(String16("LedService'), new LedService);
	addService("LedService", new LedService);

	// binder_loop(fd, myHandler);
	// �����ڵ�
	IPCThreadState::self()->joinThreadPool();

	return 0;
}