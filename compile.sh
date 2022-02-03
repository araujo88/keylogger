gcc server.c -o server.o
i686-w64-mingw32-gcc -o backdoor_win32.exe backdoor.c -lwsock32 -lwininet