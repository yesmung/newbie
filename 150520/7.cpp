// �Ϲ�ȭ ���α׷���(generic programming)
#include <iostream>
using namespace std;

#if 0
// ������ : �κ� �˻��� ���� ���� -> �˻� ������ �Ϲ�ȭ�� ���� ������ �ǹ�
int main()
{
	char* s = "abcdefg";
	char* p = strchr(s, 'c');
	if (p == 0)
		cout << "not found" << endl;
	else
		cout << *p << "found" << endl;
}
#endif


#if 0
// �˻� ������ �Ϲ�ȭ �մϴ�.
// �˻� ���� : [first, last) => [�� ����, )�� �̸��� �ǹ���, ��, �� ������ ��Ҵ� �˻��� ������ ���Ե����� �ʴ´ٶ�� ���� �ǹ�
char* xstrchr(char* first, char* last, char ch)
{
	while (first != last && *first != ch)
		++first;
	return first;	// ã�Ҵٸ� �� ��ġ�� ��ȯ, ��ã���� ��쿡��, last�� �����ϰԵ�
}

// ������ : �˻� ������ ���ؼ��� �Ϲ�ȭ�Ǿ�����, Ÿ�Կ� ���ؼ��� �Ϲ�ȭ���� ����

int main()
{
	char* str = "abcdefg";
	char* p = xstrchr(str, str + 4, 'c'); // �˻������� abcd�ε� ������������� e �̹Ƿ� 4�� �ѱ� (0,1,2,3,4)

	if (p == str + 4)
		cout << "not found" << endl;
	else
		cout << "found" << endl;
}
#endif

// ��� Ÿ�Կ� ���ؼ� ó���� �����ϰ� ������ ���ô�.
// -> int (��°����)�� ���ؼ� �Ϲ��� ��ȯ���� ���� ����, type üŷ.. �����Ϸ����� �����߻�
// -> ���ø��� ���ڸ� �ϳ� �� �߰�����. 
template<typename T1, typename T2>
T1* xfind(T1* first, T1* last, T2 ch)
{
	// �迭 ���¿��� ��� ����... ���� ���, List ���� ���� ++������ �ȵ�
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

	return 0;
}

//int main()
//{
//	char* str = "abcdefg";
//	char* p =  xfind(str, str + 4, 'c'); // �˻������� abcd�ε� ������������� e �̹Ƿ� 4�� �ѱ� (0,1,2,3,4)
//
//	if (p == str + 4)
//		cout << "not found" << endl;
//	else
//		cout << "found" << endl;
//	int arr[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 }; //5�� �˻�
//	int* p1 = xfind(arr, arr + 10, 5);
//	if (p1 == arr + 10)
//		cout << "not found" << endl;
//	else
//		cout << "found" << endl;
//}