#include "../Android.h"

class BpLedService : public BpInterface<ILedService>
{
	int msg, reply;
public:
	BpLedService(IBinder* p) : BpInterface(p) {}

	void ledOn() { remote()->transact(1, &msg, &reply); }
	void ledOff() { remote()->transact(2, &msg, &reply); }
};
IMPLEMENT_META_INTERFACE(LedService);
// IMPLEMENT_META_INTERFACE(ILedService);
//ILedService* ILedService::asInterface(IBinder* p)
//{
//	return new BpLedService(p);
//}

int main()
{
	ProcessState* proc(ProcessState::self());
	IBinder* binder = getService("LedService");
	ILedService* ledService = ILedService::asInterface(binder);

	while (1)
	{
		getchar(); ledService->ledOn();
		getchar(); ledService->ledOff();
	}

	return 0;
}