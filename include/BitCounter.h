#ifndef BIT_COUNTER_H
#define BIT_COUNTER_H

#include <bitset>
#include <type_traits> 
#include <cstdint>

namespace ex{
	
	template <uint8_t maskId, uint8_t numBytes> struct mask{
		static_assert(maskId > 5, "This mask Id is invalid. Maximum is 5.");
		static_assert(numBytes > 0 && numBytes <= 8, "Only an ammount between 1 to 8 bytes is accepted.");
	};
	template <uint8_t numBytes> struct mask<0, numBytes>{ enum : uint64_t { value = (0x55ULL << 8 * (numBytes - 1)) | mask<0, numBytes - 1>::value }; };
	template <>					struct mask<0, 1>		{ enum : uint64_t { value = 0x55ULL }; };
	
	template <uint8_t numBytes> struct mask<1, numBytes>{ enum : uint64_t { value = (0x33ULL << 8 * (numBytes - 1)) | mask<1, numBytes - 1>::value }; };
	template <>					struct mask<1, 1>		{ enum : uint64_t { value = 0x33ULL }; };

	template <uint8_t numBytes> struct mask<2, numBytes>{ enum : uint64_t { value = (0x0FULL << 8 * (numBytes - 1)) | mask<2, numBytes - 1>::value }; };
	template <>					struct mask<2, 1>		{ enum : uint64_t { value = 0x0FULL }; };

	template <uint8_t numBytes> struct mask<3, numBytes>{
		static_assert(numBytes && numBytes % 2 == 0, "The 0x00FFU mask can only be created for values with types with a an ammount of bytes multiple of 2.");
		enum : uint64_t { value = (0x00FFULL << 8 * (numBytes - 2)) | mask<3, numBytes - 2>::value };
	};
	template <>					struct mask<3, 2>		{ enum : uint64_t { value = 0x00FFULL }; };

	template <uint8_t numBytes> struct mask<4, numBytes>{
		static_assert(numBytes && numBytes % 4 == 0, "The 0x0000FFFFU mask can only be created for values with types with a an ammount of bytes multiple of 4.");
		enum : uint64_t { value = (0x0000FFFFULL << 8 * (numBytes - 4)) | mask<4, numBytes - 4>::value };
	};
	template <>					struct mask<4, 4>		{ enum : uint64_t { value = 0x0000FFFFULL }; };

	template <uint8_t numBytes> struct mask<5, numBytes>{
		static_assert(numBytes && numBytes % 8 == 0, "The 0x00000000FFFFFFFFU mask can only be created for values with types with a an ammount of bytes multiple of 8.");
		enum : uint64_t { value = (0x00000000FFFFFFFFULL << 8 * (numBytes - 8)) | mask<5, numBytes - 8>::value };
	};
	template <>					struct mask<5, 8>		{ enum : uint64_t { value = 0x00000000FFFFFFFFULL }; };
		

	template <std::uint64_t N, std::uint64_t base = 2> struct log_floor
	{
		enum{ value = 1 + log_floor<N / base, base>::value };
	};
	template <std::uint64_t base> struct log_floor<1, base>{
		enum{ value = 0 };
	};

	class BitCounter{
	
	private:
				
		static const std::uint8_t numBitsInChar[256];

		template <class T> struct unsignedDecimal{
			/*Compile time check.*/
			static_assert(std::is_integral<T>::value, "Error at 'BitCounter': this class can only count the number of bits in decimal types (positive or negative ones).");
			using u = typename std::make_unsigned<T>::type;
		};

		template <uint8_t maskId, class T> struct sumSideways{
			T operator()(T val){

				/*Mask*/
				const T m = mask<maskId, sizeof(T)>::value;
				/*Rotation*/
				const uint8_t rot = 1 << maskId;
				/*Recursion (stacking calls until we call for maskId == 0)*/
				T r = sumSideways<maskId - 1, T>()(val);
				return (m & r) + ((r >> rot) & m);
			}
		};
		template <class T> struct sumSideways<0, T>{
			T operator()(T val){
				const T m = mask<0, sizeof(T)>::value;
				return (m & val) + ((val >> 1) & m);
			}
		};
		
	public:
		BitCounter(){}
	
		/*This is the naive approach, which will always run in O(N), where N is the number of bits in the digit. In 
		other words	it will always run in worst case, no matter how many bits are actually set.*/
		template <class T> std::uint8_t get_naive(T value){

			unsignedDecimal<T>::u unsValue = value;

			std::uint8_t numBits = 0;
			while (unsValue){
				numBits += unsValue & 1;
				unsValue >>= 1;
			}
			return numBits;
		}

		/*Returns the number of '1' bits in a given decimal value. This is an iterative approach with
		a worst case of O(N), where N is sizeof(T)*8. This is Peter Wegner's algorithm, first published
		in 1960.*/
		template <class T> std::uint8_t get_wegner(T value){

			static_assert(std::is_integral<T>::value, "Error at 'BitCounter': this class can only count the number of bits in decimal types (positive or negative ones).");

			std::uint8_t numBits = 0;
			while (value){
				/*Clearing the rightmost bit at each iteration*/
				value &= value - 1;
				++numBits;
			}
			return numBits;
		}

		/*Returns the number of '1' bits in a given decimal value. This is an hybrid iterative-lookup
		approach with a worst case of O(N), where N is sizeof(T)*/
		template <class T> std::uint8_t get_lookup(T value){

			static_assert(std::is_integral<T>::value, "Error at 'BitCounter': this class can only count the number of bits in decimal types (positive or negative ones).");

			std::uint8_t numBits = 0;
			for (int i = 0; i < sizeof(T); ++i){
				T mask = 0xFF << (i * 8);
				std::uint8_t index = (value & mask) >> (i * 8);
				numBits += numBitsInChar[index];
			}
			return numBits;
		}

		/*Returns the number of '1' bits in a given decimal value. This uses the 'std::bitset<>' class,
		which uses a built in function (called '__builtin_popcount()' in some architectures) to do counting.*/
		template <class T> std::uint8_t get_builtin(T value){

			static_assert(std::is_integral<T>::value, "Error at 'BitCounter': this class can only count the number of bits in decimal types (positive or negative ones).");

			/*The bitset::count function will be optmized by the compiler into the built-in
			function __builtin_popcount(), which is really fast.*/
			return std::bitset<sizeof(T)* 8>(value).count();
		}

		/*Sideways addition algorithm.
		Counts the number of bits by a divide and conquer technique, which uses masking 
		and addition with 'Binary Magic Numbers'.	
		Good historical facts described here https://groups.google.com/forum/?hl=en#!msg/comp.graphics.algorithms/ZKSegl2sr4c/QYTwoPSx30MJ
		Good explanation (with pictures) here http://www.opensourceforu.com/2012/06/power-programming-bitwise-tips-tricks/
		*/
		std::uint8_t get_sideways_didatic(std::uint32_t v){			
			std::uint32_t oddBits		=  v	   & 0x55555555;
			std::uint32_t eveBits		= (v >> 1) & 0x55555555;
			v = oddBits + eveBits;

			std::uint32_t rightPairs	=  v	   & 0x33333333;
			std::uint32_t leftPairs		= (v >> 2) & 0x33333333;
			v = rightPairs + leftPairs;

			std::uint32_t rightQuads	=  v	   & 0x0F0F0F0F;
			std::uint32_t leftQuads		= (v >> 4) & 0x0F0F0F0F;
			v = rightQuads + leftQuads;

			std::uint32_t rightOcts		=  v	   & 0x00FF00FF;
			std::uint32_t leftOcts		= (v >> 8) & 0x00FF00FF;
			v = rightOcts + leftOcts;

			std::uint32_t rightSixteenths =  v		  & 0x0000FFFF;
			std::uint32_t  leftSixteenths = (v >> 16) & 0x0000FFFF;
			v = rightSixteenths + leftSixteenths;
			return v;
		}
		std::uint8_t get_sideways_didatic_compact(std::uint32_t x){
			x = (x & 0x55555555) + ((x >> 1) & 0x55555555);
			x = (x & 0x33333333) + ((x >> 2) & 0x33333333);
			x = (x & 0x0F0F0F0F) + ((x >> 4) & 0x0F0F0F0F);
			x = (x & 0x00FF00FF) + ((x >> 8) & 0x00FF00FF);
			x = (x & 0x0000FFFF) + ((x >> 16) & 0x0000FFFF);
			return x;
		}

		/*This is a modification of the common sideways addition with a lot of optimizations.
		The version is compact and hard to read/understand.*/
		std::uint32_t get_sideways_super_optmized(std::uint32_t i)
		{
			i = i - ((i >> 1) & 0x55555555);
			i = (i & 0x33333333) + ((i >> 2) & 0x33333333);
			return (((i + (i >> 4)) & 0x0F0F0F0F) * 0x01010101) >> 24;
		}
		template <class T> T get_sideways_super_optmized(T val)
		{
			std::make_unsigned<T>::type v = val;
			v = v - ((v >> 1) & (T)~(T)0 / 3);                           // temp
			v = (v & (T)~(T)0 / 15 * 3) + ((v >> 2) & (T)~(T)0 / 15 * 3);      // temp
			v = (v + (v >> 4)) & (T)~(T)0 / 255 * 15;                      // temp
			return (T)(v * ((T)~(T)0 / 255)) >> (sizeof(T)-1) * CHAR_BIT; // count
		}

		

		/*Version that works for any kind of decima value. Any number of bits 
		multiple of 8, either signed or unsigned. It uses a lot of template
		metaprogramming, so the approach is very fast, with most of the computations
		performed at compile time.*/
		template <class T> std::uint8_t get_sideways(T value){
			const uint8_t maskId = log_floor<sizeof(T)* 4>::value;
			return sumSideways<maskId, unsignedDecimal<T>::u>()(value);
		}

		

	};		
	
	/*Look-up table for 8-bit numbers.*/
	const std::uint8_t BitCounter::numBitsInChar[256] = {	
		0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3,
		4, 3, 4, 4, 5, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4,
		4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
		2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4,
		5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 1, 2, 2, 3, 2, 3,
		3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
		2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5,
		6, 5, 6, 6, 7, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5,
		5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
		4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8 
	};

};

#endif