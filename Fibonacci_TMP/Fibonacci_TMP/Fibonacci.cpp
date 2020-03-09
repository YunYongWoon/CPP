#include <iostream>
using namespace std;

template<int N>
struct fib {
	static const int result = fib<N - 1>::result + fib<N - 2>::result;
};

template<>
struct fib<1> {
	static const int result = 1;
}; 

template<>
struct fib<2> {
	static const int result = 1;
};


int main() {
	cout << "5 번째 피보나치 수 :: " << fib<8>::result << endl;  // 5
}