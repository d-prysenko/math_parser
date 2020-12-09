#pragma once

#pragma warning(disable:4996)

#include <cassert>
#include <stack>
#include "node.h"

typedef nodeType tokenType;

enum ParsingError
{
	Ok,
	Parantheses,
	Identificator,
	Signs
};

class BET
{
public:
	BET();
	BET(const char* expression);
	BET(std::string expression);
	~BET();

	node<>* Parse(std::string expression);
	std::string toString();

	class iterator
	{
	public:
		std::string& operator*();
		std::string& operator++();
	private:
	};

private:
	node<>* root;
	std::string sourse;

	node<>* Parse(std::string expression, node<>* n);
	std::string trimBrackets(std::string);
	std::string toString(node<>* n);
	ParsingError Analize(std::string expression);
	void PrintParsingError(ParsingError e);
};

bool issign(const char c);