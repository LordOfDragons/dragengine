/*
 * MIT License
 *
 * Copyright (C) 2026, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <dragengine/dragengine_configuration.h>

#ifdef OS_UNIX_X11

#include "deoxrGraphicApiOpenGLEGL.h"
#include "../deVROpenXR.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/systems/modules/deBaseModule.h>


typedef unsigned int EGLBoolean;

typedef EGLBoolean (*PFNEGLMAKECURRENTPROC)(EGLDisplay dpy, EGLSurface draw, EGLSurface read, EGLContext ctx);


// class deoxrGraphicApiOpenGLEGL
///////////////////////////////////

deoxrGraphicApiOpenGLEGL::deoxrGraphicApiOpenGLEGL(deVROpenXR &oxr) :
pOxr(oxr),
pLibHandle(nullptr),
pFuncMakeCurrent(nullptr){
}

deoxrGraphicApiOpenGLEGL::~deoxrGraphicApiOpenGLEGL(){
	pCleanUp();
}


// Management
///////////////

void deoxrGraphicApiOpenGLEGL::Load(){
	if(pLibHandle){
		return;
	}
	
	pLoadLibrary();
	pGetFunctions();
}

void deoxrGraphicApiOpenGLEGL::Unload(){
	if(!pLibHandle){
		return;
	}
	
	dlclose(pLibHandle);
	pLibHandle = nullptr;
}

void deoxrGraphicApiOpenGLEGL::MakeCurrent(EGLDisplay dpy, EGLSurface draw, EGLSurface read, EGLContext ctx){
	DEASSERT_NOTNULL(pFuncMakeCurrent)
	((PFNEGLMAKECURRENTPROC)pFuncMakeCurrent)(dpy, draw, read, ctx);
}


// Private Functions
//////////////////////

void deoxrGraphicApiOpenGLEGL::pCleanUp(){
	Unload();
}

void deoxrGraphicApiOpenGLEGL::pLoadLibrary(){
	pOxr.LogInfo("deoxrGraphicApiOpenGLEGL: Try loading libEGL.so");
	pLibHandle = dlopen("libEGL.so", RTLD_NOW);
	if(pLibHandle){
		pOxr.LogInfo("deoxrGraphicApiOpenGLEGL: Loading libEGL.so succeeded");
		
	}else{
		pOxr.LogErrorFormat("deoxrGraphicApiOpenGLEGL: dlerror: %s.", dlerror());
		
		pOxr.LogInfo("deoxrGraphicApiOpenGLEGL: Try loading libEGL.so.1");
		pLibHandle = dlopen("libEGL.so.1", RTLD_NOW);
		
		if(pLibHandle){
			pOxr.LogInfo("deoxrGraphicApiOpenGLEGL: Loading libEGL.so.1 succeeded");
		}
	}
	
	if(!pLibHandle){
		pOxr.LogErrorFormat("deoxrGraphicApiOpenGLEGL: dlerror: %s.", dlerror());
		DETHROW_INFO(deeInvalidAction, "Load OpenGL library failed");
	}
}

void deoxrGraphicApiOpenGLEGL::pGetFunctions(){
	pFuncMakeCurrent = pGetFunction("eglMakeCurrent");
}

void *deoxrGraphicApiOpenGLEGL::pGetFunction(const char *name){
	void *func = dlsym(pLibHandle, name);
	if(!func){
		DETHROW_INFO(deeInvalidAction, decString("Function not found: ") + name);
	}
	
	return func;
}

#endif
