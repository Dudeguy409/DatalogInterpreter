/*
 * Parser.cpp
 *
 *  Created on: Jul 5, 2014
 *      Author: davidsoa
 */

using namespace std;

#include "Parser.h"
#include "Token.h"
#include <set>
//#include <sstream>
#include "Predicate.h"
#include <vector>
#include "Parameter.h"

Parser::~Parser() {
	// TODO Auto-generated destructor stub
}

Parser::Parser(vector<Token> & v) {

	tokens = v;
	currentTokenIndex = 0;
	schemes = vector<Predicate>();
	facts = vector<Predicate>();
	rules = vector<Rule>();
	queries = vector<Predicate>();
	domain = set<string>();

}

DatalogProgram Parser::parse() {
	matchCurrentToken(TokenType::SCHEMES);
	matchCurrentToken(TokenType::COLON);
	parseSchemeList();
	matchCurrentToken(TokenType::FACTS);
	matchCurrentToken(TokenType::COLON);
	parseFactList();
	matchCurrentToken(TokenType::RULES);
	matchCurrentToken(TokenType::COLON);
	parseRuleList();
	matchCurrentToken(TokenType::QUERIES);
	matchCurrentToken(TokenType::COLON);
	parseQueryList();
	matchCurrentToken(TokenType::ENDOFFILE);

	return DatalogProgram(schemes, facts, rules, queries, domain);

}

void Parser::parseSchemeList() {
	do {
		//TODO must throw exception if empty.  Case where it isn't predicate already handled.
		schemes.push_back(parsePredicate());
	} while (tokens[currentTokenIndex].getKind() == TokenType::ID);
}

void Parser::parseFactList() {
	while (tokens[currentTokenIndex].getKind() == TokenType::ID) {
		facts.push_back(parsePredicate());
		matchCurrentToken(TokenType::PERIOD);
	}
}

void Parser::parseRuleList() {
	while (tokens[currentTokenIndex].getKind() == TokenType::ID) {
		Predicate tempPredicate = parsePredicate();
		matchCurrentToken(TokenType::COLON_DASH);
		vector<Predicate> tempPredList = parsePredicateList();
		matchCurrentToken(TokenType::PERIOD);
		rules.push_back(Rule(tempPredicate, tempPredList));
	}
}

void Parser::parseQueryList() {
	do {
		//TODO must throw exception if empty.  Case where it isn't predicate already handled.
		queries.push_back(parsePredicate());
		matchCurrentToken(TokenType::Q_MARK);
	} while (tokens[currentTokenIndex].getKind() == TokenType::ID);
}

vector<Predicate> Parser::parsePredicateList() {
	vector<Predicate> tempPredList;
	do {
		//TODO must throw exception if empty.  Case where it isn't predicate already handled.
		tempPredList.push_back(parsePredicate());
		if (tokens[currentTokenIndex].getKind() != TokenType::COMMA)
			return tempPredList;
		matchCurrentToken(TokenType::COMMA);
	} while (true);
}

Predicate Parser::parsePredicate() {
	string iDValue = tokens[currentTokenIndex].getValue();
	matchCurrentToken(TokenType::ID);
	matchCurrentToken(TokenType::LEFT_PAREN);
	vector<Parameter> tempParams = parseParameterList();
	matchCurrentToken(TokenType::RIGHT_PAREN);

	return Predicate(iDValue, tempParams);

}

vector<Parameter> Parser::parseParameterList() {
	vector<Parameter> tempParamList;
	do {
		//TODO must throw exception if empty.  Case where it isn't predicate already handled.
		tempParamList.push_back(parseParameter());
		if (tokens[currentTokenIndex].getKind() != TokenType::COMMA)
			return tempParamList;
		matchCurrentToken(TokenType::COMMA);
	} while (true);
}

Parameter Parser::parseParameter() {
	Parameter tempParam;
	if (tokens[currentTokenIndex].getKind() == TokenType::STRING) {
		domain.insert(tokens[currentTokenIndex].getValue());
		tempParam = Parameter(tokens[currentTokenIndex].getValue(), false);
		matchCurrentToken(TokenType::STRING);
	} else {
		tempParam = Parameter(tokens[currentTokenIndex].getValue(), true);
		matchCurrentToken(TokenType::ID);
	}
	return tempParam;
}

void Parser::matchCurrentToken(TokenType t) {
	if (tokens[currentTokenIndex].getKind() == t)
		currentTokenIndex++;
	else {
		throw tokens[currentTokenIndex].toString();
	}
}

