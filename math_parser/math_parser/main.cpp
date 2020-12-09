#include <iostream>
#include "BET.h"

int main()
{

	//node* root = new intNode(5);

	//std::cout << root->toString() << std::endl;

	//const char* e1 = "1 + b/c";
	//const char* e2 = "(c + b)/c";
	//const char* e3 = "c/c + b/c";
	//const char* e4 = "1*1*1*c/c + 2*1*b/c";

	//const char* e5 = "a*b + c*d + e*f";
	const char* e6 = "a + q*h/b + k*p"; // -> a + b - c
	const char* e7 = "a - (b - c)"; // -> a - b + c

	//node<> *node1 = new node<number>(5);
	//node<> *node2 = new node<identifier>("x");
	//node<> *node3 = new node<sign>('*');
	//
	//std::cout << std::endl;

	//std::cout << node1->getInfo() << std::endl;
	//std::cout << node2->getInfo() << std::endl;
	//std::cout << node3->getInfo() << std::endl;

	BET b1(e6);
	//BET b2(e7);

	std::cout << b1.toString() << std::endl;

	system("pause");
	return 0;
}