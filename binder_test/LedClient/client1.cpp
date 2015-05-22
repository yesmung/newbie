#include <iostream>
#include "..\io_ipc.h"
using namespace std;

// step 1. 바인더 드라이버 정보를 저장 및 관리하기 위하여 클래스를 도입한다.
class ProcessState
{
	int mDriverFD;
public :
	// ProcessState() { mDriverFD = binder_open(128 * 1024); }
	// ProcessState() { mDriverFD = open_driver();  }
	ProcessState() : mDriverFD(open_driver()) {}
	int getDriver() { return mDriverFD; }

	// 고급 개발자를 위한 저수준 API
	int open_driver() { return binder_open(128 * 1024); }
};


int main()
{
    // const int fd = binder_open(128*1024);
	// ProcessState proc; // stack overflow 가능성 있음, heap 에다가 만들자
	// ProcessState* proc = new ProcessState;
	// == 위와 동치, 다만 안드로이드 소스 코드는 아래와 같다
	ProcessState* PROC(new ProcessState);


    int msg, reply;

	// BINDER_SERVICE_MANAGER 라는 서비스가 있는지 체크해달라
	// 그리고 "LedService"라는 문자열을 서비스 메니저에게에게 보내달라
	// BINDER_SERVICE_MANAGER의 SVC_MGR_CHECK_SERVICE가
	// 호출될 때의 파라미터 ("LedService")
	// 리턴값은 &replay
	// fd는 바인더 드라이버를 오픈하기 위한 값(위에 binder_open)
	// fd -> proc.getDriver()
	// proc->getDriver()
    int handle = binder_call(proc->getDriver(),
                            (int*)"LedService",
                            &reply,
                            BINDER_SERVICE_MANAGER,
                            SVC_MGR_CHECK_SERVICE);
    while(1)
    {
		// 1 : led on, 2: led off
		getchar(); binder_call(proc->getDriver(), &msg, &reply, handle, 1);
		getchar(); binder_call(proc->getDriver(), &msg, &reply, handle, 2);
    }

    return 0;
}