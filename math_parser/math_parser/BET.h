#pragma once

#pragma warning(disable:4996)

#include <sstream>
#include <string>
#include <iostream>
#include <cassert>
#include <stack>
#include <queue>

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
	inline bool isplusminus();
	std::string toString();

};

class BET
{
public:
	BET();
	BET(const char* expression);
	BET(std::string expression);
	~BET();

	friend bool operator==(BET &left, BET &right);

	node* Parse(std::string expression);
	std::string toString();
	void transform();
	

private:
	node* root;
	std::string sourse;
	std::queue<node*> nodes;

	node* Parse(std::string expression, node* n);
	std::string trimBrackets(std::string);
	std::string setBrackets(node* parent_node, node* child_node);
	node* openBrackets(node* p_node);
	node* refactor(node* p_node, bool signInvert = false);
	node* refactor2(node* p_node);
	void sort(node* p_node);
	std::string toString(node* n);
	ParsingError Analize(std::string expression);
	void PrintParsingError(ParsingError e);

	void destruct(node* p_node);
};