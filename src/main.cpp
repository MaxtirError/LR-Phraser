#include "main.h"
int main()
{
	Regular r("(ba|a)*b*");
	XFA A = r.cast_to_XFA();
	A.printinfo();
	A.cast_to_DFA();
	A.printinfo();
	A.Minimize();
	A.printinfo();
	std::vector<std::string>rules1;
	rules1.push_back("S TG");
	rules1.push_back("G +TG");
	rules1.push_back("G 0");
	rules1.push_back("T FH");
	rules1.push_back("H *FH");
	rules1.push_back("H 0");
	rules1.push_back("F (S)");
	rules1.push_back("F a");
	LL1 LL1Praser(rules1, LL1RECURSION);
	LL1Praser.Prase();
	printf("%d\n", LL1Praser.Analyze("a*a+a"));
	return 0;
}
/*
S TE
E +TE
E 0
T FK
K *FK
K 0
F (S)
F a
-1
*/