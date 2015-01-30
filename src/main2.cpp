#include "List.h"
#include <iostream>
#include <functional>
#include <string>
#include <vector>

using namespace std;

#include <list>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <map>
#include "ListFunctions.h"


int main(){

	/*Testing 2.1 (duplicate removal)*/
	ex::List<int> list1 = { 1, 2, 3, 4, 6, 4, 2, 6, 7, 9, 0, 7, 5, 6, 7, 8, 9, 6, 3, 1, 3, 2, 4, 6 };
	std::cout << "List with duplicates:" << endl;
	list1.printElements();
	list1.removeDuplicatesNoTempBuffer();
	std::cout << "Same list, with duplicates removed:" << endl;
	list1.printElements();
	std::cout << std::endl;

	/*Testing 2.2 (kth to last element find)*/
	ex::List<int> list2 = {0, 1, 2, 3, 4, 5, 6 };
	int k = 5;
	std::cout << "Base list used:  ";
	list2.printElements();
	std::cout << k << "th to last element in the above list is (0-based indexing): " << list2.findKthToLastSizeUnknwon(k) << endl;
	std::cout << std::endl;

	/*Testing 2.3 (single node removal)*/
	ex::List<char> list3 = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};
	std::cout << "Base list used:  ";
	list3.printElements();
	list3.remove(4);
	std::cout << "List with element removed:  ";
	list3.printElements();
	std::cout << std::endl;

	/*Testing 2.4 (list partition around a value)*/
	ex::List<int> list4 = { 12,-4,9,9,9,1,-2,3,5,-6,87,9,8,6,4,23,2,2,1,3,5,-6,7,89,0,56,2,12,457,609,23,-127,9,4,2,3,6,5,21,23,6 };
	std::cout << "Base list used:" << endl;
	list4.printElements();
	int middleValue = 7;
	list4.partitionAroundValue(middleValue);
	std::cout << "Same list, partitioned around "<<middleValue<<":" << endl;
	list4.printElements();
	std::cout << std::endl;

	/*Testing 2.5 (sum lists as digitis)*/
	ex::List<int> list5a = { 7, 8, 9 };
	ex::List<int> list5b = { 3, 2, 1, 1 };
	bool reverse = true;
	std::cout << "Sum of lists below" << (reverse ? " (in reverse order): " : ":") << std::endl;
	list5a.printElements(); list5b.printElements();
	ex::List<int> *list5c = ex::getSumAsList1<int>(&list5a, &list5b, reverse);
	list5c->printElements();
	reverse = false;
	std::cout << "Sum of lists" << (reverse ? " (in reverse order): " : ":") << std::endl;
	ex::List<int> *list5d = ex::getSumAsList1<int>(&list5a, &list5b, reverse);
	list5d->printElements();
	cout << endl;

	/*Testing 2.6 (find node wich starts loop in list)*/
	ex::List<int> list6 = { 1,2,3,4,5,6,5,4,3,2,1 };
	list6.printElements();
	list6.causeCircularNodePurposely();
	std::cout << "Value of the node which caused a circular dependency in the list :" << list6.getCircularNodeNoMem()->element << endl;
	cout << endl;

	/*Testing 2.7 (check if list is palindrome)*/
	ex::List<int> list7 = { 0, 1, 2, 3, 1, 1, 1, 2, 2, 1, 1, 1, 3, 2, 1, 0 };
	cout << "Is the following list a palindrome?" << endl;
	list7.printElements();
	cout << "Plaindrome status: " << std::boolalpha << list7.isPalindrome() << endl;
	cout << endl;

	cin.get();
	return 0;
}