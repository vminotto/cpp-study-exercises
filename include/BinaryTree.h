#ifndef BINARY_TREE_H
#define BINARY_TREE_H

#include <iostream>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <iostream>
#include <cmath>
#include <initializer_list>
#include <vector>

using namespace std;

namespace ex{

	template <class T> struct TreeNode{
		
		TreeNode() = default;
		TreeNode(T element) : element(element){}

		TreeNode<T> *left  = nullptr;
		TreeNode<T> *right = nullptr;
		TreeNode<T> *prev  = nullptr;

		T element;
		std::uint32_t smallerElements = 0;
		bool isLeaf(){ return !left && !right; }
	};

	template <class T> class BinaryTree{

	public:

		enum class TraversalType : std::uint8_t{
			DepthFirst_PreOrder,
			DepthFirst_InOrder,
			DepthFirst_PostOrder,
			BreadthFirst_LeftToRight,
			BreadthFirst_RightToLeft
		};

		void connectToParents(){			
			
			traversePostOrderNode(root,
				[this](TreeNode<T> *node){
					if (node->left)  node->left->prev = node;
					if (node->right) node->right->prev = node;
			}
			);
		}
		
		BinaryTree() = default;
		BinaryTree(std::initializer_list<T> list, TraversalType createType = DepthFirst_PreOrder){
			int size = list.size();
			int depth = sizeToDepth(size);
			int actualDepth = 0;
			switch (createType){
			case TraversalType::DepthFirst_PreOrder:  createPreOrder(root, list.begin(), depth, actualDepth, size); break;
			case TraversalType::DepthFirst_InOrder:   createInOrder(root, list.begin(), depth, actualDepth, size); break;
			case TraversalType::DepthFirst_PostOrder: createPostOrder(root, list.begin(), depth, actualDepth, size); break;
			}
		}

		void for_each(     std::function<void(T element     )> func = [](T t){}, TraversalType type = DepthFirst_PreOrder){
			switch (type){
			case TraversalType::DepthFirst_PreOrder:  traversePreOrderData(root, func); break;
			case TraversalType::DepthFirst_InOrder:   traverseInOrderData(root, func); break;
			case TraversalType::DepthFirst_PostOrder: traversePostOrderData(root, func); break;
			case TraversalType::BreadthFirst_LeftToRight: traverseBFSLeftToRightData(root, func); break;
			case TraversalType::BreadthFirst_RightToLeft: traverseBFSRightToLeftData(root, func); break;
			}
		}
		void for_each_node(std::function<void(TreeNode<T> *n)> func = [](TreeNode<T> *n){}, TraversalType type = DepthFirst_PreOrder){
			switch (type){
			case TraversalType::DepthFirst_PreOrder:  traversePreOrderNode(root, func); break;
			case TraversalType::DepthFirst_InOrder:   traverseInOrderNode(root, func); break;
			case TraversalType::DepthFirst_PostOrder: traversePostOrderNode(root, func); break;
			case TraversalType::BreadthFirst_LeftToRight: traverseBFSLeftToRightNode(root, func); break;
			case TraversalType::BreadthFirst_RightToLeft: traverseBFSRightToLeftNode(root, func); break;
			}
		}

		TreeNode<T> *getNodeOf(T element){
			TreeNode<T> *foundNode = nullptr;
			traversePostOrderNode(root, 
				[=, &foundNode](TreeNode<T> *n){
				if (n->element == element)
					foundNode = n;
			}
			);
			return foundNode;
		}

		void print(TraversalType type = DepthFirst_PreOrder){
			this->for_each([](T el){std::cout << el << " "; }, type);
		}

		TreeNode<T> *&getRoot(){ return root; }

		/*4.1 Implement a function to check if a binary tree is balanced. For the purposes of
		this question, a balanced tree is defined to be a tree such that the heights of the
		two subtrees of any node never differ by more than one.*/
		bool isBalanced(){
			if (!root)
				throw "Three does not exist.\n";
			else if (!root->left && !root->right)
				return true;

			bool foundUnbalancedNode = false;
			int rootHeight = getHeight(root, foundUnbalancedNode);
			return !foundUnbalancedNode;
		}

		/*This method is for checking tree balance*/
		int getHeight(TreeNode<T> *node, bool &foundUnbalancedNode){
			/*The condition after || allows us to save some calls to getHeight()*/
			if (!node || foundUnbalancedNode){
				return -1;
			}
			else{
				int hLeft = getHeight(node->left, foundUnbalancedNode);
				int hRight = getHeight(node->right, foundUnbalancedNode);
				
				if (std::abs(hLeft - hRight) > 1)
					foundUnbalancedNode = true;

				return std::max(hLeft, hRight) + 1;
			}
		}
		/*This method is for simply getting the height*/
		int getHeight(TreeNode<T> *node){
			if (!node){
				return -1;
			}
			else{
				int hLeft = getHeight(node->left);
				int hRight = getHeight(node->right);

				return std::max(hLeft, hRight) + 1;
			}
		}

	private:
		TreeNode<T> *root = nullptr;

		/*4.3 Given a sorted (increasing order) array with unique integer elements, write an
		algorithm to create a binary search tree with minimal height*/
		
		/*Given numElements, returns depth of binary tree*/
		int sizeToDepth(int numElements){
			return (int)std::log2(numElements) + 1;
		}

		/*Root, Left, Right*/
		void traversePreOrderData(TreeNode<T> *node, std::function<void(T element)> doSomething){
			if (!node)
				return;
			doSomething(node->element);
			traversePreOrderData(node->left, doSomething);
			traversePreOrderData(node->right, doSomething);
		}
		void traversePreOrderNode(TreeNode<T> *node, std::function<void(TreeNode<T> *n)> doSomething){
			if (!node)
				return;
			doSomething(node);
			traversePreOrderNode(node->left, doSomething);
			traversePreOrderNode(node->right, doSomething);
		}
		template <class Iterator> 
		void createPreOrder(TreeNode<T> *&node, Iterator &&it, int maxDepth, int myDepth, int &nodesToCreate){

			if (myDepth >= maxDepth)return;
			if (node)				return;
			if (nodesToCreate <= 0)	return;

			node = new TreeNode<T>(*it++);
			myDepth++;
			nodesToCreate--;

			createPreOrder(node->left, it, maxDepth, myDepth, nodesToCreate);
			createPreOrder(node->right, it, maxDepth, myDepth, nodesToCreate);
		}

		/*Left, Root, Right*/
		void traverseInOrderData(TreeNode<T> *node, std::function<void(T element)> doSomething){
			if (!node)
				return;
			traverseInOrderData(node->left, doSomething);
			doSomething(node->element);
			traverseInOrderData(node->right, doSomething);
		}
		void traverseInOrderNode(TreeNode<T> *node, std::function<void(TreeNode<T> *n)> doSomething){
			if (!node)
				return;
			traverseInOrderNode(node->left, doSomething);
			doSomething(node);
			traverseInOrderNode(node->right, doSomething);
		}
		template <class Iterator> 
		void createInOrder(TreeNode<T> *&node, Iterator &&it, int maxDepth, int myDepth, int &nodesToCreate){

			if (myDepth >= maxDepth)return;
			if (node)				return;
			if (nodesToCreate <= 0)	return;

			node = new TreeNode<T>();
			myDepth++;
			nodesToCreate--;

			createInOrder(node->left, it, maxDepth, myDepth, nodesToCreate);
			node->element = *it++;
			createInOrder(node->right, it, maxDepth, myDepth, nodesToCreate);

		}

		/*Left, Right, Root*/
		void traversePostOrderData(TreeNode<T> *node, std::function<void(T element)> doSomething){
			if (!node)
				return;
			traversePostOrderData(node->left, doSomething);
			traversePostOrderData(node->right, doSomething);
			doSomething(node->element);
		}
		void traversePostOrderNode(TreeNode<T> *node, std::function<void(TreeNode<T> *n)> doSomething){
			if (!node)
				return;
			traversePostOrderNode(node->left, doSomething);
			traversePostOrderNode(node->right, doSomething);
			doSomething(node);
		}
		template <class Iterator> 
		void createPostOrder(TreeNode<T> *&node, Iterator &&it, int maxDepth, int myDepth, int &nodesToCreate){

			if (myDepth >= maxDepth)return;
			if (node)				return;
			if (nodesToCreate <= 0)	return;

			node = new TreeNode<T>();
			myDepth++;
			nodesToCreate--;

			createPostOrder(node->left, it, maxDepth, myDepth, nodesToCreate);
			createPostOrder(node->right, it, maxDepth, myDepth, nodesToCreate);
			node->element = *it++;
		}
		

		void traverseBFSLeftToRightNode(TreeNode<T> *node, std::function<void(TreeNode<T> *n)> doSomething){
			std::queue<TreeNode<T>*> que;
			que.push(node);
			while (!que.empty())
			{
				node = que.front();
				doSomething(node);
				
				if(node->left)			que.push(node->left);
				if(node->right)			que.push(node->right);		
				
				que.pop();
			}
		}
		void traverseBFSLeftToRightData(TreeNode<T> *node, std::function<void(T element)> doSomething){
			std::queue<TreeNode<T>*> que;
			que.push(node);
			while (!que.empty())
			{
				node = que.front();
				doSomething(node->element);

				if (node->left)			que.push(node->left);
				if (node->right)			que.push(node->right);

				que.pop();
			}
		}

		void traverseBFSRightToLeftNode(TreeNode<T> *node, std::function<void(TreeNode<T> *n)> doSomething){
			std::queue<TreeNode<T>*> que;
			que.push(node);
			while (!que.empty())
			{
				node = que.front();
				doSomething(node);

				if (node->right)			que.push(node->right);
				if (node->left)			que.push(node->left);

				que.pop();
			}
		}
		void traverseBFSRightToLeftData(TreeNode<T> *node, std::function<void(T element)> doSomething){
			std::queue<TreeNode<T>*> que;
			que.push(node);
			while (!que.empty())
			{
				node = que.front();
				doSomething(node->element);

				if (node->right)			que.push(node->right);
				if (node->left)			que.push(node->left);

				que.pop();
			}
		}
	};
};

#endif