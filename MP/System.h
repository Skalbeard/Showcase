// System.h
// Windows API system wrapper.
#ifndef _SYSTEM_H_
#define _SYSTEM_H_


#include "Input.h"
#include "Game.h"


class System
{
public:
	System();
	~System();

	void Run();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	bool Frame();
	void InitializeWindows(int&, int&);
	void ShutdownWindows();

private:
	LPCWSTR m_applicationName;
	HINSTANCE m_hinstance;
	HWND m_hwnd;
	
	Game* m_Game;
	Input m_Input;

// Prototypes.
	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
};

// Globals.
static System* ApplicationHandle = 0;

#endif