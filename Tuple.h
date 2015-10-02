/*
 * Tuple.h
 *
 *  Created on: Jul 22, 2014
 *      Author: davidsoa
 */

#ifndef TUPLE_H_
#define TUPLE_H_

using namespace std;
#include <vector>
#include <string>

class Tuple: public vector<string> {
public:
	Tuple();
	virtual ~Tuple();
};

#endif /* TUPLE_H_ */
