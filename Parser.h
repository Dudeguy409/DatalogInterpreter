/*
 * Parser.h
 *
 *  Created on: Jul 5, 2014
 *      Author: davidsoa
 */

#ifndef PARSER_H_
#define PARSER_H_

using namespace std;

#include <vector>
#include "Token.h"
#include "DatalogProgram.h"
#include "Predicate.h"
#include "Parameter.h"
#include "Rule.h"
#include <set>
#include <string>

class Parser {
public:
	virtual ~Parser();
	Parser(vector<Token> & v);
	DatalogProgram parse();
private:
	void parseSchemeList();
	void parseFactList();
	void parseRuleList();
	void parseQueryList();
	vector<Predicate> parsePredicateList();
	Predicate parsePredicate();
	vector<Parameter> parseParameterList();
	Parameter parseParameter();
	void matchCurrentToken(TokenType t);
	vector<Token> tokens;
	int currentTokenIndex;
	vector<Predicate> schemes;
	vector<Predicate> facts;
	vector<Rule> rules;
	vector<Predicate> queries;
	set<string> domain;
};

#endif /* PARSER_H_ */
