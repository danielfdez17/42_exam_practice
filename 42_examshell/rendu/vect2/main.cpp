#include "vect2.hpp"
#include <iostream>

int main()
{
	vect2 v1; // 0, 0
	vect2 v2(1, 2); // 1, 2
	const vect2 v3(v2); // 1, 2
	vect2 v4 = v2; // 1, 2

	std::cout << "v1: " << v1 << std::endl;
	std::cout << "v1: " << "{" << v1[0] << ", " << v1[1] << "}" << std::endl;
	std::cout << "v2: " << v2 << std::endl;
	std::cout << "v3: " << v3 << std::endl;
	std::cout << "v4: " << v4 << std::endl;
	std::cout << "v4++: " << v4++ << std::endl; // 2, 3
	std::cout << "v4: " << v4 << std::endl;
	std::cout << "++v4: " << ++v4 << std::endl; // 3, 4
	std::cout << "v4: " << v4 << std::endl;
	std::cout << "v4--: " << v4-- << std::endl; // 2, 3
	std::cout << "v4: " << v4 << std::endl;
	std::cout << "--v4: " << --v4 << std::endl; // 1, 2
	std::cout << "v4: " << v4 << std::endl;
	std::cout << "v2 += v3: " << (v2 += v3) << "\n"; // 2, 4
	std::cout << "v1 -= v2: " << (v1 -= v2) << "\n"; // -2, -4
	std::cout << "v2 = v3 + v3 *2: " << (v2 = v3 + v3 *2) << "\n"; // 3, 6
	std::cout << "v2 = 3 * v2: " << (v2 = 3 * v2) << "\n"; // 9, 18
	std::cout << "v2 += v2 += v3: " << (v2 += v2 += v3) << "\n"; // 20, 40
	std::cout << "v1 *= 42: " << (v1 *= 42) << "\n"; // -84, -168
	std::cout << "v1 = v1 - v1 + v1: " << (v1 = v1 - v1 + v1) << "\n";
	std::cout << "v1: " << v1 << std::endl;
	std::cout << "v2: " << v2 << std::endl;
	std::cout << "-v2: " << -v2 << std::endl;
	std::cout << "v1[1]: " << v1[1] << std::endl;
	v1[1] = 12;
	std::cout << "v1[1]: " << v1[1] << std::endl;
	std::cout << "v3[1]: " << v3[1] << std::endl;
	v1[0] = 10;
	v2[0] = 10;
	std::cout << "v1: " << v1 << std::endl;
	std::cout << "v2: " << v2 << std::endl;
	std::cout << "v1 == v2: " << (v1 == v2) << std::endl;
	std::cout << "v1 == v1: " << (v1 == v1) << std::endl;
	std::cout << "v1 != v2: " << (v1 != v2) << std::endl;
	std::cout << "v1 != v1: " << (v1 != v1) << std::endl;
}