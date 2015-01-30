#include "HardQuestions.h"
using namespace std;

int main(){

	std::basic_string<std::uint64_t> tt1 = { 'a', 'b', 'c', 'd' };
	
	std::uint64_t ovf = std::numeric_limits<std::uint64_t>::max();

	ovf += 1;

	cout << tt1.c_str() << endl;


	//uint32_t asdff = std::numeric_limits<uint32_t>::max();

	/*Testing 18.1*/
	int a = 725;
	int b = 114;
	const int size = 16;
	cout << "Summing " << a << " and " << b << endl;
	cout << "Summing (binary view): " << endl;
	cout << std::bitset<size>(a).to_string() << endl;
	cout << std::bitset<size>(b).to_string() << endl;
	int sum = ex::add(a, b);

	/*Testing 18.2*/
	cout << endl << "Testing 18.2"<<endl;
	std::vector<int> in = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
	std::copy(in.begin(), in.end(), std::ostream_iterator<int>(std::cout, " "));
	cout << endl;
	std::vector<int> out1(in), out2(in), out3(in), out4(in);
	cout << "Shuffle 1: "; ex::shuffle(out1); std::copy(out1.begin(), out1.end(), std::ostream_iterator<int>(std::cout, " ")); cout << endl;
	cout << "Shuffle 2: "; ex::shuffle(out2); std::copy(out2.begin(), out2.end(), std::ostream_iterator<int>(std::cout, " ")); cout << endl;
	cout << "Shuffle 3: "; ex::shuffle(out3); std::copy(out3.begin(), out3.end(), std::ostream_iterator<int>(std::cout, " ")); cout << endl;
	cout << "Shuffle 4: "; ex::shuffle(out4); std::copy(out4.begin(), out4.end(), std::ostream_iterator<int>(std::cout, " ")); cout << endl;
	
	/*Testing 18.3*/
	std::vector<int> out5 = ex::getShufledSection(in, 10);
	cout << "Shuffled part1: "; std::copy(out5.begin(), out5.end(), std::ostream_iterator<int>(std::cout, " ")); cout << endl;

	/*Testing 18.4*/
	int n = 12355;
	int num2s = ex::get2Count(n);

	
	/*Testing 18.5*/
	std::unordered_map < std::string, std::list<std::int64_t>> map;
	std::ifstream file("book.txt");
	ex::getFileAsMap(file, map);

	std::int64_t dist1 = ex::getShortestDistanceBetweenWord("energy", "single", map);
	std::int64_t dist3 = ex::getShortestDistanceBetweenWord("transformed", "transformation", map);
	file.clear();
	file.seekg(0, file.beg);
	std::int64_t dist2 = ex::getShortestDistanceBetweenWord("energy", "single", file);

	/*Testing 18.6*/
	std::vector<int> numbers = { 0, 63, 196, 900, 382, 373, 284, 672, 538, 988, 700, 279, 925, 182, 481, 759, 26, 243, 358, 510, 670,
		955, 941, 419, 981, 108, 70, 617, 929, 523, 800, 263, 335, 286, 368, 196, 907, 903, 504, 197, 580,
		63, 521, 750, 785, 464, 499, 905, 537, 241, 335, 769, 438, 370, 265, 430, 290, 594, 318, 795, 442,
		70, 748, 752, 184, 396, 439, 869, 435, 161, 11, 72, 153, 870, 719, 379, 132, 538, 369, 668, 600, 14,
		793, 91, 799, 44, 333, 104, 37, 805, 747, 412, 392, 669, 498, 576, 656, 485, 918, 64 };

	size_t N = 15;
	auto smallest = ex::findNSmallestAmongK(numbers, N);
	
	std::cout << std::endl << "The smallest " << N << " values in the size " << numbers.size() << " vector is :" << std::endl;
	std::copy(smallest.begin(), smallest.end(), std::ostream_iterator<int>(std::cout, " "));

	return 0;
}