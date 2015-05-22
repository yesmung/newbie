#include <iostream>
#include "..\io_ipc.h"
using namespace std;

// step 2. 바인더 정보 관리 클래스는 2개 이상의 객체를 생성할 이유가 없습니다.
// 그러므로 싱글톤 형태로 구형되어야 합니다.

class ProcessState
{
	int mDriverFD;

	// 1. 생성자는 private 영역에 정의
	ProcessState() : mDriverFD(open_driver()) {}

	// 3. 복사와 대입을 금지하기 위해 복사 생성자와 대입 연산자 함수를 private 영역에 선언
	ProcessState(const ProcessState&);
	void operator=(const ProcessState&);
	
	static ProcessState* pInstance;
public:
	// 2. 유일한 객체를 반환하기 위한 정적 인터페이스를 도입
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


int main()
{
	// Singleton 형태로 만들 필요가 있다.
	// ProcessState* PROC(new ProcessState);
	ProcessState* proc(ProcessState::self());

	int msg, reply;

	int handle = binder_call(proc->getDriver(),
		(int*)"LedService",
		&reply,
		BINDER_SERVICE_MANAGER,
		SVC_MGR_CHECK_SERVICE);
	while (1)
	{
		// 1 : led on, 2: led off
		getchar(); binder_call(proc->getDriver(), &msg, &reply, handle, 1);
		getchar(); binder_call(proc->getDriver(), &msg, &reply, handle, 2);
	}

	return 0;
}