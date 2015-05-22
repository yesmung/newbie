#include "../Android.h"

class BBinder : public IBinder
{
protected:
	// 내부 콜백을 사용자가 직접 오버라이딩 하는 것은 좋지 못하므로
	// 외부 콜백 인터페이스로 onTransact를 제공합니다.
	int transact(int code, int* msg, int* reply)
	{
		return onTransact(code, msg, reply);
	}
public:
	virtual int onTransact(int code, int* msg, int* reply) { return 0; }
};

// CRTP로 구현해야 함 
template <typename INTERFACE>
// step 3. 다중 상속을 단일 상속으로 변경하기 위해 아래의 클래스를 도입합니다.
class BnInterface : public BBinder, public INTERFACE
{
public :
	IBinder* onAsBinder() { return this; };
};

// 이제 서비스 제작자는 BnInterface를 상속받기로 약속합니다.
// class LedService : public BBinder, public ILedService
class LedService : public BnInterface<ILedService>
{
public:
	// ILedService의 인터페이스를 구현합니다.
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