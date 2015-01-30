#ifndef CARTESIAN_LINE_H
#define CARTESIAN_LINE_H

#include <cmath>
#include <algorithm>
#include <functional> 

namespace ex{
	
	const double doubleTol = 1.0e-5;

	bool isEqual(const double &x, const double &y){
		return std::abs(x - y) <= doubleTol * std::max(std::max(1.0, std::abs(x)), std::abs(y));
	}
	
	class CartLine{
	public:
		CartLine() = default;
		CartLine(double slope, double intersect) : slope(slope), intersect(intersect){}

		double getSlope() const { return slope; }
		double getIntersect() const { return intersect; }
		void set(double slope, double intersect){
			this->slope		= slope;
			this->intersect = intersect;
		}

		bool intersects(CartLine &line){

			/*Both lines are the same, so we'll consider this an instersection*/
			if (isEqual(this->slope, line.slope) && isEqual(this->intersect, line.intersect))
				return true;
			else if (isEqual(this->slope, line.slope)) //lines are parallel
				return false;
			else //if slopes are different, lines will always meet
				return true;
		}

	private:
		double slope      = 0;
		double intersect  = 0;
		
		
	
	};

};


namespace std{

	template <> struct hash<ex::CartLine>{
		size_t operator()(const ex::CartLine &line){
			return ((hash<double>()(line.getSlope()) ^ (hash<double>()(line.getIntersect()) << 1)) >> 1);
		}
	};
};

#endif