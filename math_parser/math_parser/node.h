#pragma once

#include <sstream>
#include <string>
#include <iostream>

enum nodeType
{
	number,
	identifier,
	sign,
	parent,
	unknown
};

enum signType
{
	plus,
	minus,
	multiply,
	devide
};


template <nodeType t = nodeType::parent>
struct node
{
	nodeType type;
	node* left = nullptr;
	node* right = nullptr;

	virtual std::string getInfo() = 0;
	virtual std::string toString() = 0;

protected:
	template<typename v>
	std::string _getInfo(const char* type, v val)
	{
		std::stringstream ss;
		ss << "Type: " << type << '\n';
		ss << "Value: " << val << '\n';
		return ss.str();
	}
};

template <>
struct node<nodeType::identifier> : node<>
{
	std::string val;

	node(std::string identifier)
		: val(identifier)
	{
		//std::cout << "Constructor identifier. " << identifier << "\n";
		this->type = nodeType::identifier;
	}

	std::string getInfo()
	{
		return _getInfo < std::string > ("identifier", val);
	}

	std::string toString()
	{
		return val;
	}

};

template <>
struct node<nodeType::number> : node<>
{
	int val;

	node(int val)
		: val(val)
	{
		//std::cout << "Constructor number. " << val << "\n";
		this->type = nodeType::number;
	}

	std::string getInfo()
	{
		return _getInfo < int >("number", val);
	}

	std::string toString()
	{
		std::stringstream ss;
		ss << val;
		return ss.str();
	}
};

template <>
struct node<nodeType::sign> : node<>
{
	char val;

	node(char sign)
		: val(sign)
	{
		//std::cout << "Constructor sign. " << sign << "\n";
		this->type = nodeType::sign;
	}

	std::string getInfo()
	{
		return _getInfo < char >("sign", val);
	}

	std::string toString()
	{
		std::stringstream ss;
		ss << val;
		return ss.str();
	}
};
