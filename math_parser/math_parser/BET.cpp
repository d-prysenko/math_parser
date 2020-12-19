#include "BET.h"


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
	for (char &c : expression)
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

void BET::openBrackets()
{
	this->root = openBrackets(this->root);
}

node* BET::minusRefactor(node* p_node)
{
	node* temp = p_node->right;
	std::stack<node*> nodes;

	do
	{
		nodes.push(temp);
		temp = temp->left;
	} while (temp != nullptr && (temp->isSign('+') || temp->isSign('-')));

	// looks crazy, it would be cool to refactor this using recursion

	node* q;
	node* t;

	t = new node('-');
	t->left = p_node->left;
	t->right = nodes.top()->left;
	q = t;

	while (!nodes.empty())
	{
		if (nodes.top()->isSign('+'))
			t = new node('-');
		else
			t = new node('+');

		t->left = q;
		t->right = nodes.top()->right;
		nodes.pop();
		q = t;
	}
	return t;
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

	if (p_node->isSign('-') && (p_node->right->isSign('+') || p_node->right->isSign('-')))
	{
		p_node = minusRefactor(p_node);
	}
	

	return p_node;
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


	if (p_node->isSign('+') || p_node->isSign('-'))
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
	const size_t end = token.length()-1;

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
	else if (i == 0 && (token[0] == '+' || token[0] == '-'))
	{
		// unary +-
		std::cout << "WARNING: untreated unary sign!\n";
	}
	else if (i == 0)
	{
		assert("So critical error dude, gg");
	}

	

	// Process * and /
	i = end;
	brackets = 0;
	while (i >= 0 && (token[i] != '*' && token[i] != '/' || brackets != 0))
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
		std::cout << "Tb|, eBlan, identificator is invalid" << std::endl;
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
