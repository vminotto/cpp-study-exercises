#ifndef BIT_FUNCTIONS_H
#define BIT_FUNCTIONS_H

#include <cstdint>
#include <string>
#include <cmath>
#include <sstream> 
#include <vector>
#include <sstream>

namespace ex{

	long count_bits(long n) {
		unsigned int c; // c accumulates the total bits set in v
		for (c = 0; n; c++)
			n &= n - 1; // clear the least significant bit set
		return c;
	}
	
	char numBitsInChar[256];
	void fillCharsLookupTable(){
		for (int i = 0; i < sizeof(numBitsInChar); ++i){
			numBitsInChar[i] = count_bits(i);
		}
	}

	/*The hamming weight is the number of non-'0' symbols
	in a string. For an integer, that is the number of bits
	set to one.
	/*1010101 = 4
	0001100 = 2

	11000011 - 1 = 11000010
	10011010 - 1 = 10011001

	m1 = 01010101
	m2 = 10101010 -> possible mask results: 2, 8, 10, 32, 34, 40, 42,
	*/
	int getIntHammingWeight(){
		return 0;
	}

	

	char getAscii(char val){
		if (val >= 0 && val <= 9)
			return val + '0';
		else if (val >= 10 && val <= 35)
			return val - 10 + 'A';
		else
			return 0; //A value of 0 actually means the NUL character.
	}

	/*Converts a positive decimal value to any base representation and returns
	the value as a string. Note that for bases larger than 10, ascii characters
	are used. Also note that base 36 is the maximum supported for simplicity of 
	representatoin. We may use symbols [0, 9] and [A, Z], which sum up to a total
	of 36 diferent symbols. If padAs > 0, the left side of the number will be paded
	with zeros as if it was a number compose by 'padAs' bits.*/
	template <std::uint64_t base, class T> std::string decToBase(T in, std::uint8_t padAs = 0)
	{
		static_assert(base <= 36, "Error at function 'ex::decToBase<std::uint64_t>(std::uint64_t)'. The maximum base currently supported is 36. Base largers than such value"
			" would cause a messy representation of the final conversion for lack of proper charatcters.");
		static_assert(base >= 2, "Error at function 'ex::decToBase<std::uint64_t>(std::uint64_t)'. The minimum base supported is 2. Values lower than that cannot express numbers."
			" Base 1 could actually be used, but without much sense.");

		static_assert(std::is_integral<T>::value, "Error at function 'ex::decToBase()'. Input must be integral value.");

		std::stringstream ss;
		while (in > 0){
			size_t remainder = in % base;
			ss << ex::getAscii(remainder);
			in /= base;
		}

		std::uint8_t totalNumChars = padAs / std::log2(base);
		std::uint8_t actualNumChars = ss.tellp();
		for (int i = 0; i < totalNumChars - actualNumChars; ++i){
			ss << "0";
		}
		if (base == 16)
			ss << "x0";
		
		std::string str = ss.str();
		return std::string(str.rbegin(), str.rend());
	}

	using uchar = unsigned char;
	using uint = std::uint32_t;

	using namespace std;

	/*Converts binary to decimal, as in 1011 -> 11*/
	uint64_t binToDec(uint64_t bin, int base = 1){
		if (bin == 0) return 0;
		if (bin == 1) return 1 * base;

		int num = bin % 10 * base;

		num += binToDec(bin / 10, base * 2);

		return num;
	}

	/*Converts decimal to binary, as in 11 -> 1011*/
	uint64_t decToBin(uint64_t dec, uint64_t base = 1){
		if (dec == 0) return 0;

		uint64_t num = dec % 2 * base;
		num += decToBin(dec / 2, base * 10);
		return num;
	}

	template <class T> std::string decToBinStr(T dec, int numBits){
		stringstream ss;
		ss.fill('0');
		ss.width(numBits);
		ss << decToBin(dec);
		return ss.str();
	}

	/*5.1 You are given two 32-bit numbers, N and M, and two bit positions, i and j. Write
	a method to insert M into N such that M starts at bit j and ends at bit i. You can
	assume that the bits j through i have enough space to fit all of M. That is, if
	M = 10011, you can assume that there are at least 5 bits between j and i. You
	would not, for example, have j = 3 and i = 2, because M could not fully fit
	between bit 3 and bit 2.
	EXAMPLE
	Input:  N = 10000000000, M = 10011, i = 2, j = 6
	Output: N = 10001001100*/
	template <class T> void insertMintoN(T m, T &n, uint8_t start, uint8_t end){
		if (start > end) return; //throw error

		uint8_t size = end - start + 1;
		uint8_t nBits = sizeof(T)* 8;
		m = (~0 >> (nBits - size)) & m; //zeroing all bits after 'size'
		m <<= start; //shifting m to the positing it must be inserted in n
		T maskEnd = T(~0) << end + 1;
		T maskFront = T(~0) >> nBits - start;
		T mask = maskEnd | maskFront; //creating mask that zeroes bits from 'start' to 'end' in 'n'
		n = ((n & mask) | m);
	}

	/*	5.2 Given a real number between 0 and 1 (e.g., 0.72) that is passed in as a double,
		print the binary representation. If the number cannot be represented accurately
		in binary with at most 32 characters, print "ERROR	*/
	std::string printDoubleAsBinary(double x){

		if (x >= 1 || x <= 0)
			return "ERROR";

		std::stringstream ss;
		ss << ".";
		ss.fill('0');
		ss.width(32);
		int i = 0;
		while (x > 0)
		{
			if (i > 32)
				return "ERROR";

			x *= 2;
			bool digit = std::floor(x);
			ss << std::left << digit;
			x -= digit;
			i++;
		}
		return ss.str();
	}


	/*5.3 Given a positive integer, print the next smallest and the next largest number
	that have the same number of 1 bits in their binary representation.*/
	std::uint32_t getPrevWithSameNumOnes(std::uint32_t x){
		if (x == 0 || x == std::numeric_limits<std::uint32_t>::max()) //Means x is composed by all zeros or all ones
			throw "Error. No prev value with same ammount of ones exist for this data type. It is either composed by all ones or all zeros.\n";
		if (x == uint(~0) >> 1)
			throw "Error. No prev value with same ammount of ones exist for this data type.\n";

		bool swapped = false;

		uint i = 0;
		uint prev = x;
		uint original = x;
		while (!swapped && i < 32){

			bool thisDigit = x % 2;
			bool nextDigit = (x / 2) % 2;

			if (!thisDigit && nextDigit){
				prev -= thisDigit * std::exp2(i);
				prev -= nextDigit * std::exp2(i + 1);

				prev += thisDigit * std::exp2(i + 1);
				prev += nextDigit * std::exp2(i);
				swapped = true;
			}
			x /= 2;
			i++;
		}
		if (prev == original) //Means we found something like 0...01...1 (any swap would increase x)
			throw "Error. No prev value with same ammount of ones exist for this data type.\n";
		return prev;
	}
	std::uint32_t getNextWithSameNumOnes(std::uint32_t x){
		if (x == 0 || x == std::numeric_limits<std::uint32_t>::max()) //Means x is composed by all zeros or all ones
			throw "Error. No next value with same ammount of ones exist for this data type. It is either composed by all ones or all zeros.\n";
		if (x == uint(~0) << 1)
			throw "Error. No next value with same ammount of ones exist for this data type.\n";

		bool swapped = false;

		uint i = 0;
		uint next = x;
		uint original = x;
		while (!swapped && i < 32){

			bool thisDigit = x % 2;
			bool nextDigit = (x / 2) % 2;

			if (thisDigit && !nextDigit){
				next -= thisDigit * std::exp2(i);
				next -= nextDigit * std::exp2(i + 1);

				next += thisDigit * std::exp2(i + 1);
				next += nextDigit * std::exp2(i);
				swapped = true;
			}
			x /= 2;
			i++;
		}
		if (next == original) //Means we found something like 1...10...0 (any swap would decrease x)
			return next;
	}

	/*5.5 Write a function to determine the number of bits required to convert integer A
	to integer B.
	EXAMPLE
	Input: 31,14
	Output: 2*/
	template <class T> uchar getBitsSum(T x){
		uchar sum = 0;
		for (; x != 0; x >>= 1){
			sum += x & 1;
		}
		return sum;
	}
	template <class T> uchar getBitsSum2(T x){
		int sum = 0;
		for (; x != 0; x = x & (x - 1))
			sum++;
		return sum;
	}

	template <class T> T bitwiseEqual(T a, T b){
		T r = 0;
		for (int i = 0; i < sizeof(T)* 8; i++){
			bool bitA, bitB;

			bitA = a % 2;
			bitB = b % 2;

			r += (bitA == bitB) * (1 << i);

			a >>= 1;
			b >>= 1;
		}
		return r;
	}
	template <class T> uchar getNumBitChangesForConversion(T a, T b){
		return getBitsSum(a ^ b);
	}
	template <class T> uchar getNumBitChangesForConversion2(T a, T b){
		int count = 0;
		for (int c = a ^ b; c != 0; c = c & (c - 1))
			count++;
		return count;
	}

	bool fetchBit(int num, int bitIndex){
		return num & (1 << bitIndex);
	}

	/*5.7 An array A contains all the integers from 0 through n, except for one number which is
	missing. In this problem, we cannot access an entire integer in A with a single operation.
	The elements of A are represented in binary, and the only operation we can use
	to access them is "fetch thejth bit of A[i]," which takes constant time. Write code to
	find the missing integer. Can you do it in 0(n) time?*/
	//int findMissingInteger(const std::vector<int> &vec){
	//	
	//	/*Means the first number is not 0, and therefore it is the missing one*/
	//	if (fetchBit(vec[0], 0))
	//		return 0;
	//	
	//	int numExps = log2(vec.size() + 1 );
	//	int n = 1; //we start at one because we have already checked for '0'
	//	int expOfMissingNum = -1;
	//	for (int i = 0; i < numExps && expOfMissingNum < 0; i++)
	//		for (int j = 0; j < (1 << i) && expOfMissingNum < 0; i++){
	//			bool bit = fetchBit(vec[n], i);
	//			if (!(bit & 1 << i)){ //a number is missing
	//				expOfMissingNum = 
	//			}
	//			n++;
	//		}
	//	}
	//}
	int findMissingIntegerOrdered(const std::vector<int> &vec){
		bool targetBit = 0;
		for (int i = 0; i < vec.size(); i++){
			if (fetchBit(vec[i], 0) != targetBit){ //we found our missing number
				return i;
			}
			targetBit = !targetBit;
		}
	}
	using uint = unsigned int;
	/*This is the solution taken from the book. It works with an unsorted array,
	but it violates a constraint of the excercise, which is to not be able to
	access the entire integer, which is done in the for loop below. The solution
	given in the book is therefore invalid.*/
	int findMissingBitUnordered(std::vector<uint> &vec, int bitIndex){

		if (bitIndex >= sizeof(uint)* 8)
			return 0;

		int val = 0;

		std::vector<uint> zeros, ones;
		for (int n : vec){
			if (fetchBit(n, bitIndex)) 	 ones.push_back(n);
			else						zeros.push_back(n);
		}
		if (zeros.size() <= ones.size()){
			val = findMissingBitUnordered(zeros, bitIndex + 1);
			return (0 << bitIndex) | val;
		}
		else{
			val = findMissingBitUnordered(ones, bitIndex + 1);
			return (1 << bitIndex) | val;
		}
	}



	/*5.7, for unordered array, with iterative approach. My own implementation*/
	uint getMissing(std::vector<uint> &A){
			
		uint N = A.size() + 1;
		uint missingInteger = 0;

		uint numBits = std::ceil(std::log2(N));
		std::vector<uint> bits(numBits, 0);

		for (uint j = 0; j < numBits; ++j){
			uint numZeros = 0, numOnes = 0;
			uint mask = ~uint(0) >> (32 - j);
			for (uint i = 0; i < N - 1; ++i){			
				if (j == 0 || (A[i] & mask) == missingInteger)
				{
					if (fetchBit(A[i], j))
						++numOnes;
					else
						++numZeros;
				}
			}
			bits[j] = numZeros > numOnes;
			missingInteger |= bits[j] << j;
		}
		return missingInteger;
	}

	void printByteAsBits(uchar b, std::ostream &oStream = std::cout, uchar first = 0, uchar last = 7){
		for (char i = last; i >= first; i--)
			oStream << bool(b & (1 << i));
	}

	/*5.8 A monochrome screen is stored as a single array of bytes, allowing eight consecutive
	pixels to be stored in one byte.The screen has width w, where w is divisible
	by 8 (that is, no byte will be split across rows).The height of the screen, of course,
	can be derived from the length of the array and the width. Implement a function
	drawHorizontalLine(byte[] screen, int width, int x1, int x2,
	int y) which draws a horizontal line from (x1, y)to(x2, y).*/
	void drawHorizontalLine(std::vector<uchar> &screen, int width, int x1, int x2, int y){
		
		if (x1 < 0)      x1 = 0;
		if (x1 >= width) x1 = width - 1;
		if (x2 < 0)      x2 = 0;
		if (x2 >= width) x2 = width - 1;
		if (x2 < x1) return;

		int numRows = (int)ceil(screen.size() / (float)width);
		if (y >= numRows) y = numRows - 1;

		stringstream ss;

		int bitFirst = y*width + x1;
		int bitLast = bitFirst + (x2-x1);

		int beforeByte    = (bitFirst / 8);
		int numBitsBefore = (beforeByte + 1) * 8 - bitFirst;

		/*Print remainder at start*/
		ex::printByteAsBits(screen[beforeByte], ss, 0, numBitsBefore - 1);
		
		int endByte = bitLast / 8 - 1;

		/*Print midle*/
		for (uchar j = beforeByte + 1; j <= endByte; j++)
			ex::printByteAsBits(screen[j], ss);

		int numBitsAfter = bitLast - (endByte+1) * 8;
		/*Print remainder at end*/
		ex::printByteAsBits(screen[endByte + 1], ss, 8-numBitsAfter, 7);

		std::string str = ss.str();
		cout << str;
	}

	template <class It> int getSumOfBits(It begin, It end, int bitIndex){
		int sum = 0;
		std::for_each(begin, end, [&](int n){sum += fetchBit(n, bitIndex); });
	}

	/*5.6 Write a program to swap odd and even bits in an integer with as few instructions
	as possible (e.g., bit 0 and bit 1 are swapped, bit 2 and bit 3 are swapped, and
	soon).*/
	template < class T> T getSwappedEvenOddBits(T x){
		T r = 0;
		for (int i = 0; x != 0; x >>= 2, i += 2){
			bool oddBit  = x & 1; //comes before
			bool evenBit = x & 2; // comes after
			r += evenBit * 1 << i; //now goes after
			r += oddBit  * 1 << i+1; //now goes before
		}
		return r;
	}
	template < class T> T getSwappedEvenOddBitsFast(T x){
		return (((x & 0xaaaaaaaa) >> 1) | ((x & 0x55555555) << 1));
	}



	/*
	float
	exponent -> 8 digits
	  matinssa -> 23 digits
	  sign     -> 1 digit (negative = 1, positive = 0)
	  bias      = 127*/
	template <std::uint16_t numMantissaBits, std::uint16_t numExponentBits, class T> std::string decToBinaryString(T x){

		int16_t bias = std::exp2(numExponentBits - 1) - 1;
		int16_t exponent;
		T mantissa;

		std::stringstream ss;
		ss << (x < 0); //adds sign digit

		x = abs(x);

		if (x == 0){
			exponent = -bias;
			mantissa = 0;
		}
		else{
			if (x >= 1.0)
				exponent = (int16_t)std::log2(x);
			else{
				/*Adding epsilon corrects wrong answer for x = 0.5*/
				exponent = -(log2(1.0 / (x + std::numeric_limits<float>::epsilon())) + 1);
			}
		}

		mantissa = x / exp2(exponent) - 1;

		ss << ex::decToBinStr(exponent + bias, numExponentBits); //adds exponent digits

		/*0.1012 = 1 * (1/2^1) + 0 * (1/2^2) + 1 * (1/2^3)*/
		for (int i = 0; i < numMantissaBits; i++)
		{
			mantissa *= 2;
			bool digit = std::floor(mantissa);
			ss << digit;
			mantissa -= digit;
		}
		return ss.str();
	}
	std::string floatToBinaryString(float x){
		return ex::decToBinaryString<23, 8>(x);
	}
	std::string doubleToBinaryString(double x){
		return ex::decToBinaryString<52, 11>(x);
	}
	template <class T> void zeroFirstN(T &x, uint8_t n){
		if (n >= sizeof(T)* 8)
			x = 0;
		else
			x &= T(~0) << n;
	}
	template <class T> void zeroLastN(T &x, uint8_t n){
		if (n <= sizeof(T)* 8)
			x &= T(~0) >> n;
	}
	template <class T> void setLastN(T &x, uint8_t n){
		if (n <= sizeof(T)* 8)
			x |= ~(T(~0) >> n);
	}
	template <class T> void setFirstN(T &x, uint8_t n){
		if (n >= sizeof(T) * 8)
			x = std::numeric_limits<T>::max();
		else
			x |= ~(T(~0) << n);
	}
	
	template <class T> void applyMask(T &x, T mask){
		x |= mask;
	}
	template <class T> void removeMask(T &x, T mask){
		x &= ~mask;
	}

	template <class T> T isMaskSet(T x, T mask){
		return (x & mask) == mask;
	}

};

#endif