/*
 * Node.cpp
 *
 *  Created on: Aug 5, 2014
 *      Author: davidsoa
 */

using namespace std;
#include "Node.h"
#include <sstream>

Node::Node() {
	adjacentNodes = set<string>();
	ruleIndex = -1;
	reset();
}

Node::~Node() {
	// TODO Auto-generated destructor stub
}

int Node::getPONum() {
	return postOrderNumber;
}
void Node::setPONum(int postOrderNum) {
	postOrderNumber = postOrderNum;
}
bool Node::isMarked() {
	return marked;
}
void Node::mark() {
	marked = true;
}
void Node::reset() {
	marked = false;
	postOrderNumber = -1;
}

void Node::addAdjacentNode(string & nodeName) {
	adjacentNodes.insert(nodeName);
}

set<string> Node::getAdjacencySet() {
	return adjacentNodes;
}

void Node::setRuleIdx(int index) {
	ruleIndex = index;
}
int Node::getRuleIdx() {
	return ruleIndex;
}

string Node::toString() {
	ostringstream rslt;
	for (string adjNode : adjacentNodes) {
		rslt << " " << adjNode;
	}

	return rslt.str();

}

