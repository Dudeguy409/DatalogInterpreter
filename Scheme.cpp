/*
 * Scheme.cpp
 *
 *  Created on: Jul 22, 2014
 *      Author: davidsoa
 */

#include "Scheme.h"
#include "Parameter.h"
#include <sstream>

Scheme::Scheme(vector<string> & attribs) {
	attributes = attribs;
}

Scheme::Scheme() {
// TODO Auto-generated constructor stub
}

Scheme::~Scheme() {
// TODO Auto-generated destructor stub
}

int Scheme::size() {
	return attributes.size();
}

vector<string> Scheme::getAttributes() {
	return attributes;
}

string Scheme::toString() {
	ostringstream rslt;
	for (int i = 0; i < attributes.size(); i++) {
		rslt << attributes[i];
		if (i < attributes.size() - 1) {
			rslt << ",";
		}
	}
	return rslt.str();
}

string Scheme::getAttribute(int index) {
	return attributes[index];
}

