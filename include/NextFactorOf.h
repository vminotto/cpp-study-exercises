#ifndef NEXT_FACTOR_OF_H
#define NEXT_FACTOR_OF_H

#include <cmath>
#include <vector>
#include <cstdint>

template<class... Args> struct count;

template<> struct count<> {
	static const int value = 0;
};

template<class T, class... Args> struct count<T, Args...> {
	static const int value = 1 + count<Args...>::value;
};

/*7.7 Design an algorithm to find the kth number such that the only prime factors are 3, 5, and 7*/
namespace ex{
	class NextFactorOf{

	public:
		class NextFactorOf(std::vector<std::uint64_t> factors){
			this->factors.assign(factors.begin(), factors.end());
			std::sort(this->factors.begin(), this->factors.end());

			/*Start with the first factor (note the AND at the excercise's title),
			that is, all factors must be present in the desired n'th number.*/
			std::uint64_t n = 1;
			for_each(factors.begin(), factors.end(), [&](std::uint64_t factor){ n *= factor; });
			values.push_back(n);
			this->updateFactorToUse();
		}

		std::uint64_t getNext(std::uint64_t n){
			if (n >= values.size()){
				for (std::uint64_t i = values.size(); i <= n; i++){
					values.push_back(values.at(i - 1) * factors.at(factorToUse));
					this->updateFactorToUse();
				}
			}
			return values.at(n);
		}

		

	private:
		std::vector<std::uint64_t> factors;
		std::vector<std::uint64_t> values;
		std::uint64_t factorToUse;

		void updateFactorToUse(){
			//-1 here because we already start with a value
			factorToUse = (values.size()-1) % factors.size();
		}

	};
};




#endif