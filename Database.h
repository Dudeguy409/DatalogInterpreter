/*
 * Database.h
 *
 *  Created on: Jul 22, 2014
 *      Author: davidsoa
 */

#ifndef DATABASE_H_
#define DATABASE_H_

using namespace std;
#include <map>
#include "DatalogProgram.h"
#include "Relation.h"
#include <string>

class Database {
public:
	Database();
	virtual ~Database();
	virtual string toString();
	void addRelation(string & tempName, Relation & relation);
	void addTuple(string & schemeName, Tuple & tuple);
	Relation getRelation(string & name);
	void replaceRelation(Relation & replacementRelation);
private:
	map<string, Relation> relations;
};

#endif /* DATABASE_H_ */
