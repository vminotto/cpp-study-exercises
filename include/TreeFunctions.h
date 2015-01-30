#ifndef TREE_FUNCTIONS_H
#define TREE_FUNCTIONS_H

#include "BinaryTree.h"
#include <functional>
#include <stack>
#include <queue>
#include <iomanip>
#include <deque>
#include <iomanip>
#include <sstream>
#include <string>
#include <list>
#include <vector>
#include <random>
#include <iterator>
#include <algorithm>

namespace ex{

	using namespace std;
	using uchar = unsigned char;

	/*4.9 You are given a binary tree in which each node contains a value. Design an algorithm
	to print all paths which sum to a given value. The path does not need to
	start or end at the root or a leaf.*/
	template <class T> void findPath(TreeNode<T> *node, std::vector<T> &values, T refSum, int prevDepth)
	{
		if (!node){
			return;
		}

		int myDepth = prevDepth + 1;
		
		if (values.size() < myDepth)
			values.push_back(node->element);
		else
			values.at(prevDepth) = node->element;

		T sum = 0;
		for (int i = myDepth-1; i >= 0; i--){
			sum += values.at(i);
			if (sum == refSum){
				//ex::printRange(values.begin() + i, values.begin() + myDepth);
				std::copy(values.begin() + i, values.begin() + myDepth, std::ostream_iterator<std::vector<T>::iterator::value_type>(std::cout, " "));
				cout << endl;
			}
		}

		findPath(node->left,  values, refSum, myDepth);
		findPath(node->right, values, refSum, myDepth);

		/* Removing current value from path. Not strictly necessary, since
		we would ignore this value, but it's good practice. */
		values.at(prevDepth) = std::numeric_limits<T>::min();
	}

	template <class T> void testFindPath(TreeNode<T> *root, std::vector<T> refValues){
		for (auto val : refValues){
			std::vector<uint> tempVec;
			cout << "All paths that sum to " << val <<":" <<endl;
			ex::findPath(root, tempVec, val, 0);
			cout << endl;
		}
	}

	//template <class Iterator> void printRange(Iterator begin, Iterator end){
	//	for (Iterator it = begin; it != end; ++it){
	//		cout << *it << " ";
	//	}
	//	cout << endl;
	//}


	/*4.8 You have two very large binary trees: Tl, with millions of nodes, and T2, with
	hundreds of nodes. Create an algorithm to decide if T2 is a subtree of Tl.
	A tree T2 is a subtree of Tl if there exists a node n in Tl such that the subtree of
	n is identical to T2. That is, if you cut off the tree at node n, the two trees would
	be identical.*/
	template <class T> bool isT2SubTreeOfT1(BinaryTree<T>& T2, BinaryTree<T>& T1){
		std::list<T> t2AsList;

		T2.for_each([&](T t){
			t2AsList.push_back(t);
		}, ex::BinaryTree<T>::TraversalType::DepthFirst_PostOrder);

		bool isChecking = false;
		bool isSubTree = false;
		isSubTree = ex::isListSubTree(T1.getRoot(), t2AsList, t2AsList.begin(), isChecking);

		return isSubTree;
	}
	/*Checks whether the list is a subtree of T1 in Post-Order. Here post-order is necessary so that we do not
	return true if T2 is contained in T1, but is not a subtree of T1. Notice that if T2 is a subtree of T1, it
	means T2 is contained in T1. However, if T2 is contained in T1, it does not necessarily mean that T2 is a subtree of T1.
	Traversing in post-order ensures we take this fact into consideration.*/
	template <class T, class It> bool isListSubTree(TreeNode<T> *node, std::list<T> &nodesList, It &&iter, bool &isChecking){

		if (!node)
			return false;

		bool isSubTree = false;

		/*The |= (or-equal operator) allows us to propagate the result all the way to the first call, as the stack
		of the recursing is being emptied*/
		isSubTree |= isListSubTree(node->left, nodesList, iter, isChecking);
		isSubTree |= isListSubTree(node->right, nodesList, iter, isChecking);

		if (isChecking && iter == nodesList.end()){ //Means we found the subtree!
			isChecking = false;
			isSubTree |= true;
		}

		if (isChecking){
			if (*iter == node->element){ /*Means we are on the right path, so we keep searching!*/
				++iter;
			}
			else{ //Means we were on the right path, but a node in the midle differed. So we reset the search.
				iter = nodesList.begin();
				isChecking = false;
			}
		}

		/*This 'if' is placed here so that we may start searching again even right after a failed search.
		That is, if the actual node is already equal to the root of the subtree, we reinitialize the search.*/
		if (!isChecking && node->element == nodesList.front()){
			isChecking = true;
			++iter;
		}

		return isSubTree;
	}

	/*4.7 Design an algorithm and write code to find the first common ancestor of two
	nodes in a binary tree. Avoid storing additional nodes in a data structure. NOTE:
	This is not necessarily a binary search tree.*/
	template <class T> TreeNode<T> *getFirstCommonAncestor(TreeNode<T> *root, TreeNode<T> *p, TreeNode<T> *q){

		if (!root || !p || !q)
			throw std::string("Error at 'ex::getFirstCommonAncestor(..)' no parameters may be null.\n");
		if (p == root || q == root)
			throw std::string("Error at 'ex::getFirstCommonAncestor(..)': 'p' and 'q' must differ from 'root'.\n");

		TreeNode<T> *commonAncestor = nullptr;
		getFirstCommonAncestorRec(root, p, q, commonAncestor);

		if (!commonAncestor)
			std::cerr << "Warning at 'ex::getFirstCommonAncestor(..)': no common ancestors were found. Possibly "
			"'p' and 'q' are in different trees (where the tree starts at the passed 'root')." << std::endl;

		return commonAncestor;
	}

	bool areBitsSet(uchar x, uchar mask){
		return !((x & mask) ^ mask);
	}
	//bool areBitsSet2(uchar x, uchar mask){
	//	return (x & mask) == mask;
	//}

	//                        p q
	/*uchar bits: | | | | | | | |*/
	template <class T> uchar getFirstCommonAncestorRec(TreeNode<T> *node, TreeNode<T> *p, TreeNode<T> *q, TreeNode<T> *&commonAncestor)
	{
		if (!node)
			return 0;

		uchar path = 0;

		path |= getFirstCommonAncestorRec(node->left, p, q, commonAncestor);
		path |= getFirstCommonAncestorRec(node->right, p, q, commonAncestor);

		/*Checking in post-order will assure all nodes below the root have been visited already, which is
		essential for this algorithm.*/

		/*If both bits 0b00000001 and 0b00000010 are set (that is path == 0b00000011), this
		means we reached the first common ancestor of nodes p and q.*/
		if (areBitsSet(path, 3) && !commonAncestor){
			commonAncestor = node;
			return 0;
		}

		if (node == p && node == q) //This ifs covers the case that p == q (which might have been an user error during the call of the function).
			path |= uchar(3);
		if (node == p)       //If node p is found, we flag that we are on right path (path which will go upwards)
			path |= uchar(1);
		else if (node == q)  //If node q is found, we flag that we are on right path (path which will go upwards)
			path |= uchar(2);

		return path;
	}

	/*4.6 Write an algorithm to find the'next'node (i.e., in-order successor) of a given node
	in a binary search tree. You may assume that each node has a link to its parent.*/
	template <class T> TreeNode<T> *getNextNodeInBST_InOrder(TreeNode<T> *node){

		if (!node) return nullptr;

		if (node->right)
		{
			TreeNode<T> *foundNode = getLeftMost(node->right);
			if (foundNode == node->right) //Could not go to the left,
				return node->right;       //therefore, next is the right one.
			else
				return foundNode;
		}
		else{
			if (node->prev){ //If is not root
				return getFirstParentWithLeftSon(node);
			}
			else            //Means this is the root
				return node;
		}
	}

	template <class T> bool isLeftSon(TreeNode<T> *node){
		return node->prev && node->prev->left == node;
	}
	template <class T> bool isRightSon(TreeNode<T> *node){
		return node->prev && node->prev->right == node;
	}

	template <class T> TreeNode<T> *getLeftMost(TreeNode<T> *node){

		while (node->left){
			node = node->left;
		}
		return node;
	}
	template <class T> TreeNode<T> *getFirstParentWithLeftSon(TreeNode<T> *node){

		TreeNode<T> *ref = node;

		while (node->prev && !isLeftSon(node)){
			node = node->prev;
		}

		/*If we reached null, we return the initial node (ref), which means the queried
		element has no successors in-order.*/
		return node->prev ? node->prev : ref;
	}




	template <class T> bool isInRange(T num, T minVal, T maxVal){
		return num >= minVal && num <= maxVal;
	}

	/*4.5 Imp/emen t a function to check if a binary tree is a binary search tree.*/
	template <class T> bool isBST(TreeNode<T> *node){
		/*We start by assuming the tree is a BST.*/
		bool foundWrongConstruct = false;
		isBSTRec(node, std::numeric_limits<T>::min(), std::numeric_limits<T>::max(), foundWrongConstruct);

		return !foundWrongConstruct;
	}
	template <class T> void isBSTRec(TreeNode<T> *node, T minVal, T maxVal, bool &foundWrongConstruct){
		if (foundWrongConstruct || !node)
			return;

		if (!isInRange(node->element, minVal, maxVal)){
			foundWrongConstruct = true;
			return;
		}
		if (node->left){
			if (node->element < node->left->element){
				foundWrongConstruct = true;
				return;
			}
		}

		if (node->right){
			if (node->element >= node->right->element){
				foundWrongConstruct = true;
				return;
			}
		}

		isBSTRec(node->left, minVal, node->element, foundWrongConstruct);
		isBSTRec(node->right, node->element, maxVal, foundWrongConstruct);
	}

	/*4.4 Given a sorted (increasing order) array with unique integer elements, write an
	algorithm to create a binary search tree with minimal height.*/
	template <class T> vector<list<T>*> *getNodesAsVectorOfLists(BinaryTree<T>* tree){
		vector<list<T>*> *nodesLists = new vector<list<T>*>();
		if (tree->getRoot())
			fillListWithNodes(nodesLists, tree->getRoot(), 1);

		return nodesLists;
	}

	template <class T> void fillListWithNodes(vector<list<T>*> *&nodesVec, TreeNode<T> *node, int myDepth){
		if (!node)
			return;

		if (myDepth > nodesVec->size())
			nodesVec->push_back(new std::list<T>());

		nodesVec->at(myDepth - 1)->push_back(node->element);

		fillListWithNodes(nodesVec, node->left, myDepth + 1);
		fillListWithNodes(nodesVec, node->right, myDepth + 1);
	}

	/*Given a sorted (increasing order) array with unique integer elements, write an
	algorithm to create a binary search tree with minimal height.*/
	template <class T> void createBST(TreeNode<T> *&root, std::vector<T> vec){
		asBinarySearchTreeRec2(root, vec, 0, vec.size() - 1);
	}
	template <class T> void asBinarySearchTreeRec2(TreeNode<T> *&node, std::vector<T> &vec, int first, int last){

		if (last < first || first > last)
			return;

		int mid = (first + last) / 2;
		node = new TreeNode<T>(vec.at(mid));
		asBinarySearchTreeRec2(node->left, vec, first, mid - 1);
		asBinarySearchTreeRec2(node->right, vec, mid + 1, last);
	}
	template <class T> void createBST(ex::BinaryTree<T>* tree, std::vector<T> vec){
		tree->getRoot() = asBinarySearchTreeRec1(vec, 0, vec.size() - 1);
	}
	template <class T> TreeNode<T> *asBinarySearchTreeRec1(std::vector<T> &vec, int first, int last){
		/*|0-1-2-3|-|4|-|5-6-7-8|*/
		if (last < first || first > last)
			return std::nullptr_t();

		int mid = (first + last) / 2;
		TreeNode<T> *node = new TreeNode<T>(vec.at(mid));
		node->left = asBinarySearchTreeRec1(vec, first, mid - 1);
		node->right = asBinarySearchTreeRec1(vec, mid + 1, last);
		return node;
	}

	template <class T> void asBinarySearchTreeIter(std::vector<T> vec){
		std::sort(vec.begin(), vec.end());
		std::unique(vec.begin(), vec.end());

		std::queue<TreeNode<T>*> que;

		TreeNode<T>* node;
		int size = vec.size();
		int depth = sizeToDepth(size);
		que.push(new TreeNode<T>(vec.at((size - 1) / 2)));
		root = que.front();

		for (int i = 1; i < depth; i++){
			/*Temporaries to make to algorithm more clear*/

			int numEl = std::exp2(i);
			int stepS = (size - 1) / std::exp2(i + 1);
			//int stepS = i;
			//int stepS = depth - i;
			int stepB = std::exp2(depth - i);
			//int stepB = 2*(depth - i);

			int subSize =
				for (int k = 0, j = stepS; k < numEl; k++, j += stepB){

					node = que.front();

					if (!node->left){
						node->left = new TreeNode<T>(vec.at(j));
					}
					else if (!node->right && j < size){
						node->right = new TreeNode<T>(vec.at(j));
						que.push(node->left);
						que.push(node->right);
						que.pop(); //pops front
					}
					else{
						que.push(node->left);
						que.push(node->right);
						que.pop(); //pops front
					}
				}
		}
	}


	/*Given an unsorted array, creates a BST in the order the of appearence of the elements.*/
	template <class Iterator> ex::BinaryTree< typename Iterator::value_type >
		*createBST(Iterator begin, Iterator end) {
		
		using T = typename std::iterator_traits<Iterator>::value_type;

		ex::BinaryTree<T> *tree = new ex::BinaryTree<T>();

		if (begin == end)
			return tree;

		TreeNode<T> *root = new TreeNode<T>(*begin++);

		for (auto it = begin; it != end; ++it)
			insertInBST(root, *it);
		
		tree->getRoot() = root;

		return tree;
	}
	/*This function should be called internally, and not by the user. The node parameter should be the root.*/
	template <class T> void insertInBST(TreeNode<T> *&node, T &val){
		//Traverse tree until we find a 'null', that is, the right one to inser the element
		if (node){
			if (val > node->element)
				insertInBST(node->right, val);
			else{
				node->smallerElements++;
				insertInBST(node->left, val);
			}
		}
		else{
			node = new ex::TreeNode<T>(val);
		}
	}

	/*11.8 Imagine you are reading in a stream of integers. Periodically, you wish to be able to
	look up the rank of a number x (the number of values less than or equal to x). Implement
	the data structures and algorithms to support these operations. That is, implement
	the method track(int x), which is called when each number is generated,
	and the methodgetRankOf'Number (int x), which returns the number of values
	less than or equal to x (not including x itself).*/
	template <class T> std::int32_t countSmallerValuesInBST(ex::BinaryTree<T> *tree, T val){
		return countSmallerValuesInBST(tree->getRoot(), val, 0);
	}
	template <class T> std::int32_t countSmallerValuesInBST(ex::TreeNode<T> *node, T val, int count){

		if (node){
			if (val > node->element)
				countSmallerValuesInBST(node->right, val, count + node->smallerElements + 1);
			else if (val < node->element){
				countSmallerValuesInBST(node->left, val, count);
			}
			else
				return count + node->smallerElements;
		}
		else{
			return -1; //Element not found, return error value.
		}

	}

	template <class T> void traversePreOrderIterative(TreeNode<T> *node,
		std::function<void(T)> func = [](T el){cout << el << " "; }){

		std::stack<TreeNode<T>*> st;
		st.push(node);

		while (!st.empty()){
			node = st.top();
			func(node->element);
			st.pop();
			if (node->right){
				st.push(node->right);
			}
			if (node->left){
				st.push(node->left);
			}
		}
	}

	template <class T> void traverseInOrderIterative(TreeNode<T> *node,
		std::function<void(T)> func = [](T el){cout << el << " "; }){

		std::stack<TreeNode<T>*> st;
		bool done = false;

		while (!done){
			if (node){
				st.push(node);
				node = node->left;
			}
			else{
				if (st.empty())
					done = true;
				else{
					node = st.top();
					st.pop();
					func(node->element);
					node = node->right;
				}
			}
		}
	}

	template <class T> void traversePostOrderIterative(TreeNode<T> *node,
		std::function<void(T)> func = [](T el){cout << el << " "; }){

		std::stack<TreeNode<T>*> st, out;
		st.push(node);
		//bool done = false;
		while (!st.empty()){
			node = st.top();
			out.push(node);
			st.pop();
			if (node->left){
				st.push(node->left);
			}
			if (node->right){
				st.push(node->right);
			}
		}
		while (!out.empty()){
			func(out.top()->element);
			out.pop();
		}

	}

	// Find the maximum height of the binary tree
	template <class T> int maxHeight(TreeNode<T> *p) {
		if (!p) return 0;
		int leftHeight = maxHeight(p->left);
		int rightHeight = maxHeight(p->right);
		return (leftHeight > rightHeight) ? leftHeight + 1 : rightHeight + 1;
	}

	// Convert an integer value to string
	template <class T> string toString(T val) {
		ostringstream ss;
		ss << val;
		return ss.str();
	}

	// Print the arm branches (eg, /    \ ) on a line
	template <class T> void printBranches(int branchLen, int nodeSpaceLen, int startLen, int nodesInThisLevel, const deque<TreeNode<T>*>& nodesQueue, ostream& out) {
		deque<TreeNode<T>*>::const_iterator iter = nodesQueue.begin();
		for (int i = 0; i < nodesInThisLevel / 2; i++) {
			out << ((i == 0) ? setw(startLen - 1) : setw(nodeSpaceLen - 2)) << "" << ((*iter++) ? "/" : " ");
			out << setw(2 * branchLen + 2) << "" << ((*iter++) ? "\\" : " ");
		}
		out << endl;
	}

	// Print the branches and node (eg, ___10___ )
	template <class T> void printNodes(int branchLen, int nodeSpaceLen, int startLen, int nodesInThisLevel, const deque<TreeNode<T>*>& nodesQueue, ostream& out) {
		deque<TreeNode<T>*>::const_iterator iter = nodesQueue.begin();
		for (int i = 0; i < nodesInThisLevel; i++, iter++) {
			out << ((i == 0) ? setw(startLen) : setw(nodeSpaceLen)) << "" << ((*iter && (*iter)->left) ? setfill('_') : setfill(' '));
			out << setw(branchLen + 2) << ((*iter) ? toString((*iter)->element) : "");
			out << ((*iter && (*iter)->right) ? setfill('_') : setfill(' ')) << setw(branchLen) << "" << setfill(' ');
		}
		out << endl;
	}

	// Print the leaves only (just for the bottom row)
	template <class T> void printLeaves(int indentSpace, int level, int nodesInThisLevel, const deque<TreeNode<T>*>& nodesQueue, ostream& out) {
		deque<TreeNode<T>*>::const_iterator iter = nodesQueue.begin();
		for (int i = 0; i < nodesInThisLevel; i++, iter++) {
			out << ((i == 0) ? setw(indentSpace + 2) : setw(2 * level + 2)) << ((*iter) ? toString((*iter)->element) : "");
		}
		out << endl;
	}

	// Pretty formatting of a binary tree to the output stream
	// @ param
	// level  Control how wide you want the tree to sparse (eg, level 1 has the minimum space between nodes, while level 2 has a larger space between nodes)
	// indentSpace  Change this to add some indent space to the left (eg, indentSpace of 0 means the lowest level of the left node will stick to the left margin)
	template <class T> void printPretty(TreeNode<T> *root, int level, int indentSpace, ostream& out) {
		int h = maxHeight(root);
		int nodesInThisLevel = 1;

		int branchLen = 2 * ((int)pow(2.0, h) - 1) - (3 - level)*(int)pow(2.0, h - 1);  // eq of the length of branch for each node of each level
		int nodeSpaceLen = 2 + (level + 1)*(int)pow(2.0, h);  // distance between left neighbor node's right arm and right neighbor node's left arm
		int startLen = branchLen + (3 - level) + indentSpace;  // starting space to the first node to print of each level (for the left most node of each level only)

		deque<TreeNode<T>*> nodesQueue;
		nodesQueue.push_back(root);
		for (int r = 1; r < h; r++) {
			printBranches(branchLen, nodeSpaceLen, startLen, nodesInThisLevel, nodesQueue, out);
			branchLen = branchLen / 2 - 1;
			nodeSpaceLen = nodeSpaceLen / 2 + 1;
			startLen = branchLen + (3 - level) + indentSpace;
			printNodes(branchLen, nodeSpaceLen, startLen, nodesInThisLevel, nodesQueue, out);

			for (int i = 0; i < nodesInThisLevel; i++) {
				TreeNode<T> *currNode = nodesQueue.front();
				nodesQueue.pop_front();
				if (currNode) {
					nodesQueue.push_back(currNode->left);
					nodesQueue.push_back(currNode->right);
				}
				else {
					nodesQueue.push_back(NULL);
					nodesQueue.push_back(NULL);
				}
			}
			nodesInThisLevel *= 2;
		}
		printBranches(branchLen, nodeSpaceLen, startLen, nodesInThisLevel, nodesQueue, out);
		printLeaves(indentSpace, level, nodesInThisLevel, nodesQueue, out);
	}


	template <class T> void test4_5(BinaryTree<T> &tree){
		cout << "Is the following three a BST?" << endl;
		printPretty(tree.getRoot(), 2, 0, std::cout);
		bool bst = ex::isBST(tree.getRoot());

		if (bst) cout << "Yes, it is a BST." << endl;
		else     cout << "No, it is not a BST." << endl;
	}

};


#endif