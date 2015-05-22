
#if 0
// windows thread api
#include <stdio.h>
#include <Windows.h>

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
	CreateThread(0, 0, thread_main, "hello", 0, 0);
	getchar();
}
#endif


#include <stdio.h>
#include <Windows.h>


// 아래 스레드 클래스의 나머지를 구현해 보세여 :D
class Thread
{
// 	static Thread* self;
protected:
	virtual unsigned long threadMain() { return 0; }

	static unsigned long __stdcall __threadMain(void* arg)
	{
		Thread* self = static_cast<Thread*>(arg);	// (Thread*)arg;
		self->threadMain();
		return 0;
	}

public:
	void start()	// void start(Thread* const this)
	{
		// self = this;
		CreateThread(0, 0, __threadMain, this, 0, 0);
	}
};
// Thread* Thread::self = 0;

class FileDownThread : public Thread
{
public:
	unsigned long threadMain() {
		for (int i = 0; i <= 10; i++) {
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
	FileDownThread t;
	t.start();
	getchar();
}