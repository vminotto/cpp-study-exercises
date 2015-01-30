#include "ArrayAndStrings.h"

using namespace std;

/*Stackoverflow on hash_map vs unordered_map
http://stackoverflow.com/questions/1646266/difference-between-hash-map-and-unordered-map
Basically, unordered_map is the official portable version, while hash_map is an older version kept just for compatibility purposes, which was not official.*/

/*
Stackoverflow on std::map vs std::set
http://stackoverflow.com/questions/16286714/advantages-of-stdset-vs-vectors-or-maps
*/
/*
Stackoverflow on std::array vs std::vector
http://stackoverflow.com/questions/4424579/stdvector-versus-stdarray-in-c
*/


int main(){


	/*Testing 1.8*/
	std::string base = "ABCDEFGH";
	std::string rotation = "GHABCDEF";
	std::cout << "Is rotation? " << ex::isRotation(base, rotation);

	/*Testing 1.7*/
	int nRows = 7;
	int nCols = 9;
	int **rectMat = ex::createValued2DMatrix<int>(nRows, nCols, 5);
	rectMat[2][3] = rectMat[4][0] = rectMat[6][8] = 0;
	ex::printMatrixToOStream(std::cout, rectMat, nRows, nCols);
	ex::zeroElements(rectMat, nRows, nCols);
	cout << endl << endl;
	ex::printMatrixToOStream(std::cout, rectMat, nRows, nCols);

	/*Testing 1.6*/
	int N = 5;
	float **mat = ex::create2DMatrix<float>(N, N);	
	ex::fillMatrixForTest(mat, N, N);
	ex::printMatrixToOStream(std::cout, mat, N, N);
	ex::rotateSquareMatrix(mat, N);
	cout << endl << endl;
	ex::printMatrixToOStream(std::cout, mat, N, N);


	/*Testing 1.5*/
	cout << endl << "Testing 1.5" << endl;
	std::string str1 = "taaa)))))))ccccct";
	cout << "Original  : " << str1 << endl;
	cout << "Compressed: " << ex::getCompressedString(str1) << endl;
	cout << "Compression Factor: " << ex::getCompressionFactor(str1) << endl;

	/*Testing 1.4*/
	cout << endl << "Testing 1.4" << endl;
	char c[128] = "abcd ef ghij ";
	cout << c << endl;
	ex::replaceCharsInString(c, ' ', "%20");
	cout<<c<<endl;


	/*Test 1.3*/
	cout << endl << "Testing 1.3" << endl;
	std::string a = "43241abFE5FcSSd1212eAAAAAfg";
	std::string b = "AAAAAabc2121FEFgfeSS145234d";
	cout << ex::isPermutation(a, b) << endl;


	/*Test 1.2*/
	cout << endl << "Testing 1.2" << endl;
	char cstr[8] = "abcdefg";
	cout << cstr << endl;
	ex::reverseString(cstr);
	cout << cstr << endl;

	/*Test 1.1*/
	cout << endl<< "Testing 1.1" << endl;
	string str("abcdefgha");
	cout << "Original string: " << str << endl;
	cout << "hasAllUniqueCharacters(str): " << ex::hasAllUniqueCharacters(str) << endl;
	cout << "hasAllUniqueCharactersASCII(str): " << ex::hasAllUniqueCharactersASCII(str.c_str()) << endl;
	cout << "hasAllUniqueCharactersUnicode(str): " << ex::hasAllUniqueCharactersUnicode(str.c_str()) << endl;
	cout << "hasAllUniqueCharactersASCII_a_to_z(str): " << ex::hasAllUniqueCharactersASCII_a_to_z(str.c_str()) << endl;

	
	std::unordered_map<std::string, std::string> testUnorderedMap, testUnorderedMap2;

	testUnorderedMap.insert(std::make_pair("a", "1"));
	testUnorderedMap.insert(std::make_pair("b", "2"));
	testUnorderedMap.insert(std::make_pair("c", "3"));
	testUnorderedMap.insert(std::make_pair("d", "4"));
	testUnorderedMap.insert(std::make_pair("e", "5"));

	testUnorderedMap.emplace("f", "6");
	testUnorderedMap.emplace("g", "7");
	testUnorderedMap.emplace("h", "8");
	testUnorderedMap.emplace("i", "9");
	testUnorderedMap.emplace("j", "10");
	testUnorderedMap.emplace("k", "11");
	testUnorderedMap.emplace("l", "12");
	testUnorderedMap.emplace("m", "13");
	testUnorderedMap.emplace("n", "14");

	std::map<std::string, std::string> testMap(testUnorderedMap.begin(), testUnorderedMap.end());

	cout << "std::unordered_map:" << endl;
	for (auto &val : testUnorderedMap){
		std::cout << "Key: " << val.first << "  Val: " << val.second << std::endl;
	}
	cout << endl;
	cout << "std::map:" << endl;
	for (auto &val : testMap){
		std::cout << "Key: " << val.first << "  Val: " << val.second << std::endl;
	}

	testUnorderedMap2.insert(std::begin(testMap), std::end(testMap));
	cout << endl;
	cout << "std::unordered_map 2:" << endl;
	for (auto &val : testUnorderedMap2){
		std::cout << "Key: " << val.first << "  Val: " << val.second << std::endl;
	}

	cout << "std::map:" << endl;
	for (auto &val : testMap){
		val.second = "t";
	}
	for (auto &val : testMap){
		std::cout << "Key: " << val.first << "  Val: " << val.second << std::endl;
	}

	std::cin.get();
	return 0;
}