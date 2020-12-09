#include "BET.h"



BET::BET()
{

}

BET::BET(const char * expression)
{
	this->root = Parse(expression);
}

BET::BET(std::string expression)
{
	this->root = Parse(expression);
}

BET::~BET()
{
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

std::string BET::toString(node<>* p_node)
{
	std::string out = "";
	
	if (p_node->left != nullptr)
		out += toString(p_node->left);

	out += p_node->toString();

	if (p_node->right != nullptr)
		out += toString(p_node->right);

	return out;
}

node<>* BET::Parse(std::string expression, node<>* n)
{
	
	int brackets = 0;
	const size_t end = expression.length()-1;
	
	// TODO: Thread parantheses


	// Рассматриваем на предмет наличия плюсиков и минусиков
	int i = end;
	while (i >= 0 && expression[i] != '+' && expression[i] != '-')
	{
		i--;
	}

	if (i > 0)
	{
		n = new node<sign>(expression[i]);

		std::string ltoken = expression.substr(0, i);
		std::string rtoken = expression.substr(i + 1, end - i);
		n->left = Parse(ltoken, n->left);
		n->right = Parse(rtoken, n->right);

		return n;
	}
	else if (i == 0 && (expression[0] == '+' || expression[0] == '-'))
	{
		// unary +-
		std::cout << "WARNING: untreated unary sign!\n";
	}
	else if (i == 0 && (expression[0] != '+' || expression[0] != '-'))
	{
		// unary +-
		setlocale(LC_ALL, "ru");
		//std::cout << "Ебать ты шо дурак????\n";
		assert("Ебать ты шо дурак???? ты как это сделал");
	}

	

	// Process * and /
	i = end;
	while (i >= 0 && expression[i] != '*' && expression[i] != '/') i--;

	if (i > 0)
	{
		n = new node<sign>(expression[i]);

		std::string ltoken = expression.substr(0, i);
		std::string rtoken = expression.substr(i + 1, end - i);
		n->left = Parse(ltoken, n->left);
		n->right = Parse(rtoken, n->right);

		return n;
	}


	std::string token = trimBrackets(expression);

	if (isalpha(token.front()))
		n = new node<identifier>(token);
	else
		n = new node<number>(std::stoi(token));

	return n;


	/*
	for(int i = end; i >= 0; i--)
	{
		char c = expression[i];
		if (c == '+' || c == '-')
		{
			if (n == nullptr)
			{
				std::string rtoken = expression.substr(i + 1, end - i);
				std::string ltoken = expression.substr(0, i);
				n = new node<sign>(c);
				Parse(rtoken, n->right);
				Parse(ltoken, n->left);
				return n;
			}
		}
		else if (i == 0)
		{

		}
	}
	*/
	return nullptr;
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
