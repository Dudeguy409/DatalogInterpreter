/*
 * Predicate.cpp
 *
 *  Created on: Jul 5, 2014
 *      Author: davidsoa
 */

using namespace std;

#include "Predicate.h"
#include <sstream>

Predicate::Predicate(string predName, vector<Parameter> & parameters) {
	name = predName;
	params = parameters;
}

Predicate::Predicate() {
	// Does nothing.
}

Predicate::~Predicate() {
	// TODO Auto-generated destructor stub
}

string Predicate::toString() {

	ostringstream rslt;

	rslt << name << "(";
	for (int i = 0; i < params.size(); i++) {
		rslt << params[i].toString();
		if (i != params.size() - 1)
			rslt << ",";
	}
	rslt << ")";
	string s = rslt.str();
	return s;
}

string Predicate::getName() {
	return name;
}

vector<Parameter> Predicate::getParamList() {
	return params;
}

