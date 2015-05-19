#if 0

// windows thread api
#include <stdio.h>
#include <Windows.h>
#include <iostream>

using namespace std;

/* å */
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
thread �⺻..
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
	// callback �Լ� : �ü���� ���� ȣ���� ���ϴ� �Լ�
	// callback�� ���� c������ �Լ� �����͸� ���
	// c++ ������ ���� ��� + �����Լ�!
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
	// multi thread ������ safety ���� �ʴ�. 

protected :
	virtual unsigned long threadMain()
	{
		return 0;
	}
	// wrapper, �Ϲ��Լ��� �����ϵ��� static
	static unsigned long __stdcall __threadMain(void* arg)
	{
		// multi thread�� ���� ���� ���� ���
		// Thread* self = (Thread*)arg // Ÿ�� ĳ���� ����. �̷��� ����ϸ� �ȵȴ�.
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
// Thread* Thread::self = 0; // �� ���� �´� �ʱ�ȭ

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
	FileDownThread t; // api��
	t.start();
	getchar();
}