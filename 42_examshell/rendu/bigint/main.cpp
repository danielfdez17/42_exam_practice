
#include "bigint.hpp"
#include <iostream>
using namespace std;

int main(void)
{
// 	bigintStr a(7331);
// 	bigintStr b(21), c, d(1337), e(d);

	// base test
	// std::cout << "a = " << a << "\n";
	// std::cout << "b = " << b << "\n";
	// std::cout << "c = " << c << "\n";
	// std::cout << "d = " << d << "\n";
	// std::cout << "e = " << e << "\n";

	// std::cout << "a + b = " << a + b << "\n";
	// std::cout << "a + c = " << a + c << "\n";
	// std::cout << "c = " << c << "\n";
	// std::cout << "(c += a) = " << (c += a) << "\n";
	// std::cout << "c = " << c << "\n";

	// std::cout << "b = " << b << "\n";
	// std::cout << "++b = " << ++b << "\n";
	// std::cout << "b = " << b << "\n";
	// std::cout << "b++ = " << b++ << "\n";
	// std::cout << "b = " << b << "\n";

	// // b = 23, b << 10 -> 23000000000 + 42 = 23000000042
	// std::cout << "(b << 10) + 42 = " << ((b << 10) + 42) << "\n";
	// std::cout << "d = " << d << "\n";
	// std::cout << "(d <<= 4) = " << (d <<= 4) << ", d: " << d << "\n";
	// std::cout << "(d >>= 2) = " << (d >>= (const bigintStr)2) << ", d: " << d << "\n";

	// std::cout << "a = " << a << "\n"; // a = 42
	// std::cout << "d = " << d << "\n"; // d = 5348

	// std::cout << "(d < a) = " << (d < a) << "\n"; // (d < a) = 0
	// std::cout << "(d > a) = " << (d > a) << "\n"; // (d > a) = 1
	// std::cout << "(d == d) = " << (d == d) << "\n"; // (d == d) = 1
	// std::cout << "(d != a) = " << (d != a) << "\n"; // (d != a) = 1
	// std::cout << "(d <= a) = " << (d <= a) << "\n"; // (d <= a) = 0
	// std::cout << "(d >= a) = " << (d >= a) << "\n"; // (d >= a) = 1

	// extra
	bigintStr x(12345678); bigintStr y(500);
	std::cout << "(x << y) = " << (x << y) << ", x: " << x << ", y: " << y << "\n";
	std::cout << "(x >>= y) = " << (x >>= y) << ", x: " << x << ", y: " << y << "\n";
	std::cout << "(x >= y) = " << (x >= y) << ", x: " << x << ", y: " << y << "\n";

	std::cout << "x= " << (x <<= 5) << ", y= " << (y <<= 12) << "\n";
	std::cout << "(x >= y) = " << (x >= y) << ", x: " << x << ", y: " << y << "\n";

	return (0);
}
