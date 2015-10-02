/*
 * Node.h
 *
 *  Created on: Aug 5, 2014
 *      Author: davidsoa
 */

#ifndef NODE_H_
#define NODE_H_

using namespace std;

#include <string>
#include <set>

class Node {
public:
	Node();
	virtual ~Node();
	int getPONum();
	void setPONum(int postOrderNum);
	bool isMarked();
	void mark();
	void reset();
	void addAdjacentNode(string & nodeName);
	string toString();
	set<string> getAdjacencySet();
	void setRuleIdx(int index);
	int getRuleIdx();
private:
	set<string> adjacentNodes;
	int postOrderNumber;
	bool marked;
	int ruleIndex;
};

#endif /* NODE_H_ */
