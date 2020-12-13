#pragma once

#pragma warning(disable:4996)

#include <cassert>
#include <map>
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
	void openBrackets();
	

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
	std::map<std::string, node<>*> symboltable;

	node<>* Parse(std::string expression, node<>* n);
	std::string trimBrackets(std::string);
	std::string setBrackets(node<>* parent_node, node<>* child_node);
	node<>* openBrackets(node<>* p_node);
	std::string toString(node<>* n);
	ParsingError Analize(std::string expression);
	void PrintParsingError(ParsingError e);

	void destruct(node<>* p_node);
};

bool issign(const char c);