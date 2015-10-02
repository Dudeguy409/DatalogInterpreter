/*
 * Interpreter.h
 *
 *  Created on: Jul 22, 2014
 *      Author: davidsoa
 */

#ifndef INTERPRETER_H_
#define INTERPRETER_H_

using namespace std;
#include <string>
#include "DatalogProgram.h"
#include "Scheme.h"
#include "Database.h"
#include "Tuple.h"
#include "Rule.h"
#include "Graph.h"

class Interpreter {
public:
	Interpreter();
	Interpreter(DatalogProgram & data);
	virtual ~Interpreter();
	string getResult();
private:
	string result;
	Database database;
	DatalogProgram rawData;
	Graph ruleGraph;
	void processSchemes();
	void processFacts();
	string processRules();
	int processQueryRules(vector<Rule> & rules, bool hasBackEdge);
	void buildGraph(vector<Rule> & rules, vector<Predicate> & queries);
	Scheme buildScheme(vector<Parameter> & predParams);
	Tuple buildTuple(vector<Parameter> & predParams);
	string processQuery(Predicate & query);
	Relation sortQuery(vector<Parameter> & queryParams,
			map<string, vector<int>> & identMap, Relation & relation,
			vector<string> & orderedIDs);
	Relation handleQueryProjection(Relation & destRelation,
			map<string, vector<int>> & identMap, vector<string> orderedIDs);
	Relation handleQueryMatchSelect(Relation & destRelation,
			map<string, vector<int>> & identMap);
	bool processRule(Rule & rule);
	vector<int> configureJoin(vector<string> bodyScheme1IDs,
			vector<string> bodyScheme2IDs);
	void extractRule(Rule & rule, vector<vector<string>> & bodySchemeIDs,
			vector<string> & bodySchemeStrings,
			map<string, int> & matchingParams,
			vector<vector<int>> & stringPositions);
};

#endif /* INTERPRETER_H_ */
