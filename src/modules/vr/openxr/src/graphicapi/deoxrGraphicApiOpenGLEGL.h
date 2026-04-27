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

#ifndef _DEOXRGRAPHICAPIOPENGLEGL_H_
#define _DEOXRGRAPHICAPIOPENGLEGL_H_

#include <dragengine/dragengine_configuration.h>

#ifdef OS_UNIX_X11

#include <dlfcn.h>

#include <dragengine/deObject.h>
#include <dragengine/common/string/decString.h>

#include "../deoxrBasics.h"

class deVROpenXR;


/**
 * Graphic api OpenGL using EGL.
 */
class deoxrGraphicApiOpenGLEGL{
private:
	deVROpenXR &pOxr;
	
	void *pLibHandle;
	
	void *pFuncMakeCurrent;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create graphic api opengl. */
	explicit deoxrGraphicApiOpenGLEGL(deVROpenXR &oxr);
	
	/** Clean up instance. */
	~deoxrGraphicApiOpenGLEGL();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Shared oxr. */
	inline deVROpenXR &GetOxr() const{ return pOxr; }
	
	/** Load. */
	void Load();
	
	/** Unload. */
	void Unload();
	
	/** Make current. */
	void MakeCurrent(EGLDisplay dpy, EGLSurface draw, EGLSurface read, EGLContext ctx);
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pLoadLibrary();
	void pGetFunctions();
	void *pGetFunction(const char *name);
};

#endif
#endif
