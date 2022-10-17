#include "main.h"
CPraser praser;
int main()
{
	praser.input();
	praser.SLR_Prase();
	praser.ShowInfo();
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