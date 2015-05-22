

#if 0
// 일반화 프로그래밍(generic programming)
// 문제점: 부분 검색이 되지 않음 -> 검색 범위의 일반화가 되지 않음을 의미
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
// 검색 범위를 일반화 합니다.
#include <iostream>
using namespace std;
// 검색 구간: [fisrt, last)
char* xstrchr(char* first, char* last, char ch)
{
	while (first != last && *first != ch)
		++first;
	return first;	// 찾았다면 그 위치를 반환, 못찾았을 경우, last
}

// 문제점: 검색 구간에는 일반화되었지만 타입에 대해서는 일반화되지 않음

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



// 모든 타입에 대하여 처리가 가능하게 변경해 봅니다.
#include <iostream>
using namespace std;

// 문제점: 호환되는 타입에 대하여 동작하지 않습니다.
// -> 템플릿의 인자를 하나 더 추가합니다.
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
//	int arr[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };	// 5를 검색
//	int* p1 = xfind(arr, arr + 10, 5);
//	if (p1 == arr + 10)
//		cout << "not found" << endl;
//	else
//		cout << "found" << endl;
//}