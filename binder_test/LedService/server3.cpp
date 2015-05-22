#include "../Android.h"

class BBinder : public IBinder
{
protected:
	// ���� �ݹ��� ����ڰ� ���� �������̵� �ϴ� ���� ���� ���ϹǷ�
	// �ܺ� �ݹ� �������̽��� onTransact�� �����մϴ�.
	int transact(int code, int* msg, int* reply)
	{
		return onTransact(code, msg, reply);
	}
public:
	virtual int onTransact(int code, int* msg, int* reply) { return 0; }
};

// CRTP�� �����ؾ� �� 
template <typename INTERFACE>
// step 3. ���� ����� ���� ������� �����ϱ� ���� �Ʒ��� Ŭ������ �����մϴ�.
class BnInterface : public BBinder, public INTERFACE
{
public :
	IBinder* onAsBinder() { return this; };
};

// ���� ���� �����ڴ� BnInterface�� ��ӹޱ�� ����մϴ�.
// class LedService : public BBinder, public ILedService
class LedService : public BnInterface<ILedService>
{
public:
	// ILedService�� �������̽��� �����մϴ�.
	void ledOn() { cout << "LED ON" << endl; }
	void ledOff() { cout << "LED OFF" << endl; }

	// int myHandler(int code, int* msg, int* reply)
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
	int fd = binder_open(128 * 1024);

	int reply;
	binder_call(fd,
		(int*)"LedService",
		&reply,
		BINDER_SERVICE_MANAGER,
		SVC_MGR_ADD_SERVICE);

	binder_loop(fd, myHandler);
	return 0;
}