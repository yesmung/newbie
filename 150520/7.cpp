// 일반화 프로그래밍(generic programming)
#include <iostream>
using namespace std;

#if 0
// 문제점 : 부분 검색이 되지 않음 -> 검색 범위의 일반화가 되지 않음을 의미
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
// 검색 범위를 일반화 합니다.
// 검색 구간 : [first, last) => [는 포함, )는 미만을 의미함, 즉, 맨 마지막 요소는 검색의 범위에 포함되지는 않는다라는 것을 의미
char* xstrchr(char* first, char* last, char ch)
{
	while (first != last && *first != ch)
		++first;
	return first;	// 찾았다면 그 위치를 반환, 못찾았을 경우에는, last를 리턴하게됨
}

// 문제점 : 검색 구간에 대해서는 일반화되었지만, 타입에 대해서는 일반화되지 않음

int main()
{
	char* str = "abcdefg";
	char* p = xstrchr(str, str + 4, 'c'); // 검색범위는 abcd인데 마지막다음요소 e 이므로 4를 넘김 (0,1,2,3,4)

	if (p == str + 4)
		cout << "not found" << endl;
	else
		cout << "found" << endl;
}
#endif

// 모든 타입에 대해서 처리가 가능하게 변경해 봅시다.
// -> int (셋째인자)에 대해서 암묵적 변환으로 인한 에러, type 체킹.. 컴파일러에서 에러발생
// -> 템플릿의 인자를 하나 더 추가하자. 
template<typename T1, typename T2>
T1* xfind(T1* first, T1* last, T2 ch)
{
	// 배열 형태에만 사용 가능... 예를 들면, List 같은 것은 ++연산이 안됨
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
//	char* p =  xfind(str, str + 4, 'c'); // 검색범위는 abcd인데 마지막다음요소 e 이므로 4를 넘김 (0,1,2,3,4)
//
//	if (p == str + 4)
//		cout << "not found" << endl;
//	else
//		cout << "found" << endl;
//	int arr[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 }; //5을 검색
//	int* p1 = xfind(arr, arr + 10, 5);
//	if (p1 == arr + 10)
//		cout << "not found" << endl;
//	else
//		cout << "found" << endl;
//}