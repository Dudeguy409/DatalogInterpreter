/*
 * DatalogProgram.h
 *
 *  Created on: Jul 5, 2014
 *      Author: davidsoa
 */

#ifndef DATALOGPROGRAM_H_
#define DATALOGPROGRAM_H_

using namespace std;
#include <string>
#include "Token.h"
#include "Predicate.h"
#include "Rule.h"
#include <vector>
#include <set>

class DatalogProgram {
public:
	DatalogProgram(vector<Predicate> & schemeList, vector<Predicate> & factList,
			vector<Rule> & ruleList, vector<Predicate> & queryList,
			set<string> & domainStrings);
	DatalogProgram();
	virtual ~DatalogProgram();
	virtual string toString();
	vector<Predicate> getSchemes();
	vector<Predicate> getFacts();
	vector<Rule> getRules();
	vector<Predicate> getQueries();
private:
	vector<Predicate> schemes;
	vector<Predicate> facts;
	vector<Rule> rules;
	vector<Predicate> queries;
	set<string> domain;
};

#endif /* DATALOGPROGRAM_H_ */
