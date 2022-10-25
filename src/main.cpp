#include "main.h"
CPraser praser;
int main()
{
	Regular r("(ba|a)*b*");
	XFA A = r.cast_to_XFA();
	A.printinfo();
	A.cast_to_DFA();
	A.printinfo();
	A.Minimize();
	A.printinfo();
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