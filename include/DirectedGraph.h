#ifndef DIRECTED_GRAPH_H
#define DIRECTED_GRAPH_H

#include <list>
#include <cstddef>
#include <cstdint>

namespace ex{

	

	template <class T> class DiGraph;

	enum class NodeStatus : std::uint8_t{
		Visiting,
		Visited,
		Unvisited
	};

	template <class T> class GraphNode{

	public:

		GraphNode() = default;
		GraphNode(const std::list<GraphNode<T>*> &nodes, T value = 0) : GraphNode<T>(value)
		{
			this->nodes.assign(nodes.begin(), nodes.end());
		}
		GraphNode(T value) : value(value){}

		bool hasConnections(){ return !nodes.empty(); }
		std::list<GraphNode<T>*> &getConnections(){ return nodes; }

		T value = 0;
		NodeStatus status = NodeStatus::Unvisited;

	private:
		void addConnection(GraphNode<T> *node){ nodes.push_back(node); }
		std::list<GraphNode<T>*> nodes;

		friend class DiGraph<T>;
	};

	template <class T> class DiGraph {

	private:
		/*All nodes in the graph*/
		std::list<GraphNode<T>*> allNodes;

	public:
		bool hasNode(GraphNode<T> *node){
			return !allNodes.empty() && std::find(allNodes.begin(), allNodes.end(), node) != allNodes.end();
		}
		void addNode(GraphNode<T> *node){
			if (!hasNode(node))
				allNodes.push_back(node);
		}
		/*If nodes do not exist, they are created.*/
		void addConnection(GraphNode<T> *srcNode, GraphNode<T> *dstNode){
			this->addNode(srcNode);
			this->addNode(dstNode);
			srcNode->addConnection(dstNode);
		}

		void resetNodes(){
			for(auto n : allNodes)
				n->status = NodeStatus::Unvisited;
		}

		std::list<GraphNode<T>*> *getAllNodes(){ return &allNodes; }

		/*4.2 Given a directed graph, design an algorithm to find out whether there is a route
		between two nodes.*/
		bool hasRoute(GraphNode<T> *src, GraphNode<T> *dst){
			if (!hasNode(src) || !hasNode(dst))
				return false;

			bool foundPath = false;
			goToUntilDst(src, dst, foundPath);
			return foundPath;
		}

		/*Travels to the next node until dst is found or end of graph is reached*/
		void goToUntilDst(GraphNode<T> *src, GraphNode<T> *dst, bool &foundPath){
			if (foundPath || !src->hasConnections())
				return;

			std::list<GraphNode<T>*> &nodes = src->getConnections();
			for (auto n : nodes){
				if (n == dst)
					foundPath = true;
				/*This avoids entering an infinit loop and overflowing the stack, which would
				happen if a node is connected to itself (circular connection)*/
				else if (src != n)
					goToUntilDst(n, dst, foundPath);
			}
		}
	};

};

#endif