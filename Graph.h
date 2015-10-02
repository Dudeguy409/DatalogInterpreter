/*
 * Graph.h
 *
 *  Created on: Aug 5, 2014
 *      Author: davidsoa
 */

#ifndef GRAPH_H_
#define GRAPH_H_

using namespace std;

#include "Node.h"
#include <string>
#include <map>

class Graph {
public:
	Graph();
	virtual ~Graph();
	void reset();
	bool containsBackEdge();
	int getNextPONum();
	void addNode(string & nodeName, Node & node);
	string toDependencyGraphString();
	void toPONString(map<int, int> & ruleOrder, string & postOrderString,
			string & backEdgesString);
	void depthFirstSearch(string & nodeName);
private:
	map<string, Node> nodes;
	int postOrderIncrementer;
	bool hasBackwardsEdge;
};

#endif /* GRAPH_H_ */
