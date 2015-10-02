/*
 * Main.cpp
 *
 *  Created on: Jun 27, 2014
 *      Author: davidsoa
 */

#include "Main.h"
#include <vector>
#include "Token.h"
#include <string>
#include <iostream>
#include <fstream>
#include "Tokenizer.h"
#include "DatalogProgram.h"
#include "Parser.h"
#include "Interpreter.h"

using namespace std;

int main(int argc, char* argv[]) {

	Tokenizer t(argv[1]);

	vector<Token> v = t.getTokens();

	ofstream out;
	try {

		out.open(argv[2]);

	} catch (int e) {
		cout << "The output file path was not valid.\n";
	}

	Parser p = Parser(v);
	DatalogProgram d;

	try {
		d = p.parse();
		Interpreter interpreter = Interpreter(d);
		out << interpreter.getResult();
	} catch (string e) {
		out << "Failure!\n  " << e;
		cout << "Failure!\n  " << e;
		return 0;
	} catch (char const* e) {
		cout << e;
		return 0;
	}

	return 0;
}
