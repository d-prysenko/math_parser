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
	const char* e6 = "a + b";
	const char* e7 = "d + a*(b*(w - p) - q*(c + k))";
	const char* e9 = "a - d*(e + f + n)";

	// a - (b*(c + d) + e*(f + g))

	// a - (d*e + d*f + d*n)
	//			-
	//		a		+
	//			+		d*n
	//		d*e	  d*f

	// a - d*e - d*f - d*n
	//				-
	//			-		d*n
	//		-		d*f
	//	a		d*e

	// a - (d*e - d*f + d*n)
	//			-
	//		a		+
	//			-		d*n
	//		d*e	  d*f

	// a - d*e + d*f - d*n
	//				-
	//			+		d*n
	//		-		d*f
	//	a		d*e

	//
	// d + a*(b*w + b*p + q*c + q)
	//node<> *node1 = new node<number>(5);
	//node<> *node2 = new node<identifier>("x");
	//node<> *node3 = new node<sign>('*');
	//
	//std::cout << std::endl;

	std::string a = "abc";
	std::string b = "aaa";
	


	BET b2("a - (b*(c + d) + e*(f + g))");
	//BET b2("a*(b + c + e)");

	//BET b1("a - c + b");

	//			+
	//		-		b
	//	a		c

	//			-
	//		+		c
	//	a		b


	//				+
	//			+	   q*k
	//		+	   q*c
	//	b*w	  b*p

	//				  +
	//			+	  	    +
	//	   b*p		+	    q*c   q*k
	//			a		b*w

	std::cout << b2.toString() << std::endl;
	b2.transform();
	std::cout << b2.toString() << std::endl;


	// a*(b + c)			-> a*b + a*c
	// (a + b)*(c + d)		-> (a + b)*c + (a + b)*d	-> a*c + b*c + a*d + b*d
	// (a + b + c)*(d + e)	-> (a + b + c)*d + (a + b + c)*e	 -> ...
	// (a + b)*(c + d + e)	-> (a + b)*c + (a + b)*d + (a + b)*e ->



	/*std::cout << b1.toString();

	if (b1 == b2)
		std::cout << " == ";
	else
		std::cout << " != ";
		
	std::cout << b1.toString() << std::endl;

	std::cout << a.compare(b) << std::endl;*/

	system("pause");
	return 0;
}