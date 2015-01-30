#ifndef ARRAY_AND_STRINGS_h
#define ARRAY_AND_STRINGS_h

#include <unordered_map>
#include <map>
#include <string>
#include <iostream>
#include <cstdio>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <sstream>

namespace ex{
	
	/*1.1 Implement an algorithm to determine if a string has all unique characters. What if you cannot use additional data structures?*/
	bool hasAllUniqueCharacters(const std::string &str);
	/*This version is optmized for strings assumed to contain only ASCII characters.*/
	bool hasAllUniqueCharactersASCII(const char *str);
	/*This version is optmized for strings assumed to contain only ASCII characters from 'a' to 'z'.*/
	bool hasAllUniqueCharactersASCII_a_to_z(const char *str);
	/*This version is optmized for strings assumed to contain Unicode characters.*/
	bool hasAllUniqueCharactersUnicode(const char *str);
	/*Identical to strlen(...)*/
	int getStringSize(const char *str);

	/*1.2 Implement a function void reverse(char* str) in C or C++ which reverses a null terminated string.*/
	void reverseString(char *str);
	void swap(char &a, char &b);

	/*1.3 Given two strings, write a method to decide if one is a permutation of the other.
	This method is optmized for Unicode strings, because it uses std::map to make the count of
	characters. If it was ASCII, we could simply use a char[256] array to do the count.
	Another approach would be to sort both input strings, but it would require the cost 
	for sorting as well as ruining the input strings.	*/
	bool isPermutation(const std::string &stra, const std::string &strB);

	/*1.4 Write a method to replace all spaces in a string with'%20'. You may assume that the string has 
	sufficient space at the end of the string to hold the additional characters, and that you are given 
	the "true" length of the string. (Note: if implementing in Java, please use a character array so that 
	you can perform this operation in place.)
		EXAMPLE
		 Input: "Mr John Smith    "
		Output: "Mr%20John%20Smith"*/
	void replaceCharsInString(char *str, char oldChar, char *newStr);


	/*1.5 Implement a method to perform basic string compression using the counts of repeated characters. 
	For example, the string aabcccccaaa would become a2blc5a3. If the "compressed" string would not become 
	smaller than the original string, your method should return the original string.*/
	std::string getCompressedString(const std::string &str);
	int getCompressionFactor(const std::string &str); 


	/*1.6 Given an image represented by an NxN matrix, where each pixel in the image is 4 bytes, write a 
	method to rotate the image by 90 degrees. Can you do this in place?*/
	void rotateSquareMatrix(float **&mat, int N);
	void printMatrix(float **&mat, int N);

	void rotate_bool(float **matrix, int n);

	/*1.7 Write an algorithm such that if an element in an MxN matrix is 0, its entire row and column are set to 0.*/
	/*Wrong implementation*/
	void zeroElements(int **matrix, int rowFirst, int rowLast, int colFirst, int colLast); /*this one does not work*/
	void zeroElements(int **matrix, int nRows, int nCols);
	
	/*1.8 Assume you have a method isSubstring which checks if one word is a  substring of another. Given two strings, 
	s1 and s2, write code to check if s2 is a rotation of si using only one call to isSubstring (e.g.,"waterbottle"is a 
	rotation of "erbottlewat").*/
	bool isRotation(const std::string &base, std::string &rotation);

	template <class T> void fillMatrixForTest(T** mat, int nRows, int nCols){
		int count = 0;
		for (int j = 0; j < nRows; j++){
			for (int i = 0; i < nCols; i++){
				mat[j][i] = (T)count++;
			}
		}
	}
	template <class T> T      **create2DMatrix(int firstDim, int secondDim){
		T **temp = new T*[firstDim];

		for (int i = 0; i < firstDim; i++){
			temp[i] = new T[secondDim];
		}

		return temp;
	}
	template <class T> T     ***create3DMatrix(int firstDim, int secondDim, int thirdDim){

		T ***temp = new T**[firstDim];

		for (int i = 0; i < firstDim; i++){
			temp[i] = new T*[secondDim];
			for (int j = 0; j < secondDim; j++){
				temp[i][j] = new T[thirdDim];
			}
		}

		return temp;
	}
	template <class T> T    ****create4DMatrix(int firstDim, int secondDim, int thirdDim, int fourthDim){

		T ****temp = new T***[firstDim];

		for (int i = 0; i < firstDim; i++){
			temp[i] = new T**[secondDim];
			for (int j = 0; j < secondDim; j++){
				temp[i][j] = new T*[thirdDim];
				for (int k = 0; k < thirdDim; k++){
					temp[i][j][k] = new T[fourthDim];
				}
			}
		}

		return temp;
	}
	template <class T> T   *****create5DMatrix(int firstDim, int secondDim, int thirdDim, int fourthDim, int fifthDim){
		T *****temp = new T****[firstDim];

		for (int i = 0; i < firstDim; i++){
			temp[i] = new T***[secondDim];
			for (int j = 0; j < secondDim; j++){
				temp[i][j] = new T**[thirdDim];
				for (int k = 0; k < thirdDim; k++){
					temp[i][j][k] = new T*[fourthDim];
					for (int l = 0; l < fourthDim; l++){
						temp[i][j][k][l] = new T[fifthDim];
					}
				}
			}
		}

		return temp;
	}
	template <class T> T    **createValued2DMatrix(int firstDim, int secondDim, T value){
		T **temp = new T*[firstDim];

		for (int i = 0; i < firstDim; i++){
			temp[i] = new T[secondDim];
			for (int j = 0; j < secondDim; j++){
				temp[i][j] = value;
			}
		}

		return temp;
	}

	template <class T> void printMatrixToOStream(std::ostream &outStream, T **in, int firstDim, int secondDim, char cDel = ' ', char rDel = '\n'){
		for (int i = 0; i < firstDim; i++){
			for (int j = 0; j < secondDim; j++){
				outStream << in[i][j];
				outStream << cDel;
			}
			outStream << rDel;
		}
	}
}

#endif