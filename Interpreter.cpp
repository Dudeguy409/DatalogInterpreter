/*
 * Interpreter.cpp
 *
 *  Created on: Jul 22, 2014
 *      Author: davidsoa
 */

using namespace std;
#include "Interpreter.h"
#include <iostream>
#include <map>
#include <sstream>
#include "Node.h"

Interpreter::Interpreter() {
	// TODO Auto-generated constructor stub
}

Interpreter::~Interpreter() {
	// TODO Auto-generated destructor stub
}

Interpreter::Interpreter(DatalogProgram & data) {
	database = Database();
	rawData = data;
	ruleGraph = Graph();

	processSchemes();

	processFacts();

	result = processRules();
}

void Interpreter::processSchemes() {
	vector<Predicate> schemes = rawData.getSchemes();

	for (Predicate scheme : schemes) {
		string tempName = scheme.getName();
		vector<Parameter> schemeParams = scheme.getParamList();
		Scheme schemeToAdd = buildScheme(schemeParams);
		Relation relation = Relation(tempName, schemeToAdd);
		database.addRelation(tempName, relation);
	}
}

void Interpreter::processFacts() {
	vector<Predicate> facts = rawData.getFacts();

	for (Predicate fact : facts) {
		vector<Parameter> factParams = fact.getParamList();
		Tuple tuple = buildTuple(factParams);
		string name = fact.getName();
		database.addTuple(name, tuple);
	}
}

string Interpreter::processRules() {
	ostringstream rslt;
	vector<Rule> rules = rawData.getRules();
	vector<Predicate> queries = rawData.getQueries();

	buildGraph(rules, queries);

	rslt << ruleGraph.toDependencyGraphString();

	for (int i = 0; i < queries.size(); i++) {
		ostringstream queryName;
		queryName << "Q" << i + 1;
		string queryNodeName = queryName.str();
		rslt << queries[i].toString() << "?\n\n";
		ruleGraph.reset();
		ruleGraph.depthFirstSearch(queryNodeName);
		map<int, int> ruleOrder = map<int, int>();
		string postOrderString;
		string backEdgesString;

		ruleGraph.toPONString(ruleOrder, postOrderString, backEdgesString);
		rslt << postOrderString;
		vector<Rule> rulesToExecute = vector<Rule>();
		ostringstream ruleEvalOrderRslt;
		ruleEvalOrderRslt << "  Rule Evaluation Order\n";
		for (auto p : ruleOrder) {
			if (p.second >= 0) {
				rulesToExecute.push_back(rules[p.second]);
				ruleEvalOrderRslt << "    R" << p.second + 1 << "\n";
			}
		}

		rslt << ruleEvalOrderRslt.str() << "\n" << backEdgesString;

		int numOfCycles = processQueryRules(rulesToExecute,
				ruleGraph.containsBackEdge());
		rslt << "  Rule Evaluation\n";
		for (int j = 0; j < numOfCycles; j++) {
			for (Rule rule : rulesToExecute) {
				rslt << "    " << rule.toString() << "\n";
			}
		}
		rslt << "\n" << processQuery(queries[i]) << "\n";
	}

	return rslt.str();

}

void Interpreter::buildGraph(vector<Rule> & rules,
		vector<Predicate> & queries) {
	for (int i = 0; i < rules.size(); i++) {
		Node headNode = Node();
		headNode.setRuleIdx(i);
		ostringstream nodeName;
		nodeName << "R" << i + 1;
		for (Predicate pred : rules[i].getRuleBody()) {
			for (int j = 0; j < rules.size(); j++) {
				if (pred.getName() == rules[j].getRuleHead().getName()) {
					ostringstream adjacentNodeName;
					adjacentNodeName << "R" << j + 1;
					string adjNodeName = adjacentNodeName.str();
					headNode.addAdjacentNode(adjNodeName);
				}
			}
		}
		string headNodeName = nodeName.str();
		ruleGraph.addNode(headNodeName, headNode);
	}

	for (int i = 0; i < queries.size(); i++) {
		Node queryNode = Node();
		ostringstream queryName;
		queryName << "Q" << i + 1;
		for (int j = 0; j < rules.size(); j++) {
			if (queries[i].getName() == rules[j].getRuleHead().getName()) {
				ostringstream adjacentNodeName;
				adjacentNodeName << "R" << j + 1;
				string adjNodeName = adjacentNodeName.str();
				queryNode.addAdjacentNode(adjNodeName);
			}
		}
		string queryNodeName = queryName.str();
		ruleGraph.addNode(queryNodeName, queryNode);
	}
}

int Interpreter::processQueryRules(vector<Rule> & rules, bool hasBackEdge) {
	int numOfPasses = 0;
	bool dbChangedSize;
	do {
		dbChangedSize = false;
		for (int i = 0; i < rules.size(); i++) {
			bool relationChangedSize = !processRule(rules[i]);
			dbChangedSize = dbChangedSize || relationChangedSize;
		}
		numOfPasses++;
	} while (dbChangedSize && hasBackEdge);
	return numOfPasses;
}

bool Interpreter::processRule(Rule & rule) {

	vector<vector<string>> bodySchemeIDs = vector<vector<string>>();
	vector<string> bodySchemeStrings = vector<string>();

	Predicate ruleHead = rule.getRuleHead();
	string headName = ruleHead.getName();
	Relation headRelation = database.getRelation(headName);
	vector<Parameter> headParams = ruleHead.getParamList();
//	if (headRelation.getScheme().size() != headParams.size()) {
//		throw "The schemes for one of the rules' heads didn't match.";
//	}

	vector<string> schemeParams = headRelation.getScheme().getAttributes();
	vector<string> headIDs = vector<string>();
	map<string, int> matchingParams = map<string, int>();
	for (int i = 0; i < headParams.size(); i++) {
		matchingParams[headParams[i].getValue()] = -1;
		if (!headParams[i].isID()) {
			throw "In a certain rule, at least one of the parameters was a string instead of an ID.";
		}
		headIDs.push_back(headParams[i].getValue());
	}
	vector<vector<int>> stringPositions = vector<vector<int>>();
	extractRule(rule, bodySchemeIDs, bodySchemeStrings, matchingParams,
			stringPositions);

	Relation intermediateRelation = database.getRelation(bodySchemeStrings[0]);
// renames the columns in the copy of the original relation to the names of the columns in the rule.
	for (int j = 0; j < bodySchemeIDs[0].size(); j++) {
		intermediateRelation = intermediateRelation.rename(j,
				bodySchemeIDs[0][j]);
	}

	for (int i : stringPositions[0]) {
		intermediateRelation = intermediateRelation.select(i,
				bodySchemeIDs[0][i]);
	}

//this loop goes through the list of schemes in the body of the rule and adds each one's parameters and tuple columns to the growing intermediate relation, which eventually contains the join of all the schemes.
	for (int i = 0; i < bodySchemeIDs.size() - 1; i++) {
		vector<string> intermediateIDs =
				intermediateRelation.getScheme().getAttributes();
		vector<int> positionsToJoin = configureJoin(intermediateIDs,
				bodySchemeIDs[i + 1]);
		Relation relationToJoin = database.getRelation(
				bodySchemeStrings[i + 1]);

		// Before joining them, the columns in the copy of the original relation are renamed to the names of the columns given in the rule.
		for (int j = 0; j < bodySchemeIDs[i + 1].size(); j++) {
			relationToJoin = relationToJoin.rename(j, bodySchemeIDs[i + 1][j]);
		}

		for (int j : stringPositions[i + 1]) {
			relationToJoin = relationToJoin.select(j, bodySchemeIDs[i + 1][j]);
		}

		intermediateRelation = intermediateRelation.join(relationToJoin,
				positionsToJoin);
	}

	int relationSizeBefore = headRelation.size();
	headRelation = headRelation.unionRelations(intermediateRelation, headIDs);
	int relationSizeAfter = headRelation.size();
	bool stayedSameSize = (relationSizeBefore == relationSizeAfter);

	database.replaceRelation(headRelation);

	return stayedSameSize;

	return true;

}

void Interpreter::extractRule(Rule & rule,
		vector<vector<string>> & bodySchemeIDs,
		vector<string> & bodySchemeStrings, map<string, int> & matchingParams,
		vector<vector<int>> & stringPositions) {

	vector<Predicate> ruleBody = rule.getRuleBody();
	for (int i = 0; i < ruleBody.size(); i++) {
		bodySchemeIDs.push_back(vector<string>());
		stringPositions.push_back(vector<int>());
		string bodyPredName = ruleBody[i].getName();
		bodySchemeStrings.push_back(bodyPredName);
		Relation tempRelation = database.getRelation(bodyPredName);
		if (tempRelation.getScheme().size()
				!= ruleBody[i].getParamList().size()) {
			throw "One of the scheme in one of the rules' bodies didn't match.";
		}

		vector<Parameter> paramList = ruleBody[i].getParamList();
		for (int j = 0; j < paramList.size(); j++) {
			Parameter p = paramList[j];
			if (!p.isID()) {
				stringPositions[i].push_back(j);
			}
			if (matchingParams.count(p.getValue()) != 0) {
				matchingParams[p.getValue()] = 1;
			}
			bodySchemeIDs[i].push_back(p.getValue());
		}
	}

	for (pair<string, int> p : matchingParams) {
		if (p.second != 1) {
			throw "For a certain rule, not all of the parameters in the head were able to be matched with parameters in the body.";
		}
	}

}

/**
 *  compares the IDs in both relations and returns a list of
 *  those that match and which can therefore be joined.
 *
 * @param bodyScheme1IDs: a vector containing strings
 * 		  representing the IDs of the first relation.
 * @param bodyScheme2IDs: a vector containing strings
 * 		  representing the IDs of the second relation.
 *
 * @returns a vector containing a list of integers of the positions in
 * 			the relations to join.  The index of the integer represents
 * 			the position in the first relation and the integer represents
 * 			the position in the second relation to which to join.
 **/
vector<int> Interpreter::configureJoin(vector<string> bodyScheme1IDs,
		vector<string> bodyScheme2IDs) {
	vector<int> positionsToJoin = vector<int>();
	for (int i = 0; i < bodyScheme1IDs.size(); i++) {
		positionsToJoin.push_back(-1);
		for (int j = 0; j < bodyScheme2IDs.size(); j++) {
			if (bodyScheme1IDs[i] == bodyScheme2IDs[j]) {
				positionsToJoin[i] = j;
			}
		}
	}
	return positionsToJoin;

}

string Interpreter::processQuery(Predicate & query) {
	ostringstream rslt;
	string name = query.getName();
	Relation destRelation = database.getRelation(name);
	vector<Parameter> queryParams = query.getParamList();
	map<string, vector<int>> identMap = map<string, vector<int>>();
	vector<string> orderedIDs = vector<string>();

	destRelation = sortQuery(queryParams, identMap, destRelation, orderedIDs);

	destRelation = handleQueryMatchSelect(destRelation, identMap);
	rslt << name << "(" << destRelation.getScheme().toString() << ")? ";
	destRelation = handleQueryProjection(destRelation, identMap, orderedIDs);

	rslt << destRelation.toQueryResultString();
	return rslt.str();
}

Relation Interpreter::sortQuery(vector<Parameter> & queryParams,
		map<string, vector<int>> & identMap, Relation & relation,
		vector<string> & orderedIDs) {
	if (queryParams.size() != relation.getScheme().size()) {
		throw "one of the queries did not have the correct number of parameters.";
	}

	for (int i = 0; i < queryParams.size(); i++) {
		string value = queryParams[i].getValue();
		if (queryParams[i].isID()) {
			relation = relation.rename(i, queryParams[i].getValue());
			//if pair doesn't exist, creates a pair with new vector
			if (identMap.count(value) == 0) {
				orderedIDs.push_back(value);
				identMap[value] = vector<int>();
			}
			identMap[value].push_back(i);
		} else {
			ostringstream ss;
			ss << "'" << queryParams[i].getValue() << "'";
			relation = relation.rename(i, ss.str());
			relation = relation.select(i, value);
		}
	}
	return relation;
}

Relation Interpreter::handleQueryMatchSelect(Relation & destRelation,
		map<string, vector<int>> & identMap) {
	for (pair<string, vector<int>> p : identMap) {
		if (p.second.size() > 1) {
			destRelation = destRelation.select(p.second);
		}
	}
	return destRelation;
}

Relation Interpreter::handleQueryProjection(Relation & destRelation,
		map<string, vector<int>> & identMap, vector<string> orderedIDs) {

	vector<int> positionsToProject = vector<int>();
	for (int i = 0; i < destRelation.getScheme().size(); i++) {
		positionsToProject.push_back(-1);
	}

	int varCount = 0;
	for (string id : orderedIDs) {
		positionsToProject[identMap[id][0]] = varCount;
		varCount++;
	}

	return destRelation.project(positionsToProject, varCount);
}

Scheme Interpreter::buildScheme(vector<Parameter> & predParams) {
	vector<string> attributes = vector<string>();
	for (int i = 0; i < predParams.size(); i++) {
		if (!predParams[i].isID())
			throw "Error building scheme:  There can be only identifiers in a scheme.";
		attributes.push_back(predParams[i].getValue());
	}
	return Scheme(attributes);
}

Tuple Interpreter::buildTuple(vector<Parameter> & predParams) {
	Tuple tuple = Tuple();

	for (int i = 0; i < predParams.size(); i++) {
		if (predParams[i].isID()) {
			throw "Error inserting one of the facts in the database:  There can be no identifiers in a scheme.";
		}
		tuple.push_back(predParams[i].getValue());
	}
	return tuple;
}

string Interpreter::getResult() {
	return result;
}

