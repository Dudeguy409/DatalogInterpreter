/*
 * Parameter.cpp
 *
 *  Created on: Jul 5, 2014
 *      Author: davidsoa
 */

#include "Parameter.h"
#include <sstream>

using namespace std;

Parameter::Parameter(string val, bool isIdent) {
	value = val;
	isIdentifier = isIdent;
}

Parameter::Parameter() {
	// Does nothing.
}

Parameter::~Parameter() {
	// TODO Auto-generated destructor stub
}

string Parameter::toString() {

	ostringstream rslt;
	if (isIdentifier)
		rslt << value;
	else
		rslt << "'" << value << "'";
	string s = rslt.str();
	return s;
}

bool Parameter::isID() {
	return isIdentifier;
}

string Parameter::getValue() {
	return value;
}

