/*
 * Scheme.h
 *
 *  Created on: Jul 22, 2014
 *      Author: davidsoa
 */

#ifndef SCHEME_H_
#define SCHEME_H_

using namespace std;
#include <vector>
#include "Parameter.h"

class Scheme {
public:
	Scheme(vector<string> & attribs);
	Scheme();
	virtual ~Scheme();
	int size();
	vector<string> getAttributes();
	virtual string toString();
	string getAttribute(int index);
private:
	vector<string> attributes;
};

#endif /* SCHEME_H_ */
