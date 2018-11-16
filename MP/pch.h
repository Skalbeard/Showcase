// pch.h
// Header for standard system include files.
#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <WinSDKVer.h>
#define _WIN32_WINNT 0x0600
#include <SDKDDKVer.h>

// Use the C++ standard templated min/max
#define NOMINMAX

#define WIN32_LEAN_AND_MEAN

// Windows and system.
#include <Windows.h>
#include <iostream>
#include <string>
#include <assert.h>
#include <cstdint>
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <fcntl.h>
#include <algorithm>
#include <exception>
#include <memory>
#include <malloc.h>
#include <list>
#include <io.h>

// DirectX tool kit package..
#include <DDSTextureLoader.h>
#include <WICTextureLoader.h>
#include <SpriteFont.h>
#include <SpriteBatch.h>

// DirectX.
#include <d3d11_1.h>
#include <dxgi.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <DirectXColors.h>

// Sockets.
#include <WinSock2.h>
#include <WS2tcpip.h>


/////////////////////////////////////////////////////////////////////////////////
//                              MEMORY HOOKS SETUP
//
//                             ##### IMPORTANT #####
//
// These custom alloc/free functions are declared as "extern" in AkMemoryMgr.h
// and MUST be defined by the game developer.
/////////////////////////////////////////////////////////////////////////////////

namespace AK
{
#ifdef WIN32
	void * AllocHook(size_t in_size)
	{
		return malloc(in_size);
	}
	void FreeHook(void * in_ptr)
	{
		free(in_ptr);
	}

	// Note: VirtualAllocHook() may be used by I/O pools of the default implementation
	// of the Stream Manager, to allow "true" unbuffered I/O (using FILE_FLAG_NO_BUFFERING
	// - refer to the Windows SDK documentation for more details). This is NOT mandatory;
	// you may implement it with a simple malloc().
	void * VirtualAllocHook(
		void * in_pMemAddress,
		size_t in_size,
		DWORD in_dwAllocationType,
		DWORD in_dwProtect
		)
	{
		return VirtualAlloc(in_pMemAddress, in_size, in_dwAllocationType, in_dwProtect);
	}
	void VirtualFreeHook(
		void * in_pMemAddress,
		size_t in_size,
		DWORD in_dwFreeType
		)
	{
		VirtualFree(in_pMemAddress, in_size, in_dwFreeType);
	}
#endif
}

// Global settings.
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 200.0f;	// Clipping space.
const float SCREEN_NEAR = 0.1f;
const int DEPTHMAP_WIDTH = 800;
const int DEPTHMAP_HEIGHT = 600;
const int SHADOWMAP_WIDTH = 800;
const int SHADOWMAP_HEIGHT = 600;

// Namespaces.
using namespace DirectX;


// Safe COM cleanup.
template<typename T>
inline void SafeRelease(T *&ptr)
{
	if (ptr != nullptr)
	{
		ptr->Release();
		ptr = nullptr;
	}
}


// Error display messages in a windowbox.
// Validate.
#ifndef VALIDATE
#define VALIDATE(x, msg) if (!(x)) { MessageBoxA(nullptr, (msg), "VR-BCI Framework", MB_ICONERROR | MB_OK); exit(-1); }
#endif