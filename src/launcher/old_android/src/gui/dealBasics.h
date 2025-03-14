#ifndef _DEALBASICS_H
#define _DEALBASICS_H

#include <EGL/egl.h>
#include <GLES/gl.h>
#include <GLES2/gl2.h>

#define EGL_CHECKCOMMANDS 1

#ifdef EGL_CHECKCOMMANDS
	class dealLauncher;
	void dbgCheckEglError( dealLauncher &launcher, const char *file, int line );
	void dbgCheckOglError( dealLauncher &launcher, const char *file, int line );
	#define EGL_CHECK(launcher,cmd)    glGetError(); cmd; dbgCheckEglError(launcher, __FILE__, __LINE__)
	#define EGLB_CHECK(launcher,cmd)   if((cmd) == EGL_FALSE) dbgCheckEglError(launcher, __FILE__, __LINE__)
	#define OGL_CHECK(launcher, cmd)   glGetError(); cmd; dbgCheckOglError(launcher, __FILE__, __LINE__)
	
#else
	#define EGL_CHECK(launcher,cmd)    cmd
	#define EGLB_CHECK(launcher,cmd)   cmd
	#define OGL_CHECK(launcher, cmd)   cmd
#endif

#endif
