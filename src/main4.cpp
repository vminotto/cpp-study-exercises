#include "BinaryTree.h"
#include "TreeFunctions.h"
#include "DirectedGraph.h"
#include "GraphFunctions.h"
#include "RandomGenerator.h"
#include <vector>
#include "AGraph.h"
#include <numeric>
#include <map>

using tType = ex::BinaryTree<char>::TraversalType;

using uchar = unsigned char;
using uint = unsigned int;

int main(){
	
	ex::AGraph<char, int> gra;
	std::map<char, ex::AGraphNode<char, int>*> aNodes;
	for (char c = 'A'; c < 'A' + 14; ++c)
		aNodes[c] = new ex::AGraphNode<char, int>(c);

	gra.make_directional(aNodes['A'], aNodes['G'], 0);
	gra.make_directional(aNodes['A'], aNodes['H'], 0);
	gra.make_directional(aNodes['A'], aNodes['D'], 0);
	gra.make_directional(aNodes['A'], aNodes['B'], 0);
	gra.make_directional(aNodes['A'], aNodes['F'], 0);
	gra.make_directional(aNodes['B'], aNodes['A'], 0);
	gra.make_directional(aNodes['B'], aNodes['C'], 0);
	gra.make_directional(aNodes['B'], aNodes['D'], 0);
	gra.make_directional(aNodes['C'], aNodes['F'], 0);
	gra.make_directional(aNodes['C'], aNodes['F'], 1);
	gra.make_directional(aNodes['C'], aNodes['M'], 0);
	gra.make_directional(aNodes['C'], aNodes['N'], 0);
	gra.make_directional(aNodes['D'], aNodes['I'], 0);
	gra.make_directional(aNodes['E'], aNodes['I'], 0);
	gra.make_directional(aNodes['E'], aNodes['M'], 0);
	gra.make_directional(aNodes['F'], aNodes['B'], 0);
	gra.make_directional(aNodes['F'], aNodes['L'], 0);
	gra.make_directional(aNodes['G'], aNodes['A'], 0);
	gra.make_directional(aNodes['H'], aNodes['I'], 0);
	gra.make_directional(aNodes['H'], aNodes['M'], 0);
	gra.make_directional(aNodes['I'], aNodes['B'], 0);
	gra.make_directional(aNodes['I'], aNodes['E'], 0);
	gra.make_directional(aNodes['J'], aNodes['K'], 0);
	gra.make_directional(aNodes['J'], aNodes['N'], 0);
	gra.make_directional(aNodes['K'], aNodes['N'], 0);
	gra.make_directional(aNodes['L'], aNodes['J'], 0);
	gra.make_directional(aNodes['L'], aNodes['N'], 0);
	gra.make_directional(aNodes['M'], aNodes['B'], 0);
	gra.make_directional(aNodes['N'], aNodes['C'], 0);

	std::function<void(ex::AGraphNode<char, int>*)> printNode = [](ex::AGraphNode<char, int> *n){
		cout << n->data << " ";
	};

	std::cout << std::endl;
	gra.bfs_left_first(gra.getNodeByData('A'), printNode);
	std::cout << std::endl;
	gra.bfs_right_first(gra.getNodeByData('A'), printNode);
	std::cout << std::endl;
	gra.bfs_interleaved(gra.getNodeByData('A'), printNode);
	std::cout << std::endl;
	gra.dfs_pre_order(gra.getNodeByData('A'), printNode);
	std::cout << std::endl;
	gra.dfs_post_order(gra.getNodeByData('A'), printNode);

	char from = 'C', to = 'N';
	auto allPaths     = gra.getAllPaths(gra.getNodeByData(from), gra.getNodeByData(to));
	std::cout << endl << endl << "All paths from " << from << " to " << to << ":" << endl;
	for (auto &path : allPaths)
	{
		for (ex::AGraphNode<char,int> *&n : path){
			std::cout << "'" << *n << "' ";
		}
		std::cout << std::endl;
	}

	auto allPathsRec = gra.getAllPathsRecursive(gra.getNodeByData(from), gra.getNodeByData(to));
	std::cout << endl << endl << "All paths (recursive approach) from " << from << " to " << to << ":" << endl;
	for (auto &path : allPathsRec)
	{
		for (ex::AGraphNode<char, int> *&n : path){
			std::cout << "'" << *n << "' ";
		}
		std::cout << std::endl;
	}

	auto shortestPath = gra.getShortestPath(gra.getNodeByData(from), gra.getNodeByData(to));
	std::cout << endl << "Shortest path from " << from << " to " << to << ":" << endl;
	for (ex::AGraphNode<char, int> *&n : shortestPath){
		std::cout << "'"<<*n << "' ";
	}
	std::cout << std::endl;

	auto allPathsWithWeights = gra.getAllPathsWithWeights(gra.getNodeByData(from), gra.getNodeByData(to));
	std::cout << endl << endl << "All paths (with weights) from " << from << " to " << to << ":" << endl;
	for (auto &path : allPathsWithWeights)
	{
		for (ex::Neighbor<char,int> &nei : path){
			std::cout << nei.weight << " '" << nei.node->data<< "' ";
		}
		std::cout << std::endl;
	}

	ex::AGraphNode<char, int> *aRoot = gra.getNodeByData('A');
	ex::AGraphNode<char, int> *dst = ex::find_node_dfs<char,int>(aRoot, 'E');


	/*Testing shorted path tracing*/
	ex::DiGraph<int> graph;
	std::vector<ex::GraphNode<int>*> nodes;
	
	for (int i = 0; i <= 8; i++)
		nodes.push_back(new ex::GraphNode<int>(i));	

	graph.addConnection(nodes[0], nodes[1]);
	graph.addConnection(nodes[0], nodes[2]);
	graph.addConnection(nodes[0], nodes[3]);
	graph.addConnection(nodes[1], nodes[4]);
	graph.addConnection(nodes[2], nodes[0]);
	graph.addConnection(nodes[2], nodes[1]);
	graph.addConnection(nodes[2], nodes[4]);
	graph.addConnection(nodes[2], nodes[5]);
	graph.addConnection(nodes[2], nodes[6]);
	graph.addConnection(nodes[3], nodes[2]);
	graph.addConnection(nodes[3], nodes[7]);
	graph.addConnection(nodes[3], nodes[8]);
	graph.addConnection(nodes[4], nodes[5]);
	graph.addConnection(nodes[4], nodes[6]);
	graph.addConnection(nodes[5], nodes[6]);
	graph.addConnection(nodes[6], nodes[8]);
	graph.addConnection(nodes[7], nodes[8]);
	graph.addConnection(nodes[8], nodes[2]);
	graph.addConnection(nodes[8], nodes[6]);

	ex::printAllPaths(nodes[0], nodes[6]);

	//system("mode 130,40");

	ex::BinaryTree<char> treePre({ 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u'},
		tType::DepthFirst_InOrder);

	cout << endl << "In-order print:" << endl;
	treePre.print(tType::DepthFirst_InOrder);
	cout << endl << "Pre-order print:" << endl;
	treePre.print(tType::DepthFirst_PreOrder);
	cout << endl << "Post-order print:" << endl;
	treePre.print(tType::DepthFirst_PostOrder);
	cout << endl << "BFS left-to-right print:" << endl;
	treePre.print(tType::BreadthFirst_LeftToRight);
	cout << endl << "BFS right-to-left print:" << endl;
	treePre.print(tType::BreadthFirst_RightToLeft);
	cout << endl << "In-order (iterative) print:" << endl;
	ex::traverseInOrderIterative(treePre.getRoot());
	cout << endl << "Pre-order (iterative) print:" << endl;
	ex::traversePreOrderIterative(treePre.getRoot());
	cout << endl << "Post-order (iterative) print:" << endl;
	ex::traversePostOrderIterative(treePre.getRoot());
	ex::printPretty(treePre.getRoot(), 2, 0, std::cout);

	/*Testing 4.1*/
	ex::BinaryTree<char> tree1;
	ex::TreeNode<char> *&root = tree1.getRoot();
	root = new ex::TreeNode<char>('f');
	root->left = new ex::TreeNode<char>('b');
	root->left->left = new ex::TreeNode<char>('a');
	root->left->right = new ex::TreeNode<char>('d');
	root->left->right->left = new ex::TreeNode<char>('c');
	root->left->right->right = new ex::TreeNode<char>('e');
	root->right = new ex::TreeNode<char>('g');
	root->right->right = new ex::TreeNode<char>('i');
	root->right->right->left = new ex::TreeNode<char>('h');
	
	printPretty(root, 1, 0, std::cout);
	cout << endl << boolalpha << "Is balanced? " << tree1.isBalanced() << endl;
	root->right->left = new ex::TreeNode<char>('j');
	printPretty(root, 2, 0, std::cout);
	cout << endl << boolalpha << "Is balanced? " << tree1.isBalanced() << endl;
	root->left->right->right->left = new ex::TreeNode<char>('k');
	printPretty(root, 2, 0, std::cout);
	cout << endl << boolalpha << "Is balanced? " << tree1.isBalanced() << endl;

	/*Testing 4.2*/
	ex::DiGraph<int> digraph;
	nodes.resize(0);
	nodes.push_back(new ex::GraphNode<int>());//dummy;
	for (int i = 1; i <= 9; i++)
		nodes.push_back(new ex::GraphNode<int>(i));

	digraph.addConnection(nodes[1], nodes[2]);
	digraph.addConnection(nodes[2], nodes[3]);
	digraph.addConnection(nodes[3], nodes[4]);
	digraph.addConnection(nodes[4], nodes[4]);
	digraph.addConnection(nodes[2], nodes[6]);
	digraph.addConnection(nodes[6], nodes[7]);
	digraph.addConnection(nodes[7], nodes[4]);
	digraph.addConnection(nodes[1], nodes[5]);
	digraph.addConnection(nodes[5], nodes[3]);
	digraph.addConnection(nodes[7], nodes[8]);
	digraph.addConnection(nodes[3], nodes[6]);
	digraph.addConnection(nodes[4], nodes[9]);
	
	ex::testRoute<int>(1, 4, digraph, nodes);
	ex::testRoute<int>(2, 7, digraph, nodes);
	ex::testRoute<int>(5, 2, digraph, nodes);
	ex::testRoute<int>(4, 4, digraph, nodes);
	ex::testRoute<int>(1, 8, digraph, nodes);
	ex::testRoute<int>(5, 8, digraph, nodes);
	ex::testRoute<int>(5, 9, digraph, nodes);

	cout << "Print using breadth-first traversal" << endl;
	ex::bfs<int>(digraph.getAllNodes()->front(), [](ex::GraphNode<int> *n){cout << n->value << " "; });
	cout << endl;
	digraph.resetNodes();
	cout << "Print using depth-first pre-order traversal (iterative)" << endl;
	ex::dfsPreOrder<int>(digraph.getAllNodes()->front(), [](ex::GraphNode<int> *n){cout << n->value << " "; });
	cout << endl;
	digraph.resetNodes();
	cout << "Print using depth-first pre-order traversal (recursive)" << endl;
	ex::dfsRecPreOrder<int>(digraph.getAllNodes()->front(), [](ex::GraphNode<int> *n){cout << n->value << " "; });
	cout << endl;
	digraph.resetNodes();
	cout << "Print using depth-first post-order traversal (recursive)" << endl;
	ex::dfsRecPostOrder<int>(digraph.getAllNodes()->front(), [](ex::GraphNode<int> *n){cout << n->value << " "; });

	
	/*ex::BinaryTree<int> tree5;
	tree5.asBinarySearchTree({ 0, 1, 2, 3, 4, 5, 6});
	printPretty(tree5.getRoot(), 1, 0, std::cout);*/

	/*Testing 4.3*/	
	ex::BinaryTree<int> tree4a;
	ex::createBST<int>(&tree4a, { 0, 1, 2, 3, 4, 5, 6, 7, 8 });
	printPretty(tree4a.getRoot(), 1, 0, std::cout);
	ex::BinaryTree<int> tree4b;
	ex::createBST<int>(tree4b.getRoot(), { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 });
	printPretty(tree4b.getRoot(), 1, 0, std::cout);

	/*Testing 4.4*/
	std::vector<std::list<int>*> *nodesVec = ex::getNodesAsVectorOfLists(&tree4b);
	for (int i = 0; i < nodesVec->size(); i++){
		cout << "Depth "<<i<<": ";
		for (auto const &el : *nodesVec->at(i)){
			cout << el << " ";
		}
		cout << endl;
	}

	/*Testing 4.5*/
	cout << endl;
	ex::BinaryTree<int> tree5;
	ex::createBST<int>(&tree5, { 0, 1, 2, 3, 4, 5, 6, 7, 8 });
	test4_5(tree5);
	cout << "----------------------------" << endl;
	//tree5.getRoot()->left->left->right = new ex::TreeNode<int>(9);
	test4_5(tree5);
	tree5.getRoot()->right->left->left = new ex::TreeNode<int>(3);
	test4_5(tree5);

	/*Testing 4.6*/
	ex::BinaryTree<char> tree6({ 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u' },
		tType::DepthFirst_InOrder);	
	tree6.connectToParents();
	//tree6.getNodeOf('h')->right->left = 0; //Removing node to makes tests (memory leak intended)
	
	ex::printPretty(tree6.getRoot(), 2, 0, std::cout);
	cout << "InOrder print: ";
	tree6.print(tType::DepthFirst_InOrder);
	cout << endl << endl;
	tree6.for_each([&](char c){
		ex::TreeNode<char> *node = tree6.getNodeOf(c);
		ex::TreeNode<char> *nextNode = ex::getNextNodeInBST_InOrder(node);
		cout << "Next node of " << node->element << " is " << nextNode->element << endl;
	}, ex::BinaryTree<char>::TraversalType::DepthFirst_InOrder
	);
	ex::TreeNode<char> *t1 = nullptr, *t2 = std::nullptr_t();

	/*Testing 4.7*/
	ex::BinaryTree<char> tree7({ 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u' },
		tType::DepthFirst_PostOrder);

	ex::printPretty(tree7.getRoot(), 2, 0, std::cout);
	ex::TreeNode<char> *p = tree7.getNodeOf('n');
	ex::TreeNode<char> *q = tree7.getNodeOf('n');
	ex::TreeNode<char> *commonAncestor = ex::getFirstCommonAncestor(tree7.getRoot(), p, q);
	cout << "First common ancestor of [" << p->element << "] and [" << q->element << "] is " << commonAncestor->element<< endl;

	/*Testing 4.8*/	
	ex::BinaryTree<char> tree8_T1({ 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u',
		'v', 'x', 'y', 'z' },
		tType::DepthFirst_PreOrder);
	cout << endl << "Base Tree (T1): " << endl;
	ex::printPretty(tree8_T1.getRoot(), 2, 0, std::cout);
	ex::BinaryTree<char> tree8_T2({'h', 'i', 'j', 'k', 'l', 'm', 'n'},
		tType::DepthFirst_PreOrder);;

	cout << endl << "Possible Sub-tree (T2): " << endl;
	tree8_T2.getRoot()->right->right = nullptr;
	ex::printPretty(tree8_T2.getRoot(), 2, 0, std::cout);
	cout << "Is T2 a sub-tree of T1? " << ex::isT2SubTreeOfT1(tree8_T2, tree8_T1) << endl;

	/*Testing 4.9*/
	ex::IntegerGenerator r(1, 5);
	r.printSequence(25);

	ex::BinaryTree<uint> tree9({ 1, 2, 3, 3, 1, 2, 3, 5, 5, 1, 5, 4, 4, 1, 3, 1, 2, 1, 4, 3, 3, 4, 4, 3, 2, 2, 4 },
		ex::BinaryTree<uint>::TraversalType::DepthFirst_PostOrder);
	ex::printPretty(tree9.getRoot(), 2, 0, std::cout);
	ex::testFindPath<uint>(tree9.getRoot(), {8,9,11});
	
	std::cin.get();
	return 0;
}