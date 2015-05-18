#if 0

// windows thread api
#include <stdio.h>
#include <Windows.h>
#include <iostream>

using namespace std;

/* 책 */
/*
class Thread
{
public :
	void run() { CreateThread(0, 0, _thread_main, this, 0, 0); }
	static DWORD __stdcall _thread_main(void* arg)
	{
		Thread* self = static_cast<Thread*>(arg);
		self->thread_main();
		return 0;
	}
	virtual void thread_main() {}
};

class SubThread : public Thread
{
public :
	virtual void thread_main()
	{
		for (int i = 0; i < 10; i++)
		{
			cout << "Sub : " << i << endl;
			Sleep(1000);
		}
	}
};

int main()
{
	SubThread t;
	t.run();
	getchar();
}
*/


/*
thread 기본..
unsigned long __stdcall thread_main(void* arg)
{
	const char* s = static_cast<const char*>(arg);

	for (int i = 0; i < 10; i++)
	{
		printf("%s\n", s);
		Sleep(1000);
	}
	return 0;
}

void main()
{
	// callback 함수 : 운영체제에 의해 호출을 당하는 함수
	// callback의 구현 c언어에서는 함수 포인터를 사용
	// c++ 에서는 위의 방법 + 가상함수!
	CreateThread(0, 0, thread_main, "hello, ", 0, 0);
	getchar();
}
*/
#endif

#include <stdio.h>
#include <Windows.h>

class Thread
{
	// static Thread* self;
	// multi thread 에서는 safety 하지 않다. 

protected :
	virtual unsigned long threadMain()
	{
		return 0;
	}
	// wrapper, 일반함수와 동일하도록 static
	static unsigned long __stdcall __threadMain(void* arg)
	{
		// multi thread를 위한 지역 변수 사용
		// Thread* self = (Thread*)arg // 타입 캐스팅 꺼짐. 이렇게 사용하면 안된다.
		Thread* self = static_cast<Thread*>(arg);
		self = static_cast<Thread*>(arg);
		self->threadMain();
		return 0;
	}

public :
	void start() // void start(Thread* const this)
	{
		// self = this;
		CreateThread(0, 0, __threadMain, this, 0, 0);
	}
};
// Thread* Thread::self = 0; // 윗 선언에 맞는 초기화

class FileDownThread : public Thread
{
public :
	unsigned long threadMain()
	{
		for (int i = 0; i <= 10; i++)
		{
			system("cls");
			printf("%d %% downloading...\n", i * 10);
			Sleep(1000);
		}
		printf("completed!\n");
		return 0;
	}
};

void main()
{
	FileDownThread t; // api임
	t.start();
	getchar();
}