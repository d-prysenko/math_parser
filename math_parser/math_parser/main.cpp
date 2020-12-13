#include <iostream>
#include "BET.h"

int main()
{

	//node* root = new intNode(5);

	//std::cout << root->toString() << std::endl;


	// a + 1/(b + c)
	// (a*(b + c) + 1) / (b + c)
	// (a*b + a*c + 1) / (b + c)
	// a*b/(b + c) + a*c/(b + c) + 1/(b + c)
	// a*b/(b + c) + a*c/(b + c) + (b + c)/((b + c)*(b + c))

	//const char* e1 = "1 + b/c";
	//const char* e2 = "(c + b)/c";
	//const char* e3 = "c/c + b/c";
	//const char* e4 = "1*1*1*c/c + 2*1*b/c";

	//const char* e5 = "a*b + c*d + e*f";
	const char* e6 = "a + b"; // -> a + b - c
	const char* e7 = "d + a*(b*(w - p) - q*(c + k))"; // -> a - b + c
	const char* e8 = "((6/2)*A+(8-5))/(E)"; // -> (6/2*A + 8 - 5)/E
	const char* e9 = "a + b*(c + d)";
	// d + a*(b*w + b*p + q*c + q)
	//node<> *node1 = new node<number>(5);
	//node<> *node2 = new node<identifier>("x");
	//node<> *node3 = new node<sign>('*');
	//
	//std::cout << std::endl;

	//std::cout << node1->getInfo() << std::endl;
	//std::cout << node2->getInfo() << std::endl;
	//std::cout << node3->getInfo() << std::endl;
	
	// Passed
	//BET b1(e7);
	//std::cout << b1.toString() << std::endl;
	//b1.openBrackets();
	//std::cout << b1.toString() << std::endl << std::endl;

	//b1.~BET();

	BET b3(e8);
	std::cout << e8 << "  ->  " << b3.toString() << std::endl << std::endl;


	BET b2(e9);
	std::cout << b2.toString() << std::endl;
	b2.openBrackets();
	std::cout << b2.toString() << std::endl << std::endl;


	system("pause");
	return 0;
}