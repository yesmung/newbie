

#if 0
// �Ϲ�ȭ ���α׷���(generic programming)
// ������: �κ� �˻��� ���� ���� -> �˻� ������ �Ϲ�ȭ�� ���� ������ �ǹ�
#include <iostream>
using namespace std;

int main()
{
	char* s = "abcdefg";

	char* p = strchr(s, 'c');
	if (p == 0)
		cout << "not found" << endl;
	else
		cout << "found" << endl;
}
#endif

#if 0
// �˻� ������ �Ϲ�ȭ �մϴ�.
#include <iostream>
using namespace std;
// �˻� ����: [fisrt, last)
char* xstrchr(char* first, char* last, char ch)
{
	while (first != last && *first != ch)
		++first;
	return first;	// ã�Ҵٸ� �� ��ġ�� ��ȯ, ��ã���� ���, last
}

// ������: �˻� �������� �Ϲ�ȭ�Ǿ����� Ÿ�Կ� ���ؼ��� �Ϲ�ȭ���� ����

int main()
{
	char* str = "abcdefg";
	char* p = xstrchr(str, str + 4, 'c');
	if (p == str + 4)
		cout << "not found" << endl;
	else
		cout << "found" << endl;
}
#endif



// ��� Ÿ�Կ� ���Ͽ� ó���� �����ϰ� ������ ���ϴ�.
#include <iostream>
using namespace std;

// ������: ȣȯ�Ǵ� Ÿ�Կ� ���Ͽ� �������� �ʽ��ϴ�.
// -> ���ø��� ���ڸ� �ϳ� �� �߰��մϴ�.
template<typename T1, typename T2>
T1* xfind(T1* first, T1* last, T2 ch)
{
	while (first != last && *first != ch)
		++first;
	return first;
}

int main()
{
	double arr[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	double* p1 = xfind(arr, arr + 10, 5);
	if (p1 == arr + 10)
		cout << "not found" << endl;
	else
		cout << "found" << endl;
}

//int main()
//{
//	char* str = "abcdefg";
//	char* p = xfind(str, str + 4, 'c');
//	if (p == str + 4)
//		cout << "not found" << endl;
//	else
//		cout << "found" << endl;
//
//	int arr[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };	// 5�� �˻�
//	int* p1 = xfind(arr, arr + 10, 5);
//	if (p1 == arr + 10)
//		cout << "not found" << endl;
//	else
//		cout << "found" << endl;
//}