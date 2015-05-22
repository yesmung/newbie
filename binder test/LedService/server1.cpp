
// step 1
#include <iostream>
#include "..\io_ipc.h"
using namespace std;

int myHandler( int code, int* msg, int* reply )
{
	switch( code )
	{
	case 1: cout << "LED ON" << endl; break;
	case 2: cout << "LED OFF" << endl; break;
	}
	return 0;
}


int main()
{   
    // 1. 바인더 드라이버 오픈
    // struct binder_state* bs;
    int fd = binder_open(128*1024); // 128KB

    // 2. 서비스 매니저에게 서버 이름 등록
    int reply;
    binder_call(fd,
               (int*)"LedService",
               &reply,
               BINDER_SERVICE_MANAGER,
               SVC_MGR_ADD_SERVICE);

    // 3. 클라이언트의 요청 대기
    binder_loop(fd, myHandler);
    return 0;
}