#include <windows.h>
#include <stdio.h>
#include <tchar.h>

void _tmain( int argc, TCHAR *argv[] )
{
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    DWORD exitCode;


    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
    ZeroMemory( &pi, sizeof(pi) );

    if( argc != 2 )
    {
        printf("Usage: %s [cmdline]\n", argv[0]);
        return;
    }

    // Start the child process. 
    if( !CreateProcess( NULL,  
        "child.exe",        
        NULL,           
        NULL,           
        FALSE,          
        0,             
        NULL,          
        NULL,           
        &si,            
        &pi )           
    ) 
    {
        printf( "CreateProcess failed (%d).\n", GetLastError() );
        return;
    }

    
    WaitForSingleObject( pi.hProcess, INFINITE );

    printf("Child process failed with code: ");
    fflush(stdout);
    GetExitCodeProcess(pi.hProcess, &exitCode);

    CloseHandle( pi.hProcess );
    CloseHandle( pi.hThread );
}
