/*
 * Rule.cpp
 *
 *  Created on: Jul 5, 2014
 *      Author: davidsoa
 */

#include "Rule.h"
#include "Predicate.h"
#include <vector>
#include <sstream>

using namespace std;

Rule::Rule(Predicate ruleHead, vector<Predicate> & ruleBody) {
	head = ruleHead;
	body = ruleBody;
}

Rule::Rule() {
	// Does nothing.
}

Rule::~Rule() {
	// TODO Auto-generated destructor stub
}

Predicate Rule::getRuleHead() {
	return head;
}

vector<Predicate> Rule::getRuleBody() {
	return body;
}

string Rule::toString() {

	ostringstream rslt;

	rslt << head.toString() << " :- ";
	for (int i = 0; i < body.size(); i++) {
		rslt << body[i].toString();
		if (i != body.size() - 1)
			rslt << ",";
	}
	string s = rslt.str();
	return s;
}

