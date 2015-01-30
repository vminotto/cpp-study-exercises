#ifndef LIST_FUNCTIONS_H
#define LIST_FUNCTIONS_H

#include "List.h" 

namespace ex{
	/*2.5 You have two numbers represented by a linked list, where each node contains a
	single digit. The digits are stored in reverse order, such that the Ts digit is at the
	head of the list. Write a function that adds the two numbers and returns the sum
	as a linked list.
	EXAMPLE
	Input: (7-> 1 -> 6) + (5 -> 9 -> 2).That is, 617 + 295.
	Output: 2 -> 1 -> 9.That is, 912.

	Suppose the digits are stored in forward order. Repeat the above problem.
	EXAMPLE
	Input: (6 -> 1 -> 7) + (2 -> 9 -> 5).That is, 617 + 295.
	Output: 9 -> 1 -> 2.That is, 912.

	This assume the list type is a numeric value that supports the + operator*/
	template <class T> List<T> *getSumAsList1(ex::List<T> *a, ex::List<T> *b, bool reverse = true){
		/*reverse = true  means the first problem
		reverse = false means the second problem*/

		if (a->isEmpty() && b->isEmpty())
			return new List<T>();
		else if (a->isEmpty())
			return new List<T>(*b);
		else if (b->isEmpty())
			return new List<T>(*a);

		/*Singly linked, size known*/

		long long first = 0, second = 0;
		bool r = reverse;

		/*Each node may only assume values between 0 and 9*/
		/*This implementation works greatly even if both links have different sizes*/

		int c = 0;
		int size = a->getSize();
		for (auto i : *a){
			long long temp = (long long)std::pow(10, (!r*(size - c - 1) + r*c));
			first += temp * i;
			c++;
		}

		c = 0;
		size = b->getSize();
		for (auto i : *b){
			long long temp = (long long)std::pow(10, (!r*(size - c - 1) + r*c));
			second += temp * i;
			c++;
		}

		long long result = first + second;

		List<T> *list = new List<T>();
		c = 10;
		while (result != 0){
			int digit = result % c; /*unit digit*/
			result /= c;
			if (reverse)
				list->push_back(digit);
			else
				list->push_front(digit);
		}
		return list;
	}

}
#endif