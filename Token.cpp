/*
 * Token.cpp
 *
 *  Created on: Jun 26, 2014
 *      Author: davidsoa
 */

#include "Token.h"
#include <sstream>

using namespace std;

struct X {
	static const char* enumtext[];
};

const char* X::enumtext[] = { "COMMA", "PERIOD", "Q_MARK", "LEFT_PAREN",
		"RIGHT_PAREN", "COLON", "COLON_DASH", "SCHEMES", "FACTS", "RULES",
		"QUERIES", "ID", "STRING", "ENDOFFILE", "ERROR" };

Token::Token(TokenType k, string& val, int lineNum) {
	value = val;
	lineNumber = lineNum;
	kind = k;

}

Token::Token(TokenType k, char val[], int lineNum) {
	string str(val);
	value = str;
	lineNumber = lineNum;
	kind = k;

}

Token::~Token() {
	// TODO Auto-generated destructor stub
}

string Token::toString() {

	if (kind == TokenType::ENDOFFILE)
		return "";

	ostringstream rslt;

	if (kind == TokenType::ERROR) {
		rslt << "Input Error on line " << lineNumber << "\n";
		string s = rslt.str();
		return s;
	}

	rslt << "(" << X::enumtext[kind] << "," << "\"" << value << "\","
			<< lineNumber << ")\n";
	string s = rslt.str();
	return s;
}

TokenType Token::getKind() {
	return kind;
}

string Token::getValue() {
	return value;
}

