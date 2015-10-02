/*
 * Relation.h
 *
 *  Created on: Jul 22, 2014
 *      Author: davidsoa
 */

#ifndef RELATION_H_
#define RELATION_H_

using namespace std;
#include <set>
#include "Tuple.h"
#include "Scheme.h"
#include <vector>
#include <string>
#include "Parameter.h"

class Relation {
public:
	Relation(string & relationName, Scheme & schemeParam);
	Relation(string & relationName, Scheme & schemeParam,
			set<Tuple> & tuplesToAdd);
	Relation();
	virtual ~Relation();
	Relation select(int position, string & value);
	Relation select(vector<int> & positions);
	Relation project(vector<int> & positions, int varCount);
	Relation rename(int pos, string value);
	void addTuple(Tuple & tuple);
	virtual string toString();
	string toQueryResultString();
	Scheme getScheme();
	Relation join(Relation relationToJoin, vector<int> positionsToJoin);
	set<Tuple> getTupleSet();
	string getName();
	Relation unionRelations(Relation & intermediateRelation,
			vector<string> headIDs);
	int size();
private:
	set<Tuple> tuples;
	Scheme scheme;
	string name;
	Scheme buildProjectedScheme(vector<int> & positions, int varCount);
	set<Tuple> buildProjectedTupleSet(vector<int> & positions, int varCount);
	void joinTuples(Tuple currentTuple, Tuple & tupleToJoin,
			vector<int> & positionsToJoin, set<Tuple> & joinedTuples,
			set<int> & columnsToNotAdd);
};

#endif /* RELATION_H_ */
