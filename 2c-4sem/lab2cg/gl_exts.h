#pragma once

#include <Windows.h>

#ifndef __GL_H__
#include <gl/gl.h>
#include <gl/glu.h>
#endif

#define GL_ARRAY_BUFFER 0x8892
#define GL_STATIC_DRAW 0x88E4

typedef size_t GLsizeiptr;
typedef void (WINAPI *PFNGLGENBUFFERSPROC)(GLsizei n, GLuint *buffers);
typedef void (WINAPI *PFNGLBINDBUFFERPROC)(GLenum target, GLuint buffer);
typedef void (WINAPI *PFNGLDELETEBUFFERSPROC)(GLsizei n, const GLuint *buffers);
typedef void (WINAPI *PFNGLBUFFERDATAPROC)(GLenum target, GLsizeiptr size, const void *data, GLenum usage);

extern PFNGLGENBUFFERSPROC glGenBuffers;
extern PFNGLDELETEBUFFERSPROC glDeleteBuffers;
extern PFNGLBINDBUFFERPROC glBindBuffer;
extern PFNGLBUFFERDATAPROC glBufferData;

#define LOAD_WGL_EXTENSION_OPTIONAL(procvar, type, procname) \
	procvar = (type)wglGetProcAddress(procname); \
	if(!procvar) \
		MessageBoxA(0, "Failed to load " procname " extension", "WGL extension missing", MB_ICONWARNING);

#define LOAD_GL_EXTENSION_OPTIONAL(procvar, type, procname) \
	procvar = (type)wglGetProcAddress(procname); \
	if(!procvar) \
		MessageBoxA(0, "Failed to load " procname " extension", "OpenGL extension missing", MB_ICONWARNING);

#define LOAD_GL_EXTENSION_NEEDED(procvar, type, procname) \
	procvar = (type)wglGetProcAddress(procname); \
	if(!procvar) { \
		MessageBoxA(0, "Failed to load " procname " extension", "OpenGL extension missing", MB_ICONERROR); \
		exit(1); \
	}

void gl_load_extensions();