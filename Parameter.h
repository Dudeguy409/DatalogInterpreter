/*
 * Parameter.h
 *
 *  Created on: Jul 5, 2014
 *      Author: davidsoa
 */

#ifndef PARAMETER_H_
#define PARAMETER_H_

using namespace std;
#include <string>

class Parameter {
public:
	Parameter(string val, bool isIdent);
	Parameter();
	virtual ~Parameter();
	virtual string toString();
	bool isID();
	string getValue();
private:
	string value;
	bool isIdentifier;
};

#endif /* PARAMETER_H_ */
