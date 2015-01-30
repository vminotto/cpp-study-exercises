#ifndef BIG_INT_H
#define BIG_INT_H

/*BigNum Implementation
http://stackoverflow.com/questions/2664219/what-is-a-convenient-base-for-a-bignum-library-primality-testing-algorithm
Notes on choosing a ‘base’ value. Base 10 would be the most convenient to use, for the reasons it is easy to overload I/O functions and is more natural to humans, making very intuitive to implement multiplication and division. However, this would not be very efficient in terms of space (array storage). The lowest integer size is an ‘unsigned char’ which may represent up to 256 distinct values (from 0 to 255). By using base 10, we would be underusing such capacity, because we would be representing only 10 different values (from 0 to 9), therefore it is a waste of memory space to use base 10 in any modern system. The recommendation, therefore, is to use a power of two value that is no more than half the maximum word size. In x86, the largest word size would be 32 bits, so each digit should not be more than 16 bits, that is, each digit should be represented one symbol that is picked among a pool of 2^16 possible symbols. Using unsigned long long, one could use a base of 2^32. The reason for choosing half the word size, at most, is that by multiplying together two values that are over half the wordsize, an overflow would happen. In the case of 2^16 * 2^16, the maximum value is 2^32, not overflowing the 32bit limit of the unsigned int variable. If we add sums of products, if might overflow anyway, so considering fewer bits is also a good option. 2^8 256, with 32 bits variables would be a good choice, for example.

Using base 10 allows us to implement school-like multiplication algorithms. Using higher bases, there are other algorithms that perform faster, such as Karatsuba’s.
*/

#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <cstdint>


/*Multi-precision stuff*/
namespace ex{

	using uchar = unsigned char;

	class BigInt {

	public:

		/*Just in case we want to change the implementation later without needing to
		re-work too much code.*/
		const int base = 10;

		/*All other constructors will call this one, which ensures our BigInt is always
		created with a valid initial value, which we chose to be 0.*/
		BigInt(){
			/*We start by pre-allocating twice the size of the maximum supported
			integer.*/
			values.reserve(2 * sizeof(std::uint64_t) / sizeof(uchar));
			values.push_back(0);
		}

		BigInt(std::uint64_t val) : BigInt() { this->assign(val); }
		BigInt(std::uint32_t val) : BigInt() { this->assign(val); }
		BigInt(std::uint16_t val) : BigInt() { this->assign(val); }
		BigInt(std::uint8_t val) : BigInt()  { this->assign(val); }
		BigInt(std::int64_t val) : BigInt()	 {
			this->assign(std::abs(val));
			if (val < 0) sign = true;
		}
		BigInt(std::int32_t val) : BigInt() {
			this->assign(std::abs(val));
			if (val < 0) sign = true;
		}
		BigInt(std::int16_t val) : BigInt() {
			this->assign(std::abs(val));
			if (val < 0) sign = true;
		}
		BigInt(std::int8_t val) : BigInt() {
			this->assign(std::abs(val));
			if (val < 0) sign = true;
		}
		BigInt(double val) : BigInt() {
			this->assign(static_cast<std::int64_t>(val));
		}
		BigInt(float val) : BigInt() {
			this->assign(static_cast<std::int64_t>(val));
		}

		/*Just making it explicit we want to use the default copy ctor. For this class, it should work.*/
		BigInt(const BigInt &other) = default;
		BigInt(const std::string &str);

		/*Eventhough we do not use const parameter, we make sure we don’t modify them. Using non-const references allows us to perform the operations without the need for temporary copies, in some cases.*/
		BigInt &operator+=(BigInt &rhs);
		BigInt &operator-=(BigInt &rhs);
		BigInt &operator*=(BigInt &rhs);
		BigInt &operator/=(BigInt &rhs);
		BigInt &operator++()   { return operator+=(BigInt(1)); }
		BigInt &operator++(int){ BigInt tmp(*this); operator++(); return tmp; }
		BigInt &operator--()   { return operator-=(BigInt(1)); }
		BigInt &operator--(int){ BigInt tmp(*this); operator--(); return tmp; }
		BigInt &operator=(const BigInt &rhs){
			this->assign(rhs);
			return *this;
		}
		BigInt  operator-() const {
			BigInt bigInt(*this);
			bigInt.neg();
			return bigInt;
		}
		bool	operator!() const {
			return this->isZero();
		}
		explicit operator bool() const {
			return !operator!();
		};

		/*Comparison*/
		friend bool operator==(const BigInt &lhs, const BigInt &rhs){
			if (lhs.sign == rhs.sign && lhs.isAbsEqual(rhs))	return true;
			else												return false;
		}
		friend bool operator!=(const BigInt &lhs, const BigInt &rhs){
			return !(lhs == rhs);
		}
		friend bool operator>=(const BigInt &lhs, const BigInt &rhs){
			if (lhs > rhs || rhs == lhs)	return true;
			else							return false;
		}
		friend bool operator<=(const BigInt &lhs, const BigInt &rhs){
			if (lhs < rhs || rhs == lhs)	return true;
			else							return false;
		}
		friend bool operator>(const BigInt &lhs, const BigInt &rhs){
			if (lhs.sign == rhs.sign && lhs.isAbsGreater(rhs)) return true;
			else if (!lhs.sign)				   return true;
			else 						       return false;
		}
		friend bool operator<(const BigInt &lhs, const BigInt &rhs){
			if (lhs.sign == rhs.sign && lhs.isAbsLesser(rhs)) return true;
			else if (lhs.sign)				  return true;
			else 						      return false;
		}

		/*First parameter is not const and non reference so we may define + using +=*/
		friend BigInt operator+(BigInt lhs, BigInt &rhs){
			return lhs += rhs;
		}
		friend BigInt operator-(BigInt lhs, BigInt &rhs){
			return lhs -= rhs;
		}
		friend BigInt operator*(BigInt lhs, BigInt &rhs){
			return lhs *= rhs;
		}
		friend BigInt operator/(BigInt lhs, BigInt &rhs){
			return	lhs /= rhs;
		}

		friend std::ostream &operator<<(std::ostream &out, const BigInt &rhs);
		friend std::istream &operator>>(std::istream &in,		 BigInt &rhs);


		bool isZero() const {
			return values.size() == 1 && values[0] == 0;
		}
		bool isPosOne() const {
			return values.size() == 1 && values[0] == 1 && !sign;
		}
		bool isNegOne() const {
			return values.size() == 1 && values[0] == 1 && sign;
		}
		bool isAbsOne() const {
			return values.size() == 1 && values[0] == 1;
		}

		std::string toString(){
			std::stringstream ss;
			std::copy(values.rbegin(), values.rend(), std::ostream_iterator<int>(ss, ""));
			return ss.str();
		}

	protected:

		int8_t compare(const BigInt &b)	   const;
		bool isAbsGreater(const BigInt &b) const;
		bool isAbsLesser(const BigInt &b)  const;
		bool isAbsEqual(const BigInt &b)   const;

		/*This might need to be called after subtraction or divison, where our BigInt
		may get reduced.*/
		void trimFrontalZeros(){
			size_t newSize = values.size();
			auto it = values.rbegin();
			while (it != values.rend() && *it == 0){
				++it;
				--newSize;
			}
			//Leading zeroes are automatically removed by the resize method.
			values.resize(newSize);
		}

		void assign(std::uint64_t val);
		void assign(const BigInt &val){
			values.assign(val.values.begin(), val.values.end());
			sign = val.sign;
		}

		void neg(){ this->sign = !sign; }
		void abs(){ this->sign = false; }

		/*The least significatn digit is at values[0]*/
		std::vector<uchar> values;

		//Default values is +0
		bool sign = false;
	};



	void BigInt::assign(std::uint64_t val)
	{
		if (val == 0)
			values.assign(1, 0);
		else{
			/*Reducing the logical size wont cause reallocation.*/
			values.resize(0);
			sign = false;
			while (val > 0){
				values.push_back(val % base);
				val /= base;
			}
		}
	}
	/*Here we assume the right-most digit is the least significant one (same as read by humans).
	In other words, str.back() is the least significant one.*/
	BigInt::BigInt(const std::string &str)
	{
		if (str.empty())
			values.assign(1, 0);
		else{
			auto rend = str.rend();
			if (str.front() == '-'){
				--rend;
				this->sign = true;
			}
			else if (str.front() == '+'){
				--rend;
				this->sign = false;
			}

			values.reserve(str.size()); //pre-allocation
			/*Inserting in reverse order*/
			for (auto it = str.rbegin(); it != rend; ++it){
				char c = *it;
				if (c >= '0' && c <= '9')
					values.push_back(static_cast<uchar>(c - '0'));
				else
					throw std::string("Error at 'BigInt::BigInt(const "
					"std::string&)': only number characters supported\n.");
			}
		}
	}
	/*This += needs only to be implemented for the BigInt type. If we want to make addition work with other types, 
	like double, all we need to do is provide a constructor for ‘double’, so it gets implicitly converted to BigInt 
	and this function eds up being called. As for the + operator, we may implemented it through this one.*/
	BigInt &BigInt::operator+=(BigInt &rhs)
	{

		if (rhs.isZero()){
			return *this;
		}
		else if (this->isZero()){
			return (*this = rhs);
		}

		values.push_back(0);
		if (sign == rhs.sign) //(+a+b) or (-a-b) cases
		{
			std::vector<uchar> *lowAddend, *bigAddend;
			/*If right value is larger, ‘values’ vector will always grow*/
			if (rhs.isAbsGreater(*this)){
				values.resize(rhs.values.size(), 0);
				lowAddend = &values;
				bigAddend = &rhs.values;
			}
			/*If left is larger, ‘values’ vector may only grow by one. This will happen only if the addition generates a carry. Checking for this would be costly, so we just grow ‘values’ by one always. Ex.: 9999 + 1 = 10000*/
			else{
				bigAddend = &values;
				lowAddend = &rhs.values;
			}
			uchar carry = 0;
			size_t i = 0;
			/*First we sum lower part*/
			for (; i < lowAddend->size(); ++i){
				/*For base 10, sum will be 18, at most*/
				uchar sum = values[i] + rhs.values[i] + carry;
				values[i] = sum % base;
				carry = sum / base;
			}
			/*Now we copy the remaining part*/
			for (; i < bigAddend->size(); ++i){
				/*For base 10, sum will be 18, at most*/
				uchar sum = (*bigAddend)[i] + carry;
				values[i] = sum % base;
				carry = sum / base;
			}
			/*We do not need to touch the sign, since it will remain the same, for both
			either both negative or both positive values*/
			this->trimFrontalZeros();
		}
		/*The below approach allows us to perform the addition (forwarded to a subtraction) without 
		needing to create temporary copies of rhs, and also have ‘this’ altered directly.*/
		else{ /*Operands with different signs*/
			if (rhs.sign){ //a + (-b)
				rhs.abs();
				operator-=(rhs);
				rhs.neg();
				sign = this->isAbsLesser(rhs);
			}
			else if (sign){ //-a + b
				abs();
				operator-=(rhs);
				sign = this->isAbsGreater(rhs);
			}
		}
		return *this;
	}
	BigInt &BigInt::operator-=(BigInt &rhs)
	{
		if (this->isZero()){
			this->assign(rhs);
			this->neg();
			return *this;
		}
		else if (rhs.isZero()){
			return *this;
		}

		/*Subtraction will always shrink the ‘values’ vector, except in cases where both
		operands (in the final expression) have the same signal.*/
		if (sign != rhs.sign){ /*[(-a)-(+b) = -a -b] or [(+a)-(-b) = +a +b]*/
			rhs.neg();
			operator+=(rhs);
			rhs.neg(); //Return rhs to its original state	
		}
		else { /*Equal signals: [(+a)-(+b) = a-b] or [(-a)-(-b) = -a + b]*/

			/*The strategy below is to subtract the lesser one from the larger one (in absolute value, and set the sign to the larger’s sign*/

			std::vector<uchar> *minuend, *subtrahend;
			if (this->isAbsGreater(rhs)){
				minuend = &values;
				subtrahend = &rhs.values;
				//Leave sign as is
			}
			else if (this->isAbsLesser(rhs)){
				minuend = &rhs.values;
				subtrahend = &values;
				values.resize(rhs.values.size());
				/*Get sign from rhs*/
				this->sign = rhs.sign;
			}
			else {
				this->assign(0);
				return *this;
			}
			/*Generic subtraction algorithm that works for any base. To adapt to other bases, one need to change the ‘uchar’ types to its proper types.*/
			uchar s, m, carry = 0;
			size_t i = 0;
			for (; i < subtrahend->size(); ++i){
				/*This would be generic to any base.*/
				m = minuend->at(i) - carry;
				s = subtrahend->at(i);
				carry = m < s;
				values[i] = (m + base - s) % base;
			}
			for (; i < minuend->size(); ++i){
				values[i] = (minuend->at(i) - carry + base) % base;
				carry = 0;
			}
			this->trimFrontalZeros();
		}
		return *this;
	}
	BigInt &BigInt::operator*=(BigInt &rhs)
	{
		/*Case when one of the operands is Zero*/
		if (this->isZero())
			return *this;
		else if (rhs.isZero()){
			this->assign(0);
			return *this;
		}

		bool finalSign = sign ^ rhs.sign;
		/*Case when one of the operands is One*/
		if (this->isAbsOne()){
			this->assign(rhs);
			sign = finalSign;
			return *this;
		}
		else if (rhs.isAbsOne()){
			sign = finalSign;
			return *this;
		}
		/*Note on length of a multiplication result:
		Cases where a carry is      generated during the additions: len(a*b) = len(a) + len(b)
		Cases where a carry is NOT  generated during the additions: len(a*b) = len(a) + len(b) - 1
		Therefore, maxPossibleLen(a * b) = len(a) + len(b)*/
		size_t maxLen = values.size() + rhs.values.size();
		std::vector<uchar> product(maxLen);

		size_t lowSize, bigSize;
		std::vector<uchar> *multiplier, *multiplicand;

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
		uchar carry = 0;
		for (size_t n = 0; n < maxLen; ++n){
			
			size_t numIters;
			if (maxLen - n - 1< lowSize) numIters = maxLen - n - 1;
			else					     numIters = std::min(n + 1, lowSize);

			std::uint64_t sum = 0;
			for (size_t i = 0; i < numIters; ++i){
				size_t indBelow = i + n + 1 - std::min(n+1, lowSize);
				size_t indAbove = std::min(n + 1, lowSize) - 1 - i;
				uchar be = (*multiplier)[indBelow];
				uchar ab = (*multiplicand)[indAbove];
				sum +=  be*ab ;
			}
			sum += carry;
			product[n] = sum % base;
			carry = sum / base;
		}
		values.assign(product.begin(), product.end());
		this->sign = finalSign;
		this->trimFrontalZeros();
		return *this;
	}
	BigInt &BigInt::operator/=(BigInt &rhs){
		
		if (rhs.isZero()){
			throw std::string("Error at BigInt class: division by zero detected.");
		}		
		if (this->isAbsLesser(rhs)){
			this->abs();
			this->assign(0);
			return *this;
		}
		bool finalSign = sign ^ rhs.sign;
		if (rhs.isAbsOne()){
			sign = finalSign;
			return *this;
		}

		//BigInt product = 0;
		BigInt quotient = 1;
		while ((quotient*rhs).isAbsLesser(*this)){
			++quotient;
		}

		/*Division is always ceiled, but we test because it may be an exact one (without rest)*/
		if ((quotient*rhs).isAbsGreater(*this)){
			--quotient;
		}

		this->assign(quotient);
		this->sign = finalSign;
		return *this;
	}
	std::ostream &operator<<(std::ostream &out, const BigInt &rhs){
		if (rhs.sign)
			out << '-';
		std::copy(rhs.values.rbegin(), rhs.values.rend(), std::ostream_iterator<int>(out, ""));
		return out;
	}
	std::istream &operator>>(std::istream &in, BigInt &rhs){
		std::string str;
		in >> str;
		rhs = str;
		return in;
	}

	std::int8_t BigInt::compare(const BigInt &b) const {
		if (values.size() > b.values.size())
			return +1;
		else if (b.values.size() > values.size())
			return -1;
		else{
			for (int i = values.size() - 1; i >= 0; --i){
				if (values[i] > b.values[i])
					return +1;
				else if (b.values[i] > values[i])
					return -1;
			}
			return 0; //means they are equal
		}
	}
	bool BigInt::isAbsGreater(const BigInt &b) const{
		return this->compare(b) > 0;
	}
	bool BigInt::isAbsLesser(const BigInt &b) const{
		return this->compare(b) < 0;
	}
	bool BigInt::isAbsEqual(const BigInt &b) const{
		return values == b.values;
	}



	std::string longBinaryToDecimalAsString(std::vector<bool> &longBinary){

		ex::BigInt big;
		std::uint64_t n = 0;
		for (bool bit : longBinary){
			if (bit){
				if (n > 63){
					ex::BigInt aux(1);
					for (std::uint64_t i = 0; i < n / 32; ++i){
						aux *= ex::BigInt(1ULL << 32);
					}
					aux *= ex::BigInt(1ULL << (n % 32));
					big += aux;
				}
				else{
					big += ex::BigInt(1ULL << n);
				}
			}
			++n;
		}
		return big.toString();
	}

};

#endif