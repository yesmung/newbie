
// �Լ� �����ε� �̾߱�

// �Լ� �����ε��� ����: C++ �����Ϸ��� �Լ��� �̸��� ����(name mangling)�ϱ� ����
// �̸� ���� ���: �Լ��� �̸��� �Ķ���͸� ����


// ������� �ڵ� ���� ���
// windows: cl.exe ���ϸ�.cpp /FAs       -> ���ϸ�.asm
//   linux: g++ -S ���ϸ�.cpp            -> ���ϸ�.s

   int square(int x)    { return x*x; }
double square(double x) { return x*x;  }

int main()
{
	int a = square(3);
	double b = square(3.3);
}
