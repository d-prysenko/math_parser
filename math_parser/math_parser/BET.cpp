#include "BET.h"



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

void BET::destruct(node<>* p_node)
{
	//auto it = std::find(symboltable.begin(), symboltable.end(), p_node);
	//if (p_node->type == identifier && it == symboltable.end())
	//	return;

	if (p_node->left != nullptr)
		destruct(p_node->left);

	if (p_node->right != nullptr)
		destruct(p_node->right);

	//if (p_node->type == identifier)
	//	symboltable.erase(it);

	delete p_node;
}

node<>* BET::Parse(std::string expression)
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

node<>* BET::openBrackets(node<>* p_node)
{
	if (p_node == nullptr || p_node->right == nullptr || p_node->left == nullptr)
		return p_node;



	if (p_node->toString() == "*"
	&& (p_node->left->type == nodeType::identifier || p_node->left->type == nodeType::number)
	&& (p_node->right->toString() == "+" || p_node->right->toString() == "-"))
	{
		node<sign>* p_node_new = new node<sign>(p_node->right->toString().front());

		p_node_new->left = new node<sign>('*');
		p_node_new->left->left = p_node->left;
		p_node_new->left->right = p_node->right->left;

		p_node_new->right = new node<sign>('*');
		// Тут нада полностью скопировать объект
		p_node_new->right->left = p_node->left;
		//
		p_node_new->right->right = p_node->right->right;

		delete p_node->right;
		delete p_node;

		return p_node_new;
	}

	p_node->left = openBrackets(p_node->left);
	p_node->right = openBrackets(p_node->right);
}

std::string BET::setBrackets(node<>* parent_node, node<>* child_node)
{
	if (child_node != nullptr)
	{
		if ((parent_node->toString() == "*" || parent_node->toString() == "-") && (child_node->toString() == "+" || child_node->toString() == "-"))
		{
			return "(" + toString(child_node) + ")";
		}
		else
			return toString(child_node);
	}

	return "";
}

std::string BET::toString(node<>* p_node)
{
	std::string out = "";
	
	out += setBrackets(p_node, p_node->left);


	if (p_node->toString() == "+" || p_node->toString() == "-")
	{
		out += " " + p_node->toString() + " ";
	}
	else
		out += p_node->toString();


	out += setBrackets(p_node, p_node->right);

	return out;
}

node<>* BET::Parse(std::string expression, node<>* n)
{
	std::string token = trimBrackets(expression);

	int brackets = 0;
	const size_t end = token.length()-1;

	// Рассматриваем на предмет наличия плюсиков и минусиков
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
		n = new node<sign>(token[i]);

		std::string ltoken = token.substr(0, i);
		std::string rtoken = token.substr(i + 1, end - i);
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
		// ну это будет если введена хуета и первый символ это вообще хуй пойми что

		setlocale(LC_ALL, "ru");
		//std::cout << "Ебать ты шо дурак????\n";
		assert("Ебать ты шо дурак???? ты как это сделал");
	}

	

	// Process * and /
	i = end;
	while (i >= 0 && token[i] != '*' && token[i] != '/') i--;

	if (i > 0)
	{
		n = new node<sign>(token[i]);

		std::string ltoken = token.substr(0, i);
		std::string rtoken = token.substr(i + 1, end - i);
		n->left = Parse(ltoken, n->left);
		n->right = Parse(rtoken, n->right);

		return n;
	}

	if (isalpha(token.front()))
	{
		auto it = symboltable.find(token);
		if (it != symboltable.end())
		{
			n = it->second;
		}
		else
		{
			n = new node<identifier>(token);
			symboltable[token] = n;
		}
	}
	else
		n = new node<number>(std::stoi(token));

	return n;
}

std::string BET::trimBrackets(std::string str)
{
	while (str.front() == '(' && str.back() == ')')
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

std::string & BET::iterator::operator*()
{
	// TODO: вставьте здесь оператор return
	std::string res = "overloaded operator * for custom iterator\n";
	return res;
}
