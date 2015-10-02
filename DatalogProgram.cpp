/*
 * DatalogProgram.cpp
 *
 *  Created on: Jul 5, 2014
 *      Author: davidsoa
 */

#include "DatalogProgram.h"
#include "Predicate.h"
#include "Rule.h"
#include <sstream>

using namespace std;

DatalogProgram::DatalogProgram(vector<Predicate> &schemeList,
		vector<Predicate> & factList, vector<Rule> & ruleList,
		vector<Predicate> & queryList, set<string> & domainStrings) {
	schemes = schemeList;
	facts = factList;
	rules = ruleList;
	queries = queryList;
	domain = domainStrings;

}

DatalogProgram::DatalogProgram() {

}

DatalogProgram::~DatalogProgram() {
	// TODO Auto-generated destructor stub
}

string DatalogProgram::toString() {

	ostringstream rslt;

	rslt << "Schemes(" << schemes.size() << "):\n";
	for (int i = 0; i < schemes.size(); i++)
		rslt << "  " << schemes[i].toString() << "\n";

	rslt << "Facts(" << facts.size() << "):\n";
	for (int i = 0; i < facts.size(); i++)
		rslt << "  " << facts[i].toString() << "\n";

	rslt << "Rules(" << rules.size() << "):\n";
	for (int i = 0; i < rules.size(); i++)
		rslt << "  " << rules[i].toString() << "\n";

	rslt << "Queries(" << queries.size() << "):\n";
	for (int i = 0; i < queries.size(); i++)
		rslt << "  " << queries[i].toString() << "\n";

	rslt << "Domain(" << domain.size() << "):\n";
	for (set<string>::iterator it = domain.begin(); it != domain.end(); ++it)
		rslt << "  '" << *it << "'\n";

	string s = rslt.str();
	return s;
}

vector<Predicate> DatalogProgram::getSchemes() {
	return schemes;
}

vector<Predicate> DatalogProgram::getFacts() {
	return facts;
}

vector<Rule> DatalogProgram::getRules() {
	return rules;
}

vector<Predicate> DatalogProgram::getQueries() {
	return queries;
}

