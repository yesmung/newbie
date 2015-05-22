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

// step3. 바인더와 통신을 하기 위한 클래스를 도입합니다.
// binder_call 함수의 사용이 어렵기 때문입니다.

class IPCThreadState
{
	ProcessState* mProcess;

public :
	// self가 전역함수이기 때문에 싱글톤은 어디에서는 객체에 접근할 수 있다는 장점이 있다.
	IPCThreadState() : mProcess(ProcessState::self()) {}
	
	// 아래의 함수는 binder_call의 래퍼 함수 입니다.
	int transact(int handle, int code, int* msg, int* reply)
	{
		int driver = mProcess->getDriver();
		return binder_call(driver, msg, reply, handle, code);
	}
}; 

int main()
{
	ProcessState* proc(ProcessState::self());
	IPCThreadState* ipc(new IPCThreadState);

	int msg, reply;

	// 서비스를 조사하거나, 서비스를 얻어오는 기능 즉, 서비스 매니저가 제공하는 기능이다. 
	int handle = binder_call(proc->getDriver(),
		(int*)"LedService",
		&reply,
		BINDER_SERVICE_MANAGER,
		SVC_MGR_CHECK_SERVICE);
	while (1)
	{
		// binder_call : 바인더 드라이버의 데이터를 송수신한다.
		// getchar(); binder_call(proc->getDriver(), &msg, &reply, handle, 1);
		getchar(); ipc->transact(handle, 1, &msg, &reply);
		// getchar(); binder_call(proc->getDriver(), &msg, &reply, handle, 2);
		getchar(); ipc->transact(handle, 2, &msg, &reply);
	}

	return 0;
}