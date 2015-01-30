#ifndef MATH_FUNCTIONS_H
#define MATH_FUNCTIONS_H

#include <cmath>
#include "CartesianLine.h"
#include <vector>
#include <numeric>
#include <functional> 
#include <unordered_set>
#include <queue>

namespace ex{

	using namespace std;

	/*3 5 7 9 15 21 25 */

	/*7.7 Design an algorithm to find the kth number such that the only prime factors are 3,
	5, and 7.*/

	int getKthNum357(int k){
	
		if (k == 0)
			return 0;

		std::queue<int> que3, que5, que7;

		que3.push(1);
		int v3, v5, v7, minVal;

		for (int i = 1; i <= k; i++){
			/*0 */
			v3 = que3.empty() ? std::numeric_limits<int>::max() : que3.front();
			v5 = que5.empty() ? std::numeric_limits<int>::max() : que5.front();
			v7 = que7.empty() ? std::numeric_limits<int>::max() : que7.front();

			minVal = std::min(std::min(v3, v5), v7);
			if (minVal == v3){
				que3.pop();
				que3.push(3 * minVal);
				que5.push(5 * minVal);
				que7.push(7 * minVal);
			}
			else if (minVal == v5){
				que5.pop();
				que5.push(5 * minVal);
				que7.push(7 * minVal);

			}
			else if (minVal == v7){
				que7.pop();
				que7.push(7 * minVal);
			}
		}
		return minVal;
	}


	int getKthNum(int k)
	{
		if (k == 0)
			return 0;
		queue<int> Q3;
		queue<int> Q5;
		queue<int> Q7;
		Q3.push(1);
		int val = INT_MAX;
		for (int i = 0; i <= k; ++i)
		{
			int v3 = Q3.size() == 0 ? INT_MAX : Q3.front();
			int v5 = Q5.size() == 0 ? INT_MAX : Q5.front();
			int v7 = Q7.size() == 0 ? INT_MAX : Q7.front();
			val = min(v3, min(v5, v7));
			if (val == v3)
			{
				Q3.pop();
				Q3.push(val * 3);
				Q5.push(val * 5);
			}
			else if (val == v5)
			{
				Q5.pop();
				Q5.push(val * 5);
			}
			else if (val == v7)
			{
				Q7.pop();
			}
			Q7.push(val * 7);
		}
		return val;
	}

	

	struct double2{
		double2() = default;
		double2(double x, double y) : x(x), y(y){}
		double x = 0;
		double y = 0;
	};

	double2 operator+(double2 lhs, double2 rhs){
		double2 r;
		r.x = lhs.x + rhs.x;
		r.y = lhs.y + rhs.y;
		return r;
	}
	double2 operator/(double2 lhs, double rhs){
		lhs.x /= rhs;
		lhs.y /= rhs;
		return lhs;
	}

	template<class T> std::vector<size_t> sortAndGetTrack(std::vector<T> &vec){
		std::vector<size_t> sortTrack(vec.size());

		std::iota(sortTrack.begin(), sortTrack.end(), 0);

		std::sort(sortTrack.rbegin(), sortTrack.rend(), [&](T a, T b){
			return vec[a] < vec[b];
		});
		return sortTrack;
	}

	template <class It1, class It2> void rearrange(It1 valuesBegin, It1 valuesEnd, It2 orderBegin, It2 orderEnd){
	
		if (std::distance(valuesBegin, valuesEnd) != std::distance(orderBegin, orderEnd))
			return;

		std::vector<It1::value_type> copy(valuesBegin, valuesEnd);


		//std::vector<int>::iterator::value_type
		auto value = valuesBegin;
		for (auto it = orderBegin; it != orderEnd; ++it){
			*value = copy.at(*it);
			++value;
		}
		
	}

	/*7.6 Given a two-dimensional graph with points on it, all unique lines formed by
	all unique points, they are ordered in terms of ocurrence (index 0 is the line that ocurred most).*/
	std::vector<CartLine> getLineWithMostIntersections(std::vector<double2> &pts){
		
		std::vector<CartLine> lines;
		
		/*This for computes all unique lines formed by the points in the 'pts' vector */
		for (size_t i = 0; i < pts.size(); i++){
			for (size_t j = i + 1; j < pts.size(); j++){ //iterating through all unique pairs of points
				double2 pt1 = pts.at(i);
				double2 pt2 = pts.at(j);
				if (!isEqual(pt1.x, pt2.x)){ //in case points form a vertical line, we skip it
					double slope = (pt1.y - pt2.y) / (pt1.x - pt2.x);
					double inter = pt1.y - pt1.x*slope;
					lines.push_back(CartLine(slope, inter));
				}
			}
		}

		/*Lambda function used to check if two lines are identical*/
		std::function<bool(const ex::CartLine&, const ex::CartLine&)> areEqual =
			[](const ex::CartLine &a, const ex::CartLine &b){
			return ex::isEqual(a.getIntersect(), b.getIntersect()) & ex::isEqual(a.getSlope(), b.getSlope());
		};


		/*Store all unique lines, that is, disacrd the duplicate ones in the 'lines' vector.*/
		std::vector<CartLine> uniqueLines(lines.begin(), lines.end());
		std::unordered_set<ex::CartLine, std::hash<ex::CartLine>, std::function<bool(const ex::CartLine&, const ex::CartLine&)>> uniqueLinesSet
			(lines.begin(), lines.end(), 0, std::hash<ex::CartLine>(), areEqual);
		uniqueLines.assign(uniqueLinesSet.begin(), uniqueLinesSet.end());
		//uniqueLines.erase(std::unique(uniqueLines.begin(), uniqueLines.end(), areEqual), uniqueLines.end());
		
		/*Finds the line which ocurs most*/
		std::vector<int> counts;
		for (size_t i = 0; i < uniqueLines.size(); i++){
			CartLine &example = uniqueLines.at(i);
			auto isEqualTo = [&](CartLine &a){
				return ex::isEqual(a.getIntersect(), example.getIntersect()) & ex::isEqual(a.getSlope(), example.getSlope());
			};
			int numLines = std::count_if(lines.begin(), lines.end(), isEqualTo);
			counts.push_back(numLines);
		}

		std::vector<size_t> sortTrack = ex::sortAndGetTrack(counts);

		ex::rearrange(uniqueLines.begin(), uniqueLines.end(), sortTrack.begin(), sortTrack.end());

		return uniqueLines;
	}


	/*7.5 Given two squares on a two-dimensional plane, find a line that would cut these two
	squares in half. Assume that the top and the bottom sides of the square run parallel
	to the x-axis.*/
	ex::CartLine getLineThatCutsInHalf(double2 tl1, double2 br1, double2 tl2, double2 br2){
		double2 c1 = (tl1 + br1) / 2.0; //center of square 1
		double2 c2 = (tl2 + br2) / 2.0; //center of square 2

		double slope = (c1.y - c2.y) / (c1.x - c2.x);
		double inter = c1.y - c1.x*slope;

		return CartLine(slope, inter);
	}

	/*7.4 Write methods to implement the multiply, subtract, and divide operations for integers.
	Use only the add operator.
	Order to implement.: minus, multiply (which depends on minus), division*/
	int negate(int x){
		
		int digit = x > 0 ? 0xFFFFFFFF : 0x1;
		int negatedX = 0;

		while (x != 0){ 
			x += digit;
			negatedX += digit;
		}
		return negatedX;
	}

	/*This function uses the 'negate' function, which uses an 'if', a 'while' and a '-1 (0xFFFFFFFF)'.
	According to the excercise's title, this should not be allowed... Therefore, the below version is 
	a lot more clean and direct.*/
	int subtract(int lhs, int rhs){
		return lhs + negate(rhs);
	}

	int minus(int lhs, int rhs){
		return lhs + ~rhs + 1;
	}
	int abs(int x){
		return x < 0 ? negate(x) : x;
	}
	int multiply(int lhs, int rhs){
		
		if (lhs == 0 || rhs == 0)
			return 0;

		int r = abs(rhs);
		int l = abs(lhs);
		int result = 0;
		for (int i = 0; i < l; i++){
			result += r;
		}

		if (lhs < 0 && rhs > 0)
			result = negate(result);
		else if (lhs > 0 && rhs < 0)
			result = negate(result);
		
		return result;
	}

	int divide(int lhs, int rhs){

		if (lhs == 0 && rhs == 0)
			return std::numeric_limits<int>::quiet_NaN();
		else if (lhs == 0)
			return 0;
		else if (lhs > 0 && rhs == 0)
			return +std::numeric_limits<int>::infinity();
		else if (lhs < 0 && rhs == 0)
			return -std::numeric_limits<int>::infinity();
		else if (abs(lhs) < abs(rhs))
			return 0;

		int r = abs(rhs);
		int l = abs(lhs);
		int n = 0;
		int result = 0;
		while (n < (l-1))
		{
			n += r;
			result++;
		}
	
		if (lhs < 0 && rhs > 0)
			result = negate(result);
		else if (lhs > 0 && rhs < 0)
			result = negate(result);

		return result;
	}




};









#endif