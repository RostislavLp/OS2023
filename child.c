#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

BOOL WINAPI CtrlHandler(DWORD fdwCtrlType)
{
    switch (fdwCtrlType)
    {
        
    case CTRL_C_EVENT:
        printf("Ctrl-C event\n\n");
        Beep(750, 300);
        exit(2);

    case CTRL_CLOSE_EVENT:
        Beep(600, 200);
        printf("Ctrl-Close event\n\n");
        exit(2);

    case CTRL_BREAK_EVENT:
        Beep(900, 200);
        printf("Ctrl-Break event\n\n");
        exit(2);

    case CTRL_LOGOFF_EVENT:
        Beep(1000, 200);
        printf("Ctrl-Logoff event\n\n");
        exit(2);

    case CTRL_SHUTDOWN_EVENT:
        Beep(750, 500);
        printf("Ctrl-Shutdown event\n\n");
        exit(2);

    default:
        return FALSE;
    }
}

int main()
{
    if (SetConsoleCtrlHandler(CtrlHandler, TRUE))
    {
        printf("Waiting for signal\n");

        while (1) {}
    }
	return 0;
}
