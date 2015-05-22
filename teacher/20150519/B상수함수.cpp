
// 상수 함수를 오버로딩 하는 이유
#include <iostream>
#include <string>
using namespace std;

class Vector
{
	int* arr;
	int sz;

public:
	Vector(int s = 5) : sz(5) { arr = new int[5]; }
	~Vector() { delete[] arr; }

	int& operator[](int i)       { return arr[i]; }
	int  operator[](int i) const { return arr[i]; }
};

int main()
{
	Vector v;
	// v[0] = 0;	// v.operator[](0) = 0;
	cout << v[0] << endl;

	// 아래의 코드가 동작하도록 변경해 보세요 ;D
	const Vector v1;
	cout << v1[0] << endl;
	
}