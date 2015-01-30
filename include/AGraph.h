#ifndef ADJACENCY_LIST_GRAH_H
#define ADJACENCY_LIST_GRAH_H

#include <list>
#include <vector>
#include <algorithm>
#include <functional>
#include <queue> 
#include <initializer_list>

namespace ex{

	/*Forward declaration*/
	template <class D, class W> struct Neighbor;
	template <class D, class W> struct AGraphNode;

	template <class D, class W> class AGraph{

		using Neighbor = ex::Neighbor<D, W>;
		using Node = AGraphNode<D, W>;

	public:

		using type = D;

		AGraph() = default;
		AGraph(std::initializer_list<AGraphNode<D, W>> list){
			for (Node &n : list){
				this->push(new GraphNode<D, W>(n));
			}
		}
		AGraph(std::initializer_list<AGraphNode<D, W>*> list){
			for (Node *n : list){
				this->push(new GraphNode<D, W>(*n));
			}
		}


		~AGraph(){
			for (Node *n : nodes)
				delete n;
		}

		/*Pushes a node into this graph, which will have no connections initially.*/
		Node *&push(Node *node){
			auto nodePos = std::find(nodes.begin(), nodes.end(), node);
			if (nodePos == nodes.end()){
				nodes.push_back(node);
				return nodes.back();
			}
			else
				return *nodePos;

		}

		/*Erase a node from this graph.*/
		void erase(Node *node){
			auto nodePos = std::find(nodes.begin(), nodes.end(), node);
			if (nodePos != nodes.end()){
				nodes.erase(nodePos);
				delete node;
			}
		}

		/*Returns the first node that has the given 'data'.*/
		Node *getNodeByData(D data){
			auto nodePos = std::find_if(nodes.begin(), nodes.end(), [&](Node *n){
				return n->data == data;
			});
			if (nodePos != nodes.end())
				return *nodePos;
			else
				return nullptr;
		}

		void make_bidirectional(Node *a, Node *b){
			make_directional(a, b, 0);
			make_directional(b, a, 0);
		}
		void make_bidirectional(Node *a, Node *b, W weightAtoB, W weightBtoA){
			make_directional(a, b, weightAtoB);
			make_directional(b, a, weightBtoA);
		}

		/*Creates a directional weight edge in the graph. If the nodes don't already
		exist, they are created and inserted.*/
		void make_directional(Node *from, Node *to, W weight = W()){
			this->push(from)->addConnection(this->push(to), weight);
		}

		void resetVisitStatus(){
			for (Node *n : nodes){
				n->status = Node::Status::Unvisited;
			}
		}

		void bfs_left_first(AGraphNode<D, W> *node, std::function<void(AGraphNode<D, W>*)> func);
		void bfs_right_first(AGraphNode<D, W> *node, std::function<void(AGraphNode<D, W>*)> func);
		void bfs_interleaved(AGraphNode<D, W> *node, std::function<void(AGraphNode<D, W>*)> func, bool leftToRight = true);

		void dfs_pre_order(AGraphNode<D, W> *node, std::function<void(AGraphNode<D, W>*)> func){
			_dfs_pre_order(node, func);
			resetVisitStatus();
		}
		void dfs_post_order(AGraphNode<D, W> *node, std::function<void(AGraphNode<D, W>*)> func){
			/*In post order this is necessary because nodes arevisited last, in back-propagation
			order, so this means the node passed as paramter may be stacked twice for printing.*/
			node->status = AGraphNode<D, W>::Status::ToBeVisited;
			_dfs_post_order(node, func);
			resetVisitStatus();
		}

		std::list<std::list<AGraphNode<D, W>*>> getAllPaths(Node *from, Node *to);
		std::list<std::list<AGraphNode<D, W>*>> getAllPathsRecursive(Node *from, Node *to){
			std::list<std::list<Node*>> allPaths;
			std::list<Node*> actualPath;
			actualPath.push_back(from);

			this->_get_all_paths_recursive(actualPath, to, allPaths);

			return allPaths;
		}

		std::list<AGraphNode<D, W>*> getShortestPath(Node *from, Node *to){
		
			std::list<AGraphNode<D, W>*> shortestPath;
			if (!from || !to){
				return shortestPath;
			}
			else if (from == to){
				shortestPath.push_back(from);
				return shortestPath;
			}
			
			auto allPaths = getAllPaths(from, to);

			if (allPaths.empty()){
				return shortestPath;
			}

			auto minPos = std::min_element(allPaths.begin(), allPaths.end(), []
				(std::list<AGraphNode<D, W>*> &pathA, std::list<AGraphNode<D, W>*> &pathB)
			{
				return pathA.size() < pathB.size();
			});

			if (minPos != allPaths.end())
				shortestPath = *minPos;
			
			return shortestPath;
		}

		std::list<std::list<Neighbor>> getAllPathsWithWeights(Node *from, Node *to);

	private:

		void _dfs_pre_order(AGraphNode<D, W> *node, std::function<void(AGraphNode<D, W>*)> func);
		void _dfs_post_order(AGraphNode<D, W> *node, std::function<void(AGraphNode<D, W>*)> func);
		void _get_all_paths_recursive(std::list<Node*> prevPath, Node *to, std::list<std::list<Node*>> &allPaths);
		
		/*This is the master list holding all nodes of this graph.*/
		std::list<AGraphNode<D, W>*> nodes;

	};

	template <class D, class W> void ex::AGraph<D, W>::bfs_left_first(
		AGraphNode<D, W> *node, std::function<void(AGraphNode<D, W>*)> func)
	{
		std::queue <AGraphNode<D, W>*> que;
		que.push(node);
		while (!que.empty()){
			node = que.front();
			func(node);
			node->status = AGraphNode<D, W>::Status::Visited;
			for (Neighbor &nei : node->neighbors){
				if (nei.node->status == AGraphNode<D, W>::Status::Unvisited){
					que.push(nei.node);
					nei.node->status = AGraphNode<D, W>::Status::ToBeVisited;
				}
			}
			que.pop();
		}
		this->resetVisitStatus();
	}
	template <class D, class W> void ex::AGraph<D, W>::bfs_right_first(
		AGraphNode<D, W> *node, std::function<void(AGraphNode<D, W>*)> func)
	{
		std::queue <AGraphNode<D, W>*> que;
		que.push(node);
		while (!que.empty()){
			node = que.front();
			func(node);
			node->status = AGraphNode<D, W>::Status::Visited;
			for (auto it = node->neighbors.rbegin(); it != node->neighbors.rend(); ++it){
				if (it->node->status == AGraphNode<D, W>::Status::Unvisited){
					que.push(it->node);
					it->node->status = AGraphNode<D, W>::Status::ToBeVisited;
				}
			}
			que.pop();
		}
		this->resetVisitStatus();
	}
	template <class D, class W> void ex::AGraph<D, W>::bfs_interleaved(
		AGraphNode<D, W> *node, std::function<void(AGraphNode<D, W>*)> func, bool leftToRight)
	{
		std::queue <AGraphNode<D, W>*> que;
		que.push(node);
		while (!que.empty()){
			node = que.front();
			func(node);
			node->status = AGraphNode<D, W>::Status::Visited;

			if (!node->neighbors.empty()){
				auto first = node->neighbors.begin();
				auto last = std::prev(node->neighbors.end());
				bool moveRight = leftToRight;
				/*This will reflect as an interleaved iteration. The boolean parameter indicates whether
				to start at the right or left boundary of the container..*/
				while (first != last){
					if (moveRight){
						if (first->node->status == AGraphNode<D, W>::Status::Unvisited){
							que.push(first->node);
							first->node->status = AGraphNode<D, W>::Status::ToBeVisited;
							moveRight = false;
						}
						++first;
					}
					else{
						if (last->node->status == AGraphNode<D, W>::Status::Unvisited){
							que.push(last->node);
							last->node->status = AGraphNode<D, W>::Status::ToBeVisited;
							moveRight = true;
						}
						--last;
					}
				}
				/*After the while loop ends, the midle element in the 'neighbors' list will not
				be added to the queue, so we have to add it, outside the loop.*/
				if (first->node->status == AGraphNode<D, W>::Status::Unvisited){
					que.push(first->node);
					first->node->status = AGraphNode<D, W>::Status::ToBeVisited;
					moveRight = false;
				}

			}
			que.pop();
		}
		this->resetVisitStatus();
	}

	template <class D, class W> void ex::AGraph<D, W>::_dfs_pre_order(
		AGraphNode<D, W> *node, std::function<void(AGraphNode<D, W>*)> func){

		/*In theory this 'if' is not necessary, but we use it as a safety check, anyway.*/
		if (!node || node->status == AGraphNode<D, W>::Status::Visited)
			return;

		func(node);
		node->status = AGraphNode<D, W>::Status::Visited;

		for (Neighbor &nei : node->neighbors){
			if (nei.node->status != AGraphNode<D, W>::Status::Visited)
				_dfs_pre_order(nei.node, func);
		}
	}
	template <class D, class W> void ex::AGraph<D, W>::_dfs_post_order(
		AGraphNode<D, W> *node, std::function<void(AGraphNode<D, W>*)> func){

		/*In theory this 'if' is not necessary, but we use it as a safety check, anyway.*/
		if (!node || node->status == AGraphNode<D, W>::Status::Visited)
			return;

		for (Neighbor &nei : node->neighbors){
			if (nei.node->status == AGraphNode<D, W>::Status::Unvisited){
				nei.node->status = AGraphNode<D, W>::Status::ToBeVisited;
				_dfs_post_order(nei.node, func);
			}
		}

		func(node);
		node->status = AGraphNode<D, W>::Status::Visited;
	}
	
	template <class D, class W> std::list<std::list<AGraphNode<D, W>*>> ex::AGraph<D, W>::getAllPaths(Node *from, Node *to)
	{		
		std::list<std::list<Node*>> allPaths;
		std::list<std::list<Node*>> pathQue;
		
		pathQue.push_back(std::list<Node*>());
		pathQue.front().push_back(from);

		while (!pathQue.empty()){
			std::list<Node*> actualPath = pathQue.front();

			Node *n = actualPath.back();
			if (n == to){
				allPaths.push_back(actualPath);
			}

			for (Neighbor &nei : n->neighbors){
				/*If true, means this node does not yet exists on the current path, so we add it.*/
				if (std::find(actualPath.begin(), actualPath.end(), nei.node) == actualPath.end()){
					std::list<Node*> tempPath(actualPath);
					tempPath.push_back(nei.node);
					
					/*This if check if this path is duplicate, which could happen, at this point, if
					a pair of have two identical connections. This is common thing to happen if the 
					graph is weighted, and two identical connections receive the same weight, for some reason.*/
					if(std::find(pathQue.begin(), pathQue.end(), tempPath) == pathQue.end())
						pathQue.push_back(tempPath);
				}
			}
			pathQue.pop_front();
		}
		return allPaths;
	}

	template <class D, class W> void ex::AGraph<D, W>::_get_all_paths_recursive(
		std::list<Node*> myPath, Node *to, std::list<std::list<Node*>> &allPaths)
	{
		
		Node *thisNode = myPath.back();

		/*Fist call will come with root node inside 'myPath' already. Therefore, in 
		future calls, the 'myPath' list will already hold this currend node.*/
		if (thisNode == to){
			allPaths.push_back(myPath);
		}
		for (Neighbor &nei : thisNode->neighbors){
			if (std::find(myPath.begin(), myPath.end(), nei.node) == myPath.end()){
				myPath.push_back(nei.node);
				
				/*This if check if this path is duplicate, which could happen, at this point, if
				a pair of have two identical connections. This is common thing to happen if the
				graph is weighted, and two identical connections receive the same weight, for some reason.*/
				if (std::find(allPaths.begin(), allPaths.end(), myPath) == allPaths.end())
					_get_all_paths_recursive(myPath, to, allPaths);

				myPath.pop_back();
			}
		}
	}

	template <class D, class W> std::list<std::list<Neighbor<D,W>>> ex::AGraph<D, W>::getAllPathsWithWeights(Node *from, Node *to){
		
		std::list<std::list<Neighbor>> allPaths;
		std::queue<std::list<Neighbor>> pathQue;

		pathQue.push(std::list<Neighbor>());
		pathQue.front().push_back(Neighbor(from,0));

		while (!pathQue.empty()){
			std::list<Neighbor> actualPath = pathQue.front();

			Neighbor n = actualPath.back();
			if (n.node == to){
				allPaths.push_back(actualPath);
			}

			for (Neighbor &nei : n.node->neighbors){
				/*If true, means this node does not yet exists on the current path, so we add it.*/
				if (std::find(actualPath.begin(), actualPath.end(), nei) == actualPath.end()){
					std::list<Neighbor> tempPath(actualPath);
					tempPath.push_back(nei);
					pathQue.push(tempPath);
				}
			}
			pathQue.pop();
		}
		return allPaths;
	}

	/*This represents a vertex*/
	template <class D, class W> struct AGraphNode{
	
		using Neighbor = ex::Neighbor<D, W>;
		using Node = AGraphNode<D, W>;

		/*ToBeVisited means the node has already been queued for the visit.*/
		enum Status{ Visited, Unvisited, ToBeVisited };

		AGraphNode(D data) : data(data) {}
		~AGraphNode(){
			for (Node *par : parents){
				par->removeConnection(this);
			}
			for (Neighbor &nei : neighbors){
				this->removeConnection(nei);
			}
		}
		
		void addConnection(Node *node, W weight = W()){
			addConnection(Neighbor(node, weight));
		}
		void addConnection(Neighbor neighbor){
			/*First we test for duplicate elements. If it is not a duplicate, we add it to the list.
			Note that we consider both the node and weight for checking it. This means there may be 
			two or more connections from A to B, if they have different weights.*/
			if (std::find(neighbors.begin(), neighbors.end(), neighbor) == neighbors.end())
			{
				neighbors.push_back(neighbor);
				neighbor.node->parents.push_back(this);
			}
		}

		/*This method removes all neighbors composed by the node passed as parameter. In other words
		if there are three diferent neighbors containing this same 'node', but with different weights
		all three are removed anyway.*/
		void removeConnection(Node *node){
			
			auto pos = std::remove_if(neighbors.begin(), neighbors.end(), [&](Neighbor &nei){
				return nei.node == node; 
			});
			
			for (auto it = pos; it != neighbors.end(); ++it){
				it->node->parents.remove(this);
			}
			neighbors.erase(pos, neighbors.end());
		}

		/*This method removes the exact neighbor of this node, that is, we use both the
		node pointer and the weigth to find the Neighbor and remove it.*/
		void removeConnection(const Neighbor &neighbor){
			auto nodePos = std::find(neighbors.begin(), neighbors.end(), neighbor);
			/*If node exists as a connection*/
			if (nodePos != neighbors.end()){
				neighbor.node->parents.remove(this);
				neighbors.erase(nodePos);
			}
		}

		/*Visiting status of this node. Used for traversal algorithms.*/
		Status status = Status::Unvisited;

		/*Soem data this node element will hold*/
		D data;

		/*Nodes that this one is connected to. Since this actually represents
		a connect (an edge), we use the class 'Neighbor' which encapsulates 
		a pointer to a 'AGraphNode' as well as the weight of the edge.*/
		std::list<Neighbor> neighbors;
		
		/*Nodes that connect to this one.*/
		std::list<Node*> parents;
	};
	
	template <class D, class W> std::ostream &operator<<(std::ostream &out, const AGraphNode<D, W> *&node){
		out << node->data;
		return out;
	}
	template <class D, class W> std::ostream &operator<<(std::ostream &out, const AGraphNode<D, W> &node){
		out << node.data;
		return out;
	}

	/*This tests equivalency, instead of identity.*/
	template <class D, class W> bool operator==(const AGraphNode<D, W> &lhs, const AGraphNode<D, W> &rhs){
		return lhs.data == rhs.data &&
			lhs.status == rhs.status &&
			lhs.parents == rhs.parents &&
			lhs.neighbors == rhs.neighbors;
	}
	

	/*This represents an edge*/
	template <class D, class W> struct Neighbor{
		Neighbor(AGraphNode<D, W> *node, W weight) : node(node), weight(weight){}
		AGraphNode<D, W> *node = nullptr;
		W weight;
	};
	template <class D, class W> bool operator==(const Neighbor<D, W> &lhs, const Neighbor<D, W> &rhs){
		return lhs.node == rhs.node && lhs.weight == rhs.weight;
	}
	/*template <class D, class W> bool operator==(Neighbor<D, W> *lhs, Neighbor<D, W> *rhs){
		return lhs->node == rhs->node && lhs->weight == rhs->weight;
	}*/
	
	/*This function may also be used to check if there exists a path between two nodes. If the return
	from the first call is a nullptr, there is not a path.*/
	template <class D, class W> AGraphNode<D, W> *find_node_dfs(AGraphNode<D, W> *root, D data){
		
		if (!root)
			return nullptr;

		root->status = AGraphNode<D, W>::Status::Visited;

		/*If node is found, no need to keep searching deeper, so we return it.*/
		if (root->data == data){
			return root;
		}
		else{	
			for (Neighbor<D, W> &nei : root->neighbors){
				if (nei.node->status != AGraphNode<D, W>::Status::Visited){
					AGraphNode<D, W> *n = find_node_dfs(nei.node, data);
					/*If we found it, we break the for, and stop searching*/
					if (n && n->data == data){
						return n;
					}
				}
			}
			/*If the above 'for' was left, this means the node was not found, so we return nullptr*/
			return nullptr;
		}
	}

};




#endif