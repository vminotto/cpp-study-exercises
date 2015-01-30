#ifndef BIG_NUM_H
#define BIG_NUM_H

#include <string>
#include <vector>
#include <sstream>
#include <cstdint>

class BigNum {

public:

	BigNum(){values.push_back(0);}
	
	BigNum(std::uint64_t val){
		if (val == 0)
			values.assign(1, 0);
		else{
			while (val > 0){
				values.push_back(val % 10);
				val /= 10;
			}
		}
	}
	
	BigNum &operator+=(BigNum &rhs)
	{
		std::vector<std::uint8_t> *lowAddend, *bigAddend;
		/*If right value is larger, ‘values’ vector will always grow*/
		if (rhs.values.size() > values.size()){
			values.resize(rhs.values.size(), 0);
			lowAddend = &values;
			bigAddend = &rhs.values;
		}
		else{
			values.push_back(0);
			bigAddend = &values;
			lowAddend = &rhs.values;
		}

		std::uint8_t carry = 0;
		size_t i = 0;
		/*First we sum lower part*/
		for (; i < lowAddend->size(); ++i){
			std::uint8_t sum = values[i] + rhs.values[i] + carry;
			values[i] = sum % 10;
			carry = sum / 10;
		}
		/*Now we copy the remaining part*/
		for (; i < bigAddend->size(); ++i){
			/*For 10 10, sum will be 18, at most*/
			std::uint8_t sum = (*bigAddend)[i] + carry;
			values[i] = sum % 10;
			carry = sum / 10;
		}
		this->trimFrontalZeros();
		return *this;
	}
	BigNum &operator*=(BigNum &rhs)
	{
		/*Case when one of the operands is Zero*/
		if (this->isZero())
			return *this;
		else if (rhs.isZero()){
			values.assign(1, 0);	
			return *this;
		}

		size_t maxLen = values.size() + rhs.values.size();
		std::vector<std::uint8_t> product(maxLen);

		size_t lowSize, bigSize;
		std::vector<std::uint8_t> *multiplier, *multiplicand;

		/*The iteration process for the multiplicaiton is developed as the multiplier (A in A*B)
		as the one with more digits.*/
		if (values.size() > rhs.values.size()){
			multiplier = &values;
			multiplicand = &rhs.values;
			bigSize = values.size();
			lowSize = rhs.values.size();
		}
		else{
			multiplier = &rhs.values;
			multiplicand = &values;
			bigSize = rhs.values.size();
			lowSize = values.size();
		}

		/*Implemented as 'values x rhs.values' */
		std::uint8_t carry = 0;
		for (size_t n = 0; n < maxLen; ++n){

			size_t numIters;
			if (maxLen - n - 1< lowSize) numIters = maxLen - n - 1;
			else					     numIters = std::min(n + 1, lowSize);

			std::uint64_t sum = 0;
			for (size_t i = 0; i < numIters; ++i){
				size_t indBelow = i + n + 1 - std::min(n + 1, lowSize);
				size_t indAbove = std::min(n + 1, lowSize) - 1 - i;
				std::uint8_t be = (*multiplier)[indBelow];
				std::uint8_t ab = (*multiplicand)[indAbove];
				sum += be*ab;
			}
			sum += carry;
			product[n] = sum % 10;
			carry = sum / 10;
		}
		values.assign(product.begin(), product.end());
		this->trimFrontalZeros();
		return *this;
	}

public:

	bool isAbsOne() {return values.size() == 1 && values[0] == 1;}
	bool isZero()   {return values.size() == 1 && values[0] == 0;}
	void trimFrontalZeros(){
		size_t newSize = values.size();
		auto it = values.rbegin();
		while (it != values.rend() && *it == 0){
			++it;
			--newSize;
		}
		values.resize(newSize);
	}
	std::string toString(){
		std::stringstream ss;
		std::copy(values.rbegin(), values.rend(), std::ostream_iterator<int>(ss, ""));
		return ss.str();
	}
	std::vector<std::uint8_t> values;
};

std::string longBinaryToDecimalAsString(std::vector<bool> &longBinary){

	BigNum big;
	std::uint64_t n = 0;
	for (bool bit : longBinary){
		if (bit){
			if (n > 63){
				BigNum aux(1);			
				for (std::uint64_t i = 0; i < n / 32; ++i){
					aux *= BigNum(1ULL << 32);
				}
				aux *= BigNum(1ULL << (n % 32));
				big += aux;
			}
			else{
				big += BigNum(1ULL << n);
			}
		}
		++n;
	}
	return big.toString();
}

#endif