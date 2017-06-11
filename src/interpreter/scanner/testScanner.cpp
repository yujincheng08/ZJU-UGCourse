#include "Scanner.h"
#include "def.h"
#include <iostream>

using namespace std;

int main()
{
	ios::sync_with_stdio(false);
	Scanner scanner;
	while(int token = scanner.lex())
	{
		const string &text = scanner.matched();
		cout<<"Get: "<<token<<" with "<<text<<endl;
	}
	return 0;
}
