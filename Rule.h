/*
 * Rule.h
 *
 *  Created on: Jul 5, 2014
 *      Author: davidsoa
 */

#ifndef RULE_H_
#define RULE_H_

using namespace std;

#include "Predicate.h"
#include <vector>
#include <string>

class Rule {
public:
	Rule(Predicate ruleHead, vector<Predicate> & ruleBody);
	Rule();
	virtual ~Rule();
	virtual string toString();
	Predicate getRuleHead();
	vector<Predicate> getRuleBody();
private:
	vector<Predicate> body;
	Predicate head;
};

#endif /* RULE_H_ */
