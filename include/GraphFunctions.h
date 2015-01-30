#ifndef GRAPH_FUNCTIONS_H
#define GRAPH_FUNCTIONS_H

#include "DirectedGraph.h"
#include <vector>
#include <functional> 
#include <stack>
#include <queue> 
#include <list>

namespace ex{

	template <class T> void printAllPaths(GraphNode<T> *src, GraphNode<T> *end){
		std::list<std::list<GraphNode<T>*>> paths;
		ex::getAllPaths(src, end, paths);

		if (paths.empty()){
			std::cout << "No paths found" << endl;
		}
		else{
			int i = 0;
			for (auto &path : paths){
				std::cout << "Path " << i << ": ";
				for (auto *node : path){
					std::cout << node->value << " ";
				}
				cout << endl;
				++i;
			}
			
			std::cout << endl << "Shortest path " << endl;
			auto shortestPath = getShortestPath(src, end);
			for (auto *node : shortestPath){
				std::cout << node->value << " ";
			}
		}
	}

	template <class T> std::list<GraphNode<T>*> getShortestPath(GraphNode<T> *src, GraphNode<T> *end){
		std::list<std::list<GraphNode<T>*>> paths;
		ex::getAllPaths(src, end, paths);

		if (paths.empty()){
			return std::list<GraphNode<T>*>();
		}
		else{
			paths.sort([](std::list<ex::GraphNode<T>*> &a, std::list<ex::GraphNode<T>*> &b){
				return a.size() < b.size();
			});
			return paths.front();
		}
	}

	/*Get all paths from 'src' to 'end'*/
	template <class T> void getAllPaths(GraphNode<T> *src, GraphNode<T> *end, std::list<std::list<GraphNode<T>*>> &paths){
		
		/*For path tracking, instead of just queueing the node, we queue the paths.*/
		std::queue<std::list<GraphNode<T>*>> pathQue;

		pathQue.push(std::list<GraphNode<T>*>());
		pathQue.front().push_back(src);

		while (!pathQue.empty()){
			std::list<GraphNode<T>*> &actualPath = pathQue.front();
			for (GraphNode<T> *n : actualPath.back()->getConnections()){
				/*This 'if' testes for circular paths. It avoids entering 
				a node self-connection and cycles in the graph.*/
				if (std::find(actualPath.begin(), actualPath.end(), n) == actualPath.end())
				{
					/*Fork new path.*/
					std::list<GraphNode<T>*> temp(actualPath);
					/*Adds actual node to the forked path*/
					temp.push_back(n);

					/*Enqueues the path, so it can continuing being traversed later.
					This is a natural step of the BFS algorithm.*/
					pathQue.push(temp);
					
					/*Tests if we found the desired node. If so, we record the path.*/
					if (n == end){
						paths.push_back(temp);
					}
				}
			}
			pathQue.pop();
		}

	}

	template <class T> void testRoute (int src, int dst, ex::DiGraph<T> &digraph, std::vector<GraphNode<T>*> &nodes)
	{
		cout << "Is there a route from [" << src << "] to [" << dst << "]? " << boolalpha << digraph.hasRoute(nodes[src], nodes[dst]) << endl;
	}

	template <class T> void bfs(GraphNode<T> *root, std::function<void(GraphNode<T>*)> func){
		std::queue<GraphNode<T>*> que;
		que.push(root);
		while (!que.empty()){
			GraphNode<T> *node = que.front();
			if (node->status == ex::NodeStatus::Unvisited){
				func(node);
				node->status = ex::NodeStatus::Visited;
			}
			std::list<GraphNode<T>*> &nodes = node->getConnections();
			for (const auto &n : nodes){
				if (n->status == ex::NodeStatus::Unvisited)
					que.push(n);
			}
			que.pop();
		}

	}

	/*rec = recursive*/
	template <class T> void dfsPreOrder(GraphNode<T> *root, std::function<void(GraphNode<T>*)> func){
		std::stack<GraphNode<T>*> st;
		st.push(root);

		while (!st.empty()){
			GraphNode<T> *node = st.top();
			st.pop();
			std::list<GraphNode<T>*> &nodes = node->getConnections();
			for (auto it = nodes.rbegin(); it != nodes.rend(); ++it){
				if ((*it)->status == ex::NodeStatus::Unvisited){
					st.push(*it);
				}
			}
			if (node->status == ex::NodeStatus::Unvisited){
				func(node);
				node->status = ex::NodeStatus::Visited;
			}
		}
	}

	template <class T> void dfsRecPreOrder(GraphNode<T> *node, std::function<void(GraphNode<T>*)> func){

		if (!node || node->status == ex::NodeStatus::Visited)
			return;

		func(node);
		node->status = ex::NodeStatus::Visited;

		std::list<GraphNode<T>*> &nodes = node->getConnections();
		for (const auto &n : nodes){
			if (n->status == ex::NodeStatus::Unvisited)
				dfsRecPreOrder(n, func);
		}
	}
	template <class T> void dfsRecPostOrder(GraphNode<T> *node, std::function<void(GraphNode<T>*)> func){

		if (!node || node->status == ex::NodeStatus::Visited)
			return;

		
		node->status = ex::NodeStatus::Visited;
		std::list<GraphNode<T>*> &nodes = node->getConnections();
		for (const auto &n : nodes){
			if (n->status == ex::NodeStatus::Unvisited)
				dfsRecPostOrder(n, func);
		}
		func(node);
	}

};

#endif