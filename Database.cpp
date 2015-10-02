/*
 * Database.cpp
 *
 *  Created on: Jul 22, 2014
 *      Author: davidsoa
 */

using namespace std;
#include "Database.h"
#include "DatalogProgram.h"
#include "Relation.h"
#include <sstream>
#include <iostream>

Database::Database() {
	relations = map<string, Relation>();
}

Database::~Database() {
// TODO Auto-generated destructor stub
}

string Database::toString() {
	ostringstream rslt;
	for (pair<const std::basic_string<char>, Relation> p : relations) {
		rslt << p.second.toString();
	}
	return rslt.str();
}

void Database::addRelation(string & tempName, Relation & relation) {
	//TODO handle adding of duplicate relations.
	relations[tempName] = relation;
}

void Database::addTuple(string & schemeName, Tuple & tuple) {
	if (!relations.count(schemeName)) {
		throw "One of the facts' function names did not match any of the schemes provided.";
	}
	relations[schemeName].addTuple(tuple);
}

Relation Database::getRelation(string & name) {
	if (!relations.count(name)) {
		throw "One of the queries' or rules' function names did not match any of the schemes provided.";
	}
	return relations[name];
}

void Database::replaceRelation(Relation & replacementRelation) {
	relations[replacementRelation.getName()] = replacementRelation;
}

