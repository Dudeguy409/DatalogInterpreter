/*
 * Token.h
 *
 *  Created on: Jun 26, 2014
 *      Author: davidsoa
 */

#ifndef TOKEN_H_
#define TOKEN_H_

using namespace std;

#include <string>

enum TokenType {
	COMMA,
	PERIOD,
	Q_MARK,
	LEFT_PAREN,
	RIGHT_PAREN,
	COLON,
	COLON_DASH,
	SCHEMES,
	FACTS,
	RULES,
	QUERIES,
	ID,
	STRING,
	ENDOFFILE,
	ERROR
};

class Token {
public:
	Token(TokenType k, string& val, int lineNum);
	Token(TokenType k, char val[], int lineNum);
	virtual ~Token();
	virtual string toString();
	TokenType getKind();
	string getValue();

private:

	string value;
	int lineNumber;
	TokenType kind;

};

#endif /* TOKEN_H_ */
