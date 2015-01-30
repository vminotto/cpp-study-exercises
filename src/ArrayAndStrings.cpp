#include "ArrayAndStrings.h"

bool ex::hasAllUniqueCharacters(const std::string &str){
	if (str.size() <= 1)
		return true;

	size_t i = 0;
	for (const auto& c : str){
		if (str.find(c, i + 1) != std::string::npos)
			return false;
		i++;
	}
	return true;
}

bool ex::hasAllUniqueCharactersASCII(const char *str){
	size_t size = ex::getStringSize(str);
	if (size <= 1)
		return true;
	/*More characters than the number of unique ones, obviously there are repeated ones.
	This obviously assume a 8-bit ASCII-based string.*/
	else if (size > 256) 
		return true;

	/*Using new with '()' here to force all bools to initialize as 'false'*/
	bool *hasOcurred = new bool[256]();

	for (int i = 0; i < size; i++){
		if (hasOcurred[str[i]]){
			delete[] hasOcurred;
			return false;
		}
		hasOcurred[str[i]] = true;
	}
	delete[] hasOcurred;
	return true;
}
bool ex::hasAllUniqueCharactersASCII_a_to_z(const char *str){

	int maxNumberOfChars = 'z' - 'a';

	size_t size = ex::getStringSize(str);
	if (size <= 1)
		return true;
	/*More characters than the number of unique ones, obviously there are repeated ones.
	This obviously assume a 8-bit ASCII-based string.*/
	else if (size > maxNumberOfChars)
		return true;

	/*Int has 32 bits, which is enough to check the maximum of 25 possible characters (total of 25 from 'a' to 'z')*/
	int checker = 0;
	
	for (int i = 0; i < size; i++){
		int mask = 1 << (str[i] - 'a');
		if (mask & checker){
			return false;
		}
		checker |= mask;
	}
	return true;
}

bool ex::hasAllUniqueCharactersUnicode(const char *str){
	
	size_t size = ex::getStringSize(str);
	if (size <= 1)
		return true;

	/*Creating a dicionary would consume too much memory, so doing an inner check in the string
	is more efficient, in general.*/
	for (int i = 0; i < size; i++){
		for (int j = i + 1; j < size; j++){
			if (str[i] == str[j]){
				return false;
			}
		}
	}
	return true;
}

int ex::getStringSize(const char *str){
	size_t size = 0;
	/*char c = str[0];
	while (c != '\0'){
		c = str[++size];
	}*/
	while (str[size++] != '\0'){}
	return size-1;

}
void ex::swap(char &a, char &b){
	char c = a;
	a = b;
	b = c;
}
void ex::reverseString(char *str){

	size_t size = strlen(str);

	if (size <= 1) return;
	
	/*Even if size is and odd value, implicit rounding will assure correct iteration.
	If odd, center element does not need to and wont be swapped*/
	for (int i = 0; i < size/2; i++){
		int left  = i;
		int right = size - i - 1;
		std::swap(str[left], str[right]);
	}
}

bool ex::isPermutation(const std::string &strA, const std::string &strB){
	
	if (strA.size() != strB.size())
		return false;
	else if (strA == strB)
		return true;

	std::unordered_map<char, int> mapA, mapB;

	for (int i = 0; i < strA.size(); i++){
		/*Even if a given key has not been created, acessing it will automatically 
		create it, with its pair defaulty counstructed. In the case of an int, it is
		constructed to 0 by default, so the incremant also works.*/
		mapA[strA.at(i)]++;
		mapB[strB.at(i)]++;
	}

	/*This comparison works both for std::unordered_map and std::map. In the unordered case,
	the operator == is implemented in such a way that the unordering factor does not yield
	false when the ocurrance of key/value is identical.*/
	return mapA == mapB;
}



void ex::replaceCharsInString(char *str, char oldChar, char *newStr){
	/*Although this approach iterates twice through the string, it moves less memory, which might be better
	in the scenario were 'oldChar' ocurres a lot.*/
	size_t actualSize = strlen(str);

	if (actualSize <= 0)
		return;

	size_t chunkSize = strlen(newStr);

	int i = 0;
	while (char *tmpPtr = strchr(str + i, oldChar)){
		memmove(tmpPtr + chunkSize, tmpPtr + 1, (actualSize - i)*sizeof(char)); /*Including '\0' char*/
		memcpy(tmpPtr, newStr, chunkSize*sizeof(char));
		i += chunkSize;
		actualSize += chunkSize;
	}

}

std::string ex::getCompressedString(const std::string &str){
	
	///*No adjacent elements found, return original string*/
	//if (std::adjacent_find(str.begin(), str.end()) == str.end()){
	//	return str;
	//}

	/*No compression will happen, so we return the original string*/
	if (ex::getCompressionFactor <= 0)
		return str;

	/*Compressed string*/
	std::stringstream strSream;

	std::string::const_iterator first, last;
	first = str.begin();

	while (first != str.end()){
		
		last = std::adjacent_find(first, str.end());
		
		if (first == last) /*Adjacency found on first element*/
			strSream << *first;
		else               /*Adjacency found on a middle element*/
			strSream << std::string(first, last);
		
		
		int count = 0;
		char c = *first;
		while (first != str.end() && *first == c){
			first++;
			count++;
		}
		strSream << count; /*c++ std::string is mutable, so this is efficient*/
	}
	return strSream.str();
}

int ex::getCompressionFactor(const std::string &str){
	
	if (str.size() == 0)
		return 0;

	int totalCompression = 0;
	/*-1 because letters alone actually consume space in the compression, while pairs cause
	no gain in compression and triples or larger cause compression gain.*/
	int localCompression = -1; 
	for (int i = 0; i < str.size()-1; i++){
		if (str.at(i) == str.at(i + 1)){
			localCompression++;
		}
		else{
			totalCompression += localCompression;
			localCompression = -1;
		}
	}
	/*Last iteration is excluded, so we add here the last computation.*/
	totalCompression += localCompression;
	/* 0 = no compression (string will have same size)
	  >0 =    compression (string will be shrunk)
	  <0 = opposite to compression (string will have a larger size)*/
	return totalCompression;
}

void ex::printMatrix(float **&mat, int N){
	
	for (int i = 0; i < N; i++){
		for (int j = 0; j < N; j++){
			std::cout << mat[j][i] << " ";
		}
		std::cout << std::endl;
	}
}

void ex::rotateSquareMatrix(float **&mat, int totalSize){
	
	if (totalSize == 1)
		return;
	
	//float mat[5][5];
	//int count = 0;
	//for (int i = 0; i < 5; i++){
	//	for (int j = 0; j < 5; j++){
	//		mat[j][i] = count++;
	//	}
	//}

	/*Rotatin counter clock wise*/
	for (int layer = 0; layer < totalSize / 2; layer++){
		int N  = totalSize - layer;
		int first    = layer;
		int last = totalSize  - layer-1;
		for (int i = first; i < last; i++){
			
			/*Backwards i*/
			int bi = last - i + first;

			int offset = first + 1;

			/*Keep top*/
			float temp = mat[first][i];
			
			/*Top receives left*/
			mat[first][i] = mat[bi][first];

			/*Left receives bottom*/
			mat[bi][first] = mat[last][bi];

			/*Bottom receives right*/
			mat[last][bi] = mat[i][last];
		
			/*Right receives top*/
			mat[i][last] = temp;
		}
	}

}
void ex::rotate_bool(float **matrix, int n) {
	for (int layer = 0; layer < n / 2; ++layer) {
		int first = layer;
		int last = n - 1 - layer;
		for (int i = first; i < last; ++i) {
			int offset = i - first;
			// save top
			int top = matrix[first][i];

			// left -> top
			matrix[first][i] = matrix[last - offset][first];

			// bottom -> left
			matrix[last - offset][first] = matrix[last][last - offset];

			// right -> bottom
			matrix[last][last - offset] = matrix[i][last];

			// top -> right
			matrix[i][last] = top;

		}

	}

}

void ex::zeroElements(int **matrix, int rowFirst, int rowLast, int colFirst, int colLast){
	
	int nRows = rowLast - rowFirst + 1;
	int nCols = colLast - colFirst + 1;

	/*No more elements to zero*/
	if (nRows <= 0 || nCols <= 0)
		return;

	/*Flaggin a zero was found and we have to break both fors*/
	bool found = false;

	for (int j = rowFirst; j <= rowLast && !found; j++){
		for (int i = colFirst; i <= colLast && !found; i++){
			if (matrix[j][i] == 0){				
				/*Zeroing the row and col*/
				for (int c = colFirst; c <= colLast; c++) matrix[j][c] = 0;
				for (int r = rowFirst; r <= rowLast; r++) matrix[r][i] = 0;

				/*Now there are 4 sub-matrices remaining*/				
				/*Top-left sub-matrix*/
				zeroElements(matrix, 0, j - 1, 0, i - 1);

				/*Top-right sub-matrix*/
				zeroElements(matrix, 0, j - 1, i+1, colLast);

				/*Bottom-left sub-matrix*/
				zeroElements(matrix, j+1, rowLast, 0, i-1);

				/*Bottom-right sub-matrix*/
				zeroElements(matrix, j + 1, rowLast, i+1, colLast);
				
				found = true;
			}
		}
	}
}

void ex::zeroElements(int **matrix, int nRows, int nCols){

	if (nRows <= 0 || nCols <= 0)
		throw "Error (or some other exception-specific class).";

	/*Initiating values to 0 through the () operator*/
	bool *rowFlags = new bool[nRows]();
	bool *colFlags = new bool[nCols]();

	for (int j = 0; j < nRows; j++){
		if (!rowFlags[j]){
			for (int i = 0; i < nCols; i++){
				if (!colFlags[i] && matrix[j][i] == 0){

					for (int c = 0; c < nCols; c++) matrix[j][c] = 0;
					for (int r = 0; r < nRows; r++) matrix[r][i] = 0;

					rowFlags[j] = true;
					colFlags[i] = true;
					
					/*This break is necessary so that we do not test true the zeros we have just set*/
					break;
				}
			}
		}
	}

	delete[] rowFlags;
	delete[] colFlags;

}


bool ex::isRotation(const std::string &base, std::string &rotation){
	if (base.size() != rotation.size())
		return false;

	/*This means they are identical, i.e., no 'rotation' has no rotation
	Depending on the definition of 'rotaiton' this could mean a 0-characters
	rotation, which then should return true. But we assume it should be false.*/
	if (base == rotation)
		return false;

	int size = base.size();

	std::string temp = rotation + rotation;

	return temp.find(base) != std::string::npos;
}