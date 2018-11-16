// Utils.h
// Utility functions for the client and server.
#ifndef _UTILS_H_
#define _UTILS_H_

// Print an error message and die.
void die(const char *message);

// Initialise the WinSock library.
void startWinSock();

// In a Win32 application, open a console window and redirect the
// standard input, output and error streams to it.
void openConsoleWindow();

// Console output handle.
void handleToConsole(DWORD handle, FILE *stream, const char *mode);

#endif