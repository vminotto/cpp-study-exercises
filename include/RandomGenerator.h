#ifndef RANDOM_GENERATOR_H
#define RANDOM_GENERATOR_H

#include <random>
#include <iostream>
#include <string>
#include <array>

namespace ex{

	class DoubleGenerator{

		/*Constructs a Pseudo-random number generator, which will output
		a number of type 'T' in the range of [min, max] if 'includeMax == true'
		or [min, max) if 'includeMax == false'*/
	public:
		DoubleGenerator(double min, double max, bool includeMax = true){
			randomEngine.seed(randomDevice());
			set(min, max, includeMax);
		}

		/*Returns a random number*/
		double operator()(){ return dist(randomEngine); }
		
		void set(double min, double max, bool includeMax = true){
			if (includeMax)
				dist.param(std::uniform_real_distribution<double>::param_type(min, std::nextafter(max, std::numeric_limits<double>::max())));
			else
				dist.param(std::uniform_real_distribution<double>::param_type(min, max));
		}

	private:
		std::random_device randomDevice;
		std::mt19937 randomEngine;
		std::uniform_real_distribution<double> dist;
	};
	
	class IntegerGenerator{

		/*Constructs a Pseudo-random number generator, which will output
		a number of type 'T' in the range of [min, max].*/
	public:
		IntegerGenerator(int min, int max){
			randomEngine.seed(randomDevice());
			set(min, max);
		}

		/*Returns a random number*/
		int operator()(){ return dist(randomEngine); }
	
		void printSequence(int numRandomNumbers, char separator = ',', std::ostream &oStream = std::cout){
			for (int i = 0; i < numRandomNumbers-1; i++){
				oStream << operator()() << separator;
			}
			oStream << operator()();
		}

		void set(int min, int max){
			dist.param(std::uniform_int_distribution<int>::param_type(min, max));
		}

	private:
		std::random_device randomDevice;
		std::mt19937 randomEngine;
		std::uniform_int_distribution<int> dist;
	};

	class UIntegerGenerator{

		/*Constructs a Pseudo-random number generator, which will output
		a number of type 'T' in the range of [min, max] if 'includeMax == true'
		or [min, max) if 'includeMax == false'*/
	public:
		UIntegerGenerator(unsigned int min, unsigned int max) : UIntegerGenerator() {
			set(min, max);
		}
		UIntegerGenerator(){
			randomEngine.seed(randomDevice());
		}

		/*Returns a random number*/
		unsigned int operator()(){ return dist(randomEngine); }

		void printSequence(unsigned int numRandomNumbers, char separator = ',', std::ostream &oStream = std::cout){
			for (unsigned int i = 0; i < numRandomNumbers - 1; i++){
				oStream << operator()() << separator;
			}
			oStream << operator()();
		}

		void set(unsigned int min, unsigned int max){
			dist.param(std::uniform_int_distribution<unsigned int>::param_type(min, max));
		}

	private:
		std::random_device randomDevice;
		std::mt19937 randomEngine;
		std::uniform_int_distribution<unsigned int> dist;
	};

	class ByteGenerator{
		
		/*Constructs a Pseudo-random number generator, which will output
		a number of type 'T' in the range of [min, max] if 'includeMax == true'
		or [min, max) if 'includeMax == false'*/
	public:
		ByteGenerator(unsigned char min, unsigned char max){
			randomEngine.seed(randomDevice());
			set(min, max);
		}

		/*Returns a random number*/
		unsigned char operator()(){ return static_cast<unsigned char>(dist(randomEngine)); }

		void printSequence(unsigned char numRandomNumbers, char separator = ',', std::ostream &oStream = std::cout){
			for (unsigned char i = 0; i < numRandomNumbers - 1; i++){
				oStream << operator()() << separator;
			}
			oStream << operator()();
		}

		void set(unsigned char min, unsigned char max){
			dist.param(std::uniform_int_distribution<int>::param_type(min, max));
		}

	private:
		std::random_device randomDevice;
		std::mt19937 randomEngine;
		std::uniform_int_distribution<int> dist;
	};

	class StringGenerator{
	
	public:
		StringGenerator() : StringGenerator("0123456789abcdefghijklmnopqrstuvxywzABCDEFGHIJKLMNOPQRSTUVXYWZ") {}
		StringGenerator(std::string possibleCharacters) : possibleCharacters(possibleCharacters) {
			rd.set(0, possibleCharacters.size() - 1);
		}
		
		/*Constructs a random string with length 'lenght'*/
		std::string operator()(unsigned int length){
			std::string str;
			for (unsigned int i = 0; i < length; ++i){
				str += possibleCharacters[rd()];
			}
			return str;
		}

		/*Constructs a random string with a random length between '0' and 'possibleCharacters.size()' */
		std::string operator()(){
			return operator()(rd()+1); //we sum +1 so no blank strings are generated
		}

	private:	
	
		std::string possibleCharacters;
		UIntegerGenerator rd;

	};

	//auto rand = bind(uniform_int_distribution<>(0, 7), mt19937(get_seed()));

	/*std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<double> dist(1, 10);*/

	
	/*for (int i = 0; i<16; ++i)
		std::cout << dist(mt) << "\n";*/




};

#endif
