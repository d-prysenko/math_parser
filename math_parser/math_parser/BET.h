#pragma once

#pragma warning(disable:4996)

#include <sstream>
#include <string>
#include <iostream>
#include <cassert>
#include <stack>

enum class nodeType
{
	number,
	id,
	sign,
	parent,
	unknown
};

enum ParsingError
{
	Ok,
	Parantheses,
	Identificator,
	Signs
};

typedef nodeType tokenType;

struct node
{
	nodeType type;

	union
	{
		std::string* id;
		int number;
		char sign;
	} val;

	node* left;
	node* right;

	node(std::string);
	node(int);
	node(char);
	node(node& p_node);

	~node();

	inline bool isSign(char c);
	std::string toString();

};

class BET
{
public:
	BET();
	BET(const char* expression);
	BET(std::string expression);
	~BET();

	node* Parse(std::string expression);
	std::string toString();
	void openBrackets();

private:
	node* root;
	std::string sourse;

	node* Parse(std::string expression, node* n);
	std::string trimBrackets(std::string);
	std::string setBrackets(node* parent_node, node* child_node);
	node* openBrackets(node* p_node);
	node* minusRefactor(node* p_node);
	std::string toString(node* n);
	ParsingError Analize(std::string expression);
	void PrintParsingError(ParsingError e);

	void destruct(node* p_node);
};