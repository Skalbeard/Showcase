// BaseApplication.h
// Contains base application functionality. For inheritance purposes.
#ifndef _BASEAPPLICATION_H_
#define _BASEAPPLICATION_H_


#include "Input.h"
#include "D3D.h"
#include "Camera.h"
#include "Timer.h"
#include "Server.h"


class BaseApplication
{
public:
	BaseApplication(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input*);
	~BaseApplication();

	virtual bool Frame();

protected:
	virtual void HandleInput(float);
	virtual bool Render() = 0;

protected:
	Input*	m_Input;
	D3D*	m_Direct3D;
	Camera* m_Camera;
	Timer*	m_Timer;
	Server*	m_Server;
};

#endif