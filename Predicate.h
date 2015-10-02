/*
 * Predicate.h
 *
 *  Created on: Jul 5, 2014
 *      Author: davidsoa
 */

#ifndef PREDICATE_H_
#define PREDICATE_H_

using namespace std;
#include <vector>
#include "Parameter.h"

class Predicate {
public:
	Predicate(string predName, vector<Parameter> & parameters);
	Predicate();
	virtual ~Predicate();
	virtual string toString();
	string getName();
	vector<Parameter> getParamList();
private:
	string name;
	vector<Parameter> params;
};

#endif /* PREDICATE_H_ */
