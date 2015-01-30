#include <cstdint>
#include <type_traits>
#include <iostream>
#include "BitFunctions.h"
#include <iostream>
#include <vector>
#include <chrono>
#include <algorithm> 
#include "RandomGenerator.h"
#include "BigInt.h"
#include <iomanip> 
#include <numeric>
#include <queue>
#include <bitset>
#include "BitCounter.h"

using uchar = unsigned char;
using uint  = unsigned int;
using ushort = unsigned short;
using ulong = unsigned long;

using namespace std;

int main(){

	std::vector<bool> vv(120, 1);
	cout << ex::longBinaryToDecimalAsString(vv);


	std::cout << ex::decToBase<16>((uint64_t)ex::mask<0, 1>::value, 8) << endl;
	std::cout << ex::decToBase<16>((uint64_t)ex::mask<1, 1>::value, 8) << endl;
	std::cout << ex::decToBase<16>((uint64_t)ex::mask<2, 1>::value, 8) << endl;
	
	cout << endl;
	std::cout << ex::decToBase<16>((uint64_t)ex::mask<0, 2>::value, 16) << endl;
	std::cout << ex::decToBase<16>((uint64_t)ex::mask<1, 2>::value, 16) << endl;
	std::cout << ex::decToBase<16>((uint64_t)ex::mask<2, 2>::value, 16) << endl;
	std::cout << ex::decToBase<16>((uint64_t)ex::mask<3, 2>::value, 16) << endl;

	cout << endl;
	std::cout << ex::decToBase<16>((uint64_t)ex::mask<0, 4>::value, 32) << endl;
	std::cout << ex::decToBase<16>((uint64_t)ex::mask<1, 4>::value, 32) << endl;
	std::cout << ex::decToBase<16>((uint64_t)ex::mask<2, 4>::value, 32) << endl;
	std::cout << ex::decToBase<16>((uint64_t)ex::mask<3, 4>::value, 32) << endl;
	std::cout << ex::decToBase<16>((uint64_t)ex::mask<4, 4>::value, 32) << endl;

	cout << endl;
	std::cout << ex::decToBase<16>((uint64_t)ex::mask<0, 8>::value, 64) << endl;
	std::cout << ex::decToBase<16>((uint64_t)ex::mask<1, 8>::value, 64) << endl;
	std::cout << ex::decToBase<16>((uint64_t)ex::mask<2, 8>::value, 64) << endl;
	std::cout << ex::decToBase<16>((uint64_t)ex::mask<3, 8>::value, 64) << endl;
	std::cout << ex::decToBase<16>((uint64_t)ex::mask<4, 8>::value, 64) << endl;
	std::cout << ex::decToBase<16>((uint64_t)ex::mask<5, 8>::value, 64) << endl;

	std::uint64_t mm = ex::mask<1, 3>::value;
	
	ex::BitCounter bc;

	int64_t ss1 = 5999852385;

	std::uint32_t c1 = ex::count_bits(ss1);
	std::uint32_t c2 = bc.get_naive(ss1);
	std::uint32_t c3 = bc.get_wegner(ss1);
	std::uint32_t c4 = bc.get_sideways_didatic(ss1);
	std::uint32_t c5 = bc.get_sideways_didatic_compact(ss1);
	std::uint32_t c6 = bc.get_builtin(ss1);
	std::uint32_t c7 = bc.get_sideways(ss1);
	//std::uint32_t c8 = bc.get_sideways_super_optmized(ss1);

	long nBits  = ex::count_bits(1234);


	/*after inlining this compiles to a single __builtin_popcount call*/
	long nBits2 = std::bitset<32>(1234).count();

	/*Testing 5.6*/
	std::vector<uint> A(19);
	std::iota(A.begin(), A.end(), 0);
	A.erase(A.begin() + 7);
	//std::shuffle(A.begin(), A.end(), std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count()));
	uint missingUint = ex::getMissing(A);

	uint missingUint2 = ex::findMissingBitUnordered(A, 0);

	using uint = unsigned int;

	// 10  = 00001010
	//-10  = 11110110

	int sdfss = (1 << 4) - 1;
	int sdfsu = ~uint(0) >> (32 - 4);

	char asdsdsf = ~0;
	asdsdsf >>= 3;

	char a12 = 123;
	a12 = ~a12 + 1;
	a12 = ~a12 + 1;

	/*Testing BigInt*/
	ex::BigInt big1 = "33333333333333";
	ex::BigInt big2 = "66666666666666666";
	
	cout << "Testing ex::BigNum addition operations." << endl;
	cout << " " <<   big1  +   big2  << endl;      // (+a) + (+b)
	cout << " " <<   big1  + (-big2) << endl;      // (+a) + (-b)
	cout << " " << (-big1) +   big2  << endl;      // (-a) + (+b)
	cout << " " << (-big1) + (-big2)  << endl;     // (-a) + (-b)
	cout << endl;
	
	cout << "Testing ex::BigNum subtraction operations." << endl;
	cout << " " <<   big1  -   big2  << endl;      // (+a) - (+b)
	cout << " " <<   big1  - (-big2) << endl;      // (+a) - (-b)
	cout << " " << (-big1) -   big2  << endl;      // (-a) - (+b)
	cout << " " << (-big1) - (-big2) << endl;      // (-a) - (-b)

	cout << "Testing ex::BigNum multiplication operations." << endl;
	cout << " " <<   big1  *   big2  << endl;      // (+a) * (+b)
	cout << " " <<   big1  * (-big2) << endl;      // (+a) * (-b)
	cout << " " << (-big1) *   big2  << endl;      // (-a) * (+b)
	cout << " " << (-big1) * (-big2) << endl;      // (-a) * (-b)

	cout << "Testing ex::BigNum division operations." << endl;
	cout << " " <<   big1   /   big2  << endl;      // (+a) / (+b)
	cout << " " <<   big1   / (-big2) << endl;      // (+a) / (-b)
	cout << " " <<  (-big1) /   big2  << endl;      // (-a) / (+b)
	cout << " " <<  (-big1) / (-big2) << endl;      // (-a) / (-b)

	/*Testing Decimal (base 10) to any base converter (at most base 36)*/
	std::uint64_t decimal = 909455;
	std::cout << ex::decToBase<25>(decimal);

	/*Testing 5.8*/
	ex::ByteGenerator br(0, 255);
	std::vector<uchar> screen;
	for (int i = 0; i < 1000; i++){
		screen.push_back(br());
	}
	ex::drawHorizontalLine(screen, 100, 33, 98, 7);

	/*Testing 5.7*/
	/*For an ordered array*/
	std::vector<int> vec;
	for (int i = 0; i < 20; i++) 
		vec.push_back(i);	
	cout << "Vector before erase." << endl;
	for (int i : vec) cout << i << " ";	
	cout << endl << "Vector after erase." << endl;
	vec.erase(vec.begin() + 10);
	for (int i : vec) cout << i << " ";	
	cout << endl << "The missing value is: " << ex::findMissingIntegerOrdered(vec) << endl;

	cout << endl << endl;
	std::vector<uint> vec2;
	for (int i = 0; i < 20; i++)
		vec2.push_back(i);
	shuffle(vec2.begin(), vec2.end(), std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count()));
	cout << "Vector before erase." << endl;
	for (int i : vec2) cout << i << " ";
	cout << endl << "Vector after erase." << endl;
	vec2.erase(vec2.begin() + 10);
	for (int i : vec2) cout << i << " ";
	cout << endl << "The missing value is: " << ex::findMissingBitUnordered(vec2, 0) << endl;

	/*Testing 5.6*/
	int v6 = 5433;
	int r6a = ex::getSwappedEvenOddBits(v6);
	int r6b = ex::getSwappedEvenOddBitsFast(v6);
	cout << "Original:          " << v6 << " (" << ex::decToBinStr(v6, 16) << ")" << endl;
	cout << "Swapped (normal):  " << r6a << "(" << ex::decToBinStr(r6a, 16) << ")" << endl;
	cout << "Swapped (fast)  :  " << r6b << "(" << ex::decToBinStr(r6b, 16) << ")" << endl;

	/*Testing 5.5*/
	int v1 = 345;
	int v2 = 356;
	cout << "Number of bit changes necessary to turn " << v1 << "(" << ex::decToBin(v1) << ")" << endl;
	cout << "                                   into " << v2 << "(" << ex::decToBin(v2) << "): " << endl;
	cout << "Total of " << (ushort)ex::getNumBitChangesForConversion(v1, v2) <<" bits."<<endl;
	cout << "Total of " << (ushort)ex::getNumBitChangesForConversion2(v1, v2) << " bits." << endl;

	/*5.4 Explain what the following code does: ((n & (n-1)) == 0
	it returns true if n is power of 2 or 0*/

	/*Testing 5.3*/
	uint val = ex::binToDec(10001);
	uint next = ex::getNextWithSameNumOnes(val);
	uint prev = ex::getPrevWithSameNumOnes(val);
	cout << "Original value:                 " << val << "  " << ex::decToBin(val) << endl;
	cout << "Next with same ammount of 1s is " << next << endl;
	cout << "Prev with same ammount of 1s is " << prev << endl;

	/*Testing 5.2*/
	double x = 1.0; //1.0 throws an error according to the excercise's constraint
	cout << "Number " << x << " be represented in 32-bit binary: " << ex::printDoubleAsBinary(x) << endl;
	x = 0.3; //0.3 is not exact
	cout << "Number " << x << " be represented in 32-bit binary: " << ex::printDoubleAsBinary(x) << endl;
	x = 0.5; //0.5 is exact in 32 bit representation.
	cout << "Number " << x << " be represented in 32-bit binary: " << ex::printDoubleAsBinary(x) << endl;

	/*Testing 5.1*/
	uchar m = ex::binToDec(11001);
	uchar n = ex::binToDec(10101010);
	//result should be 11100110
	ex::insertMintoN(m, n, 2, 6);
	int res1 = ex::decToBin(n);

	char tt  = 5;
	char mtt = 255 ^ tt;

	int a = ex::binToDec(1111);
	int b = ex::decToBin(12);

	std::cin.get();
	return 0;
}