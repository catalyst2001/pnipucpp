#pragma once
/**
* DEBUG HELP HEADER
* 
* Created: 12.04.2023
* Modified: 12.04.2023
* Author: Deryabin K.
*/

#if defined(_MSC_VER) && !defined(__PRETTY_FUNCTION__)
#define __PRETTY_FUNCTION__ __FUNCSIG__
#endif

#include <stdio.h>

#ifdef _DEBUG
#ifdef _WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#define DBG(x, ...) fprintf(stderr, __PRETTY_FUNCTION__": " x "\n", __VA_ARGS__)

inline void initialize_output()
{
	FILE *p_conout;
	if (AllocConsole()) {
		freopen_s(&p_conout, "conout$", "w", stdout);
		freopen_s(&p_conout, "conout$", "w", stderr);
		freopen_s(&p_conout, "conin$", "w", stdin);
	}
}

#define DBG_INIT() (initialize_output())
#else
#error "Directive _WINDOWS is not defined. DBG_INIT() is not implemented for this platform!"
#endif

#else
#define DBG_INIT()
#define DBG(x, ...)
#endif