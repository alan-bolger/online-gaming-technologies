// This code is based on a tutorial by Pindrought
// Original source code can be downloaded at http://www.mediafire.com/download/gcd4n1dpo55tkg6/Tutorial_7.rar

#include "Server.h"

int main()
{
	Server f_onlineTagServer(8888);

	for (int i = 0; i < 100; i++)
	{
		f_onlineTagServer.ListenForNewConnection();
	}

	system("pause");
	return 0;
}