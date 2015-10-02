/*
 * Graph.cpp
 *
 *  Created on: Aug 5, 2014
 *      Author: davidsoa
 */

#include "Graph.h"

using namespace std;
#include <sstream>
#include <iostream>

Graph::Graph() {
	nodes = map<string, Node>();
	reset();
}

Graph::~Graph() {
	// TODO Auto-generated destructor stub
}

void Graph::reset() {
	hasBackwardsEdge = false;
	postOrderIncrementer = 0;
	for (auto & p : nodes) {
		p.second.reset();
	}
}
bool Graph::containsBackEdge() {
	return hasBackwardsEdge;
}
int Graph::getNextPONum() {
	postOrderIncrementer++;
	return postOrderIncrementer;
}

void Graph::addNode(string & nodeName, Node & node) {
	nodes[nodeName] = node;
}

void Graph::depthFirstSearch(string & nodeName) {
	Node & currentNode = nodes[nodeName];
	if (currentNode.isMarked())
		return;
	currentNode.mark();
	for (string adjNodeName : currentNode.getAdjacencySet()) {
		depthFirstSearch(adjNodeName);
	}
	currentNode.setPONum(getNextPONum());
}

string Graph::toDependencyGraphString() {
	ostringstream rslt;
	rslt << "Dependency Graph\n";
	for (pair<string, Node> p : nodes) {
		rslt << "  " << p.first << ":" << p.second.toString() << "\n";
	}
	rslt << "\n";
	return rslt.str();
}

void Graph::toPONString(map<int, int> & ruleOrder, string & postOrderString,
		string & backEdgesString) {
	//TODO handle pairs in the set of backwards edges which contain multiple strings that match.

	ostringstream postOrderRslt;
	ostringstream backEdgeRslt;
	map<string, string> backwardsEdges = map<string, string>();
	postOrderRslt << "  Postorder Numbers\n";
	backEdgeRslt << "  Backward Edges\n";

	for (pair<string, Node> p : nodes) {
		if (p.second.isMarked()) {
			postOrderRslt << "    " << p.first << ": " << p.second.getPONum()
					<< "\n";
			for (string adjNode : p.second.getAdjacencySet()) {
				if (nodes[adjNode].getPONum() >= p.second.getPONum()) {
					if (backwardsEdges.count(p.first)) {
						ostringstream mergeStream;
						mergeStream << backwardsEdges[p.first];
						mergeStream << " "<<adjNode;
						backwardsEdges[p.first] = mergeStream.str();
					} else {
						backwardsEdges[p.first] = adjNode;
					}
				}
			}
			int postNumb = p.second.getPONum();
			int index = p.second.getRuleIdx();
			ruleOrder[postNumb] = index;
		}
	}

	for (auto p : backwardsEdges) {
		backEdgeRslt << "    " << p.first << ": " << p.second << "\n";
	}

	if (backwardsEdges.size() > 0) {
		hasBackwardsEdge = true;
	}

	postOrderRslt << "\n";
	backEdgeRslt << "\n";
	postOrderString = postOrderRslt.str();
	backEdgesString = backEdgeRslt.str();

}

