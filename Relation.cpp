/*
 * Relation.cpp
 *
 *  Created on: Jul 22, 2014
 *      Author: davidsoa
 */

using namespace std;
#include "Relation.h"
#include "Parameter.h"
#include <vector>
#include <sstream>
#include <iostream>

Relation::Relation(string & relationName, Scheme & schemeParam) {
	name = relationName;
	scheme = schemeParam;
	tuples = set<Tuple>();
}

Relation::Relation(string & relationName, Scheme & schemeParam,
		set<Tuple> & tuplesToAdd) {
	name = relationName;
	scheme = schemeParam;
	tuples = tuplesToAdd;
}

Relation::Relation() {
	// TODO Auto-generated constructor stub
}

Relation::~Relation() {
	// TODO Auto-generated destructor stub
}

void Relation::addTuple(Tuple & tuple) {
	if (tuple.size() != scheme.size()) {
		throw "One of the facts did not have the right number of parameters.";
	}
	tuples.insert(tuple);
}

Relation Relation::select(int position, string & value) {
	if (scheme.size() <= position) {
		throw "Internal error: select statement dimension mismatch";
	}

	set<Tuple> matchingTuples = set<Tuple>();
	for (set<Tuple>::iterator it = tuples.begin(); it != tuples.end(); ++it) {
		Tuple tempTuple = *it;
		if (tempTuple[position] == value) {
			matchingTuples.insert(tempTuple);
		}
	}
	return Relation(name, scheme, matchingTuples);
}

Relation Relation::select(vector<int> & positions) {
	//TODO re-uncomment this and potentially fix errors with multiple same IDs.
//	if (positions.size() == 0 || positions.size() == 1) {
//		throw "Internal error: multiple select statement position vector invalid.";
//	}
//	for (int i = 0, length = sizeof(positions); i < length; i++) {
//		if (scheme.size() <= positions[i]) {
//			throw "Internal error: multiple select statement dimension mismatch.  A position to select was out of range.";
//		}
//	}

	set<Tuple> matchingTuples = set<Tuple>();
	for (set<Tuple>::iterator it = tuples.begin(); it != tuples.end(); ++it) {
		Tuple tempTuple = *it;
		string value = tempTuple[positions[0]];
		bool matches = true;
		for (int i = 1; i < positions.size(); i++) {
			if (tempTuple[positions[i]] != value)
				matches = false;
		}
		if (matches) {
			matchingTuples.insert(tempTuple);
		}
	}
	return Relation(name, scheme, matchingTuples);

}

Relation Relation::project(vector<int> & positions, int varCount) {
	if (positions.size() != scheme.size()) {
		throw " project statement invalid.  Check that the dimensions in the query match.";
	}

	Scheme schemeToReturn = buildProjectedScheme(positions, varCount);
	set<Tuple> tuplesToReturn = buildProjectedTupleSet(positions, varCount);

	return Relation(name, schemeToReturn, tuplesToReturn);
}

Scheme Relation::buildProjectedScheme(vector<int> & positions, int varCount) {
	vector<string> oldAtts = scheme.getAttributes();
	vector<string> attsToSet = vector<string>();

	for (int i = 0; i < varCount; i++) {
		attsToSet.push_back("Error");
	}

	for (int i = 0; i < positions.size(); i++) {
		if (positions[i] >= 0)
			attsToSet[positions[i]] = oldAtts[i];
	}
	return Scheme(attsToSet);
}

set<Tuple> Relation::buildProjectedTupleSet(vector<int> & positions,
		int varCount) {
	set<Tuple> tuplesToReturn = set<Tuple>();
	for (Tuple tuple : tuples) {
		Tuple tupleToAdd = Tuple();
		for (int i = 0; i < varCount; i++) {
			tupleToAdd.push_back("Error");
		}

		for (int i = 0; i < positions.size(); i++) {
			if (positions[i] >= 0)
				tupleToAdd[positions[i]] = tuple[i];
		}
		tuplesToReturn.insert(tupleToAdd);
	}
	return tuplesToReturn;
}

Relation Relation::rename(int pos, string value) {
	vector<string> tempAtts = scheme.getAttributes();
	tempAtts[pos] = value;
	Scheme schemeToReturn = Scheme(tempAtts);
	return Relation(name, schemeToReturn, tuples);
}

string Relation::toString() {

	ostringstream rslt;

	rslt << "Relation: " << name << "\n" << "  Scheme:\n" << "  " << name << "("
			<< scheme.toString() << ")" << "\n" << "    Facts:\n";
	for (Tuple tuple : tuples) {
		rslt << "    ";
		for (int i = 0; i < tuple.size(); i++) {
			rslt << tuple[i];
			if (i < tuple.size() - 1) {
				rslt << ", ";
			}
		}
		rslt << "\n";
	}
	return rslt.str();
}

string Relation::toQueryResultString() {
	ostringstream rslt;

	if (tuples.size() == 0) {
		rslt << "No\n";
	} else {
		rslt << "Yes(" << tuples.size() << ")\n";
		for (Tuple tuple : tuples) {
			if (tuple.size() > 0) {
				rslt << "  ";
				for (int i = 0; i < tuple.size(); i++) {
					rslt << scheme.getAttribute(i) << "='" << tuple[i] << "'";
					if (i < tuple.size() - 1) {
						rslt << ", ";
					}
				}
				rslt << "\n";
			}
		}
	}
	return rslt.str();
}

Scheme Relation::getScheme() {
	return scheme;
}

string Relation::getName() {
	return name;
}

set<Tuple> Relation::getTupleSet() {
	return tuples;
}

Relation Relation::join(Relation relationToJoin, vector<int> positionsToJoin) {
	vector<string> joinedSchemeAttributes = scheme.getAttributes();
	vector<string> attributesToJoin =
			relationToJoin.getScheme().getAttributes();

	set<int> columnsToNotAdd = set<int>();
	for (int i : positionsToJoin) {
		columnsToNotAdd.insert(i);
	}

	for (int i = 0; i < attributesToJoin.size(); i++) {
		if (columnsToNotAdd.count(i) == 0) {
			joinedSchemeAttributes.push_back(attributesToJoin[i]);
		}
	}

	Scheme joinedScheme = Scheme(joinedSchemeAttributes);

	set<Tuple> joinedTuples = set<Tuple>();
	set<Tuple> currentTuples = tuples;
	set<Tuple> tuplesToJoin = relationToJoin.getTupleSet();

	for (Tuple currentTuple : currentTuples) {
		for (Tuple tupleToJoin : tuplesToJoin) {
			joinTuples(currentTuple, tupleToJoin, positionsToJoin, joinedTuples,
					columnsToNotAdd);
		}
	}

	string joinedSchemeName = "Error: intermediate relation";
	return Relation(joinedSchemeName, joinedScheme, joinedTuples);
}

void Relation::joinTuples(Tuple currentTuple, Tuple & tupleToJoin,
		vector<int> & positionsToJoin, set<Tuple> & joinedTuples,
		set<int> & columnsToNotAdd) {
	bool join = true;
	for (int i = 0; i < positionsToJoin.size(); i++) {
		if (positionsToJoin[i] >= 0) {
			if (currentTuple[i] != tupleToJoin[positionsToJoin[i]]) {
				join = false;
			}
		}
	}
	if (join) {
		for (int i = 0; i < tupleToJoin.size(); i++) {
			if (columnsToNotAdd.count(i) == 0) {
				currentTuple.push_back(tupleToJoin[i]);
			}
		}
		joinedTuples.insert(currentTuple);
	}
}

Relation Relation::unionRelations(Relation & intermediateRelation,
		vector<string> headIDs) {
	vector<string> attsToProject =
			intermediateRelation.getScheme().getAttributes();

	vector<int> positionsToProject = vector<int>();

	for (int j = 0; j < attsToProject.size(); j++) {
		int index = -1;
		for (int i = 0; i < headIDs.size(); i++) {
			if (headIDs[i] == attsToProject[j]) {
				index = i;
			}
		}
		positionsToProject.push_back(index);
	}

	intermediateRelation = intermediateRelation.project(positionsToProject,
			scheme.getAttributes().size());

	set<Tuple> unionedTuples = tuples;
	set<Tuple> tuplesToUnion = intermediateRelation.getTupleSet();
	for (Tuple tuple : tuplesToUnion) {
		unionedTuples.insert(tuple);
	}

	return Relation(name, scheme, unionedTuples);
}

int Relation::size() {
	return tuples.size();
}

