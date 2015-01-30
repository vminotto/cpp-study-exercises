#include <bitset>
#include <type_traits> 
#include <cstdint>

namespace ex{
	
	class BitCounter{
			
		BitCounter(){}

		template <class T> struct integral{
			/*Compile time check.*/
			static_assert(std::is_integral<T>::value, "Error at 'BitCounter': this class can only count the number of bits in decimal types (positive or negative ones).");		
			using unsT = std::make_unsigned<T>::type;
		};

		/*Returns the number of '1' bits in a given decimal value. This is an iterative approach with 
		a worst case of O(N), where N is sizeof(T)*8 */
		template <class T> std::uint8_t getBitCountIter(T value){

			integral<T>::unsT unsignedValue = static_cast<unsT>(value);

			std::uint8_t bitCount = 0;
			while (value){
				/*Clearing the rightmost bit at each iteration*/
				value &= value - 1;
				++bitCount;
			}
			return bitCount;
		}

		template <class T> std::uint8_t getBitCountLookUp(T value){
					
			integral<T>::unsT unsignedValue = static_cast<unsT>(value);

			std::uint8_t numBits = 0;
			std::uint8_t numBytes = sizeof(T);
			std::uint8_t totalBits = numBytes * 8;
			for (int i = 0; i < numBytes; ++i){
				integral<T>::unsT mask = 0xFF << (i*8)
				numBits += numBitsInChar[];
			}
		}

	private:

		static const std::uint8_t numBitsInChar[256];
		
	};

	
	
	
	
	
	char BitCounter::numBitsInChar = {	0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3,
										4, 3, 4, 4, 5, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4,
										4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
										2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4,
										5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 1, 2, 2, 3, 2, 3,
										3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
										2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5,
										6, 5, 6, 6, 7, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5,
										5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
										4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8 };







};

