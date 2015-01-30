
#include <iostream>
#include "MathFunctions.h"
#include "RandomGenerator.h"
#include "NextFactorOf.h"

using uint = unsigned int;
using uchar = unsigned char;
using namespace std;

typedef unsigned char sdfsdf;

#include <bitset>

int main(){

	std::list<int> tt2 = { 1, 2, 3, 4, 5, 6, 7, 8 };
	std::list<int> tt1 = { 1, 2, 3, 4, 5, 6, 7, 8 };

	using double2 = ex::double2;
	
	int asdf = std::numeric_limits<int>::max()*656;

	/*Testing 7.7*/
	ex::NextFactorOf next({ 3, 5, 7 });
	for (int i = 0; i < 15; i++){
		std::cout << ex::getKthNum357(i) << endl;
	}

	/*Testing 7.6*/
	std::vector<double2> pts;

	pts.push_back(double2(1, 1));
	pts.push_back(double2(2, 2));
	pts.push_back(double2(3, 3));
	pts.push_back(double2(4, 4));

	pts.push_back(double2(2, 1));
	pts.push_back(double2(3, 2));
	pts.push_back(double2(4, 3));
	pts.push_back(double2(5, 4));
	
	ex::IntegerGenerator rd(-10, 10);
	for (int i = 0; i < 4; i++){
		pts.push_back(double2(rd(), rd()));
	}
	std::vector<ex::CartLine> lines = ex::getLineWithMostIntersections(pts);

	/*Testing 7.5*/
	double2 tl1(1, 3);
	double2 br1(3, 1);
	double2 tl2(4, 4);
	double2 br2(6, 2);
	ex::CartLine line1 = ex::getLineThatCutsInHalf(tl1, br1, tl2, br2);
	
	double2 tl3(-1, -3);
	double2 br3(-3, -1);
	double2 tl4(4, 6);
	double2 br4(6, 4);
	ex::CartLine line2 = ex::getLineThatCutsInHalf(tl3, br3, tl4, br4);



	/*Testing 7.4*/
	int a = 10, b = 20;
	cout << a << " - " << b << " = " << ex::minus(a, b)<<endl;
	a = 20, b = 10;
	cout << a << " - " << b << " = " << ex::minus(a, b) << endl;	
	a = 100, b = 2;
	cout << a << " * " << b << " = " << ex::multiply(a, b) << endl;
	a = -5, b = 47;	
	cout << a << " * " << b << " = " << ex::multiply(a, b) << endl;	
	a = -220, b = 3;	
	cout << a << " / " << b << " = " << ex::divide(a, b) << endl;
	a = 333, b = -20;
	cout << a << " / " << b << " = " << ex::divide(a, b)<<endl;

	cout << std::bitset<8>(-1).to_string();

	std::cin.get();
	return 0;
}