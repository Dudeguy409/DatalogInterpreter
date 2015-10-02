/*
 * Tokenizer.h
 *
 *  Created on: Jun 25, 2014
 *      Author: davidsoa
 */

#ifndef TOKENIZER_H_
#define TOKENIZER_H_

using namespace std;

#include <fstream>
#include "Token.h"
#include <vector>

class Tokenizer {
public:
	Tokenizer(char* fileName);
	virtual ~Tokenizer();
	vector<Token> getTokens();
	bool getIsInvalid();

private:
	Token scan();
	char removeWhiteSpace();
	Token scanString();
	Token scanID(char c);
	Token divideIDsAndKeys(string& value);

	bool isAtEnd;
	bool isInvalid;
	vector<Token> tokens;
	ifstream in;
	int lineCount;
};

#endif /* TOKENIZER_H_ */
