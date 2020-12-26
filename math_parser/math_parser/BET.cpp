#include "BET.h"

#pragma warning(disable:26451)

node::node(std::string id)
	: left(nullptr), right(nullptr), type(nodeType::id)
{
	val.id = new std::string(id);
}

node::node(int num)
	: left(nullptr), right(nullptr), type(nodeType::number)
{
	val.number = num;
}

node::node(char sign)
	: left(nullptr), right(nullptr), type(nodeType::sign)
{
	val.sign = sign;
}

node::node(node& source)
	: left(nullptr), right(nullptr), type(source.type), val(source.val)
{
	if (type == nodeType::id)
		val.id = new std::string(*source.val.id);

	if (source.left != nullptr)
	{
		left = new node(*source.left);
	}

	if (source.right != nullptr)
	{
		right = new node(*source.right);
	}
}

node::~node()
{
	if (type == nodeType::id && val.id != nullptr)
	{
		delete val.id;
		val.id = nullptr;
	}
}

inline bool node::isSign(char c)
{
	return this->type == nodeType::sign && this->val.sign == c;
}

inline bool node::isplusminus()
{
	return isSign('+') || isSign('-');
}

std::string node::toString()
{
	switch (type)
	{
	case nodeType::number:
		return std::to_string(val.number);
	case nodeType::id:
		return *val.id;
	case nodeType::sign:
		return std::string(1, val.sign);
	default:
		assert("Critical error! Unknown node!");
		break;
	}
}

BET::BET()
	: root(nullptr)
{

}

BET::BET(const char* expression)
{
	this->root = Parse(expression);
}

BET::BET(std::string expression)
{
	this->root = Parse(expression);
}

BET::~BET()
{
	if (root != nullptr)
	{
		destruct(root);
		root = nullptr;
	}
}

void BET::destruct(node* p_node)
{
	if (p_node->left != nullptr)
	{
		destruct(p_node->left);
		p_node->left = nullptr;
	}

	if (p_node->right != nullptr)
	{
		destruct(p_node->right);
		p_node->right = nullptr;
	}

	delete p_node;
}

node* BET::Parse(std::string expression)
{
	for (char& c : expression)
	{
		if (!isblank(c))
			this->sourse += c;
	}

	ParsingError res = Analize(this->sourse);
	if (res != 0)
	{
		PrintParsingError(res);
		return nullptr;
	}

	return Parse(this->sourse, root);
}

std::string BET::toString()
{
	return toString(root);
}


void BET::transform()
{
	this->root = openBrackets(this->root);
	sort(this->root);
}

// Sorting method before comparison expression trees
void BET::sort(node* p_node)
{

}

node* BET::refactor(node* p_node, bool signInvert)
{
	if (p_node->left != nullptr && p_node->right != nullptr && (p_node->left->isplusminus() || p_node->right->isplusminus()))
	{

		fillNodesQueue(p_node);

		// [queue] nodes - consequenced nodes

		// There are a few memory leaks here

		node* left = nodes.front(); nodes.pop();

		node* new_node;
		node* tmp;
		do
		{
			tmp = nodes.front();
			new_node = new node(nodes.front()->val.sign); nodes.pop();
			new_node->left = left;
			new_node->right = nodes.front(); nodes.pop();
			left = new_node;
		} while (signInvert && tmp != p_node);

		while (nodes.size() != 0)
		{
			char sign = nodes.front()->val.sign;
			if (signInvert)
				if (sign == '+')
					sign = '-';
				else if (sign == '-')
					sign = '+';

			new_node = new node(sign); nodes.pop();
			new_node->left = left;
			new_node->right = nodes.front(); nodes.pop();
			left = new_node;
		}
		p_node = left;
	}
	return p_node;
}

node* BET::fillNodesQueue(node* p_node)
{
	if (p_node->isplusminus() && p_node->left != nullptr)
	{
		fillNodesQueue(p_node->left);
	}

	nodes.push(p_node);

	if (p_node->isplusminus() && p_node->right != nullptr)
	{
		fillNodesQueue(p_node->right);
	}

	return p_node;
}

node* BET::openBrackets(node* p_node)
{
	if (p_node == nullptr || p_node->right == nullptr || p_node->left == nullptr)
		return p_node;

	p_node->left = openBrackets(p_node->left);
	p_node->right = openBrackets(p_node->right);

	if (p_node->isSign('*')
		&& (p_node->left->type == nodeType::id || p_node->left->type == nodeType::number)
		&& (p_node->right->isSign('+') || p_node->right->isSign('-')))
		/*if (p_node->isSign('*')
		&& ((p_node->left->type != nodeType::id && p_node->left->type != nodeType::number) || (p_node->right->type != nodeType::id && p_node->right->type != nodeType::number)))*/
	{
		node* p_node_new = new node(p_node->right->val.sign);

		p_node_new->left = new node('*');
		p_node_new->left->left = p_node->left;
		p_node_new->left->right = p_node->right->left;
		p_node_new->left = openBrackets(p_node_new->left);

		p_node_new->right = new node('*');
		p_node_new->right->left = new node(*p_node->left);
		p_node_new->right->right = p_node->right->right;
		p_node_new->right = openBrackets(p_node_new->right);

		delete p_node->right;
		delete p_node;

		p_node = p_node_new;
	}

	if (p_node->isSign('-') && p_node->right != nullptr && p_node->right->isSign('-') && p_node->right->left == nullptr)
	{
		node* p_node_new;
		if (p_node->toString() == p_node->right->toString())
			p_node_new = new node('+');
		else
			p_node_new = new node('-');

		p_node_new->left = p_node->left;
		p_node_new->right = p_node->right->right;

		delete p_node->right;
		delete p_node;

		p_node = p_node_new;
	}

	if (p_node->isSign('-') && (p_node->right->isSign('+') || p_node->right->isSign('-')))
		return refactor(p_node, true);
	else
		return refactor(p_node);
}



std::string BET::setBrackets(node* parent_node, node* child_node)
{
	if (child_node != nullptr)
	{
		if ((parent_node->isSign('*') || parent_node->isSign('/')) && (child_node->isSign('+') || child_node->isSign('-'))
			|| (parent_node->isSign('/') && child_node->isSign('*'))
			|| (parent_node->isSign('-') && parent_node->left != child_node && (child_node->isSign('+') || child_node->isSign('-'))))
		{
			return "(" + toString(child_node) + ")";
		}
		else
			return toString(child_node);
	}

	return "";
}

std::string BET::toString(node* p_node)
{
	std::string out = "";

	out += setBrackets(p_node, p_node->left);


	if ((p_node->isSign('+') || p_node->isSign('-')) && p_node->left != nullptr)
	{
		out += " " + p_node->toString() + " ";
	}
	else
		out += p_node->toString();


	out += setBrackets(p_node, p_node->right);

	return out;
}

node* BET::Parse(std::string expression, node* n)
{
	std::string token = expression;
	int brackets = 0;
	const size_t end = token.length() - 1;

	// Process + and -
	int i = end;
	while (i >= 0 && (token[i] != '+' && token[i] != '-' || brackets != 0))
	{
		if (token[i] == '(')
			brackets++;
		else if (token[i] == ')')
			brackets--;

		i--;
	}

	if (i > 0)
	{
		n = new node(token[i]);

		std::string ltoken = trimBrackets(token.substr(0, i));
		std::string rtoken = trimBrackets(token.substr(i + 1, end - i));
		n->left = Parse(ltoken, n->left);
		n->right = Parse(rtoken, n->right);

		return n;
	}
	else if (i == 0 && token[0] != '+' && token[0] != '-')
	{
		assert("So critical error dude, gg");
	}



	// Process * and /
	int j = end;
	brackets = 0;
	while (j >= 0 && (token[j] != '*' && token[j] != '/' || brackets != 0))
	{
		if (token[j] == '(')
			brackets++;
		else if (token[j] == ')')
			brackets--;

		j--;
	}

	if (j > 0)
	{
		n = new node(token[j]);

		std::string ltoken = trimBrackets(token.substr(0, j));
		std::string rtoken = trimBrackets(token.substr(j + 1, end - j));
		n->left = Parse(ltoken, n->left);
		n->right = Parse(rtoken, n->right);

		return n;
	}

	// unary +-
	if (i == 0 && (token[0] == '+' || token[0] == '-'))
	{
		n = new node(token[0]);

		std::string rtoken = trimBrackets(token.substr(i + 1, end - i));
		n->right = Parse(rtoken, n->right);

		return n;
	}

	if (isalpha(token.front()))
	{
		n = new node(token);
	}
	else
		n = new node(std::stoi(token));

	return n;
}

std::string BET::trimBrackets(std::string str)
{
	if (str.front() == '(' && str.back() == ')')
		str = str.substr(1, str.length() - 2);

	return str;
}

ParsingError BET::Analize(std::string expression)
{
	int brackets = 0;
	char lastC = 0;
	for (char& c : expression)
	{
		if (c == '(')
		{
			brackets++;
		}
		else if (c == ')')
		{
			brackets--;
		}

		// OK: asd232fsa
		// Bad: 213asfas
		if (isdigit(lastC) && isalpha(c))
		{
			return ParsingError::Identificator;
		}
		else if ((lastC == '*' || lastC == '/') && (c == '*' || c == '/'))
		{
			return ParsingError::Signs;
		}

		lastC = c;

	}

	if (brackets != 0)
		return ParsingError::Parantheses;

	return ParsingError::Ok;
}

void BET::PrintParsingError(ParsingError e)
{
	switch (e)
	{
	case Parantheses:
		std::cout << "Error in parantheses" << std::endl;
		break;
	case Identificator:
		std::cout << "Identificator is invalid" << std::endl;
		break;
	case Signs:
		std::cout << "Two signs can't stand one right after another one" << std::endl;
		break;
	default:
		std::cout << "Unknown error" << std::endl;
		break;
	}
}

bool issign(const char c)
{
	if (c == '+' || c == '-' || c == '*' || c == '/')
		return true;

	return false;
}

bool islang(const char c)
{
	if (isalnum(c) || issign(c) || c == '^')
		return true;

	return false;
}

bool operator==(BET& left, BET& right)
{
	/*
	left.sort(left.root);
	right.sort(right.root);
	if (left.toString() == right.toString())
		return true;
	*/

	// There is no correct comparison code yet :(

	return false;
}
