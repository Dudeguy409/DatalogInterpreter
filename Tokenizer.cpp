/*
 * Tokenizer.cpp
 *
 *  Created on: Jun 25, 2014
 *      Author: davidsoa
 */

#include "Tokenizer.h"
#include <istream>
#include <vector>
#include <sstream>
#include "Token.h"
#include <iostream>

using namespace std;

Tokenizer::Tokenizer(char* fileName) {

	isAtEnd = 0;
	isInvalid = 0;
	tokens = vector<Token>();
	lineCount = 1;

	try {
		in.open(fileName);

	} catch (int e) {
		cout << "The input file path was not recognized.\n";
	}

	do {

		Token token = scan();
		tokens.push_back(token);
	} while (!isAtEnd && !isInvalid);

}

Tokenizer::~Tokenizer() {
// TODO Auto-generated destructor stub
}

Token Tokenizer::scan() {

	char c = removeWhiteSpace();

	switch (c) {
	case -1:
		isAtEnd = 1;
		return Token(TokenType::ENDOFFILE, "", lineCount);
	case ',':
		return Token(TokenType::COMMA, ",", lineCount);
	case '.':
		return Token(TokenType::PERIOD, ".", lineCount);
	case '?':
		return Token(TokenType::Q_MARK, "?", lineCount);
	case '(':
		return Token(TokenType::LEFT_PAREN, "(", lineCount);
	case ')':
		return Token(TokenType::RIGHT_PAREN, ")", lineCount);
	case ':':
		if (in.get() == '-') {
			return Token(TokenType::COLON_DASH, ":-", lineCount);
		}
		in.unget();
		return Token(TokenType::COLON, ":", lineCount);
	case '\'':
		return scanString();
	default:
		return scanID(c);
	}
}

char Tokenizer::removeWhiteSpace() {
	char c = in.get();

	int flag1 = 0;

	while (flag1 == 0) {
		if (c == '\n') {
			lineCount++;
			c = in.get();
		} else if (c == ' ' || c == '\t')
			c = in.get();
		else if (c == '#') {
			while (c != '\n' && c != -1) {
				c = in.get();
			}
		} else
			flag1 = 1;
	}
	return c;
}

Token Tokenizer::scanString() {

	ostringstream rslt;
	int flag2 = 0;
	while (flag2 < 1) {
		char n = in.get();
		if (n == -1 || n == '\n') {
			isAtEnd = 1;
			isInvalid = 1;
			return Token(TokenType::ERROR, "", lineCount);
		}
		if (n == '\'')
			flag2 = 1;
		else
			rslt << n;
	}
	string s = rslt.str();
	return Token(TokenType::STRING, s, lineCount);
}

Token Tokenizer::scanID(char c) {

	if (!isalpha(c)) {
		isAtEnd = 1;
		isInvalid = 1;
		cout << "you have an invalid character in the file.  Char value: "
				<< static_cast<int>(c) << ".\n";
		return Token(TokenType::ERROR, "", lineCount);
	}

	stringstream rslt;
	rslt << c;
	int flag = 0;
	while (flag == 0) {
		char n = in.get();
		if (!(isalpha(n) || isdigit(n))) {
			in.unget();
			flag = 1;
		} else
			rslt << n;
	}
	string s = rslt.str();

	return divideIDsAndKeys(s);

}

Token Tokenizer::divideIDsAndKeys(string& value) {

	if (value == "Schemes")
		return Token(TokenType::SCHEMES, value, lineCount);
	if (value == "Facts")
		return Token(TokenType::FACTS, value, lineCount);
	if (value == "Queries")
		return Token(TokenType::QUERIES, value, lineCount);
	if (value == "Rules")
		return Token(TokenType::RULES, value, lineCount);
	return Token(TokenType::ID, value, lineCount);
}

vector<Token> Tokenizer::getTokens() {
	return tokens;
}

bool Tokenizer::getIsInvalid() {
	return isInvalid;
}
