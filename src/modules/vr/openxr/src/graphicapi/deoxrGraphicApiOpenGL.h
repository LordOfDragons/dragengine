/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
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

#ifndef _DEOXRGRAPHICAPIOPENGL_H_
#define _DEOXRGRAPHICAPIOPENGL_H_

#include <dragengine/dragengine_configuration.h>

#ifdef OS_BEOS
#include <kernel/image.h>
#endif

#ifdef HAS_LIB_DL
#include <dlfcn.h>
#endif

#ifdef OS_W32
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0502
#endif
#include <dragengine/app/include_windows.h>
#endif

#include <dragengine/deObject.h>
#include <dragengine/common/string/decString.h>

#include "../deoxrBasics.h"

class deVROpenXR;

typedef unsigned int GAOglFbo;
typedef unsigned int GAOglImage;

/**
 * Oxr instance.
 */
class deoxrGraphicApiOpenGL{
public:
	class Framebuffer : public deObject{
	public:
		typedef deTObjectReference<Framebuffer> Ref;
		
		Framebuffer( deoxrGraphicApiOpenGL &gaogl, unsigned int image );
		
	protected:
		virtual ~Framebuffer();
		
	private:
		deoxrGraphicApiOpenGL &pGAOgl;
		unsigned int pFBO;
	};
	friend class Framebuffer;
	
private:
	deVROpenXR &pOxr;
	
	#ifdef OS_BEOS
	image_id pLibHandle;
	#endif
	
	#ifdef HAS_LIB_DL
	void *pLibHandle;
	#endif
	
	#ifdef OS_W32
	HMODULE pLibHandle;
	#endif
	
	void *pFuncGetCurrentDrawable;
	void *pFuncMakeCurrent;
	void *pFuncGetIntegerv;
	void *pFuncEnable;
	void *pFuncDisable;
	void *pFuncIsEnabled;
	
	void *pFuncGenFramebuffers;
	void *pFuncBindFramebuffer;
	void *pFuncDeleteFramebuffers;
	void *pFuncBlitFramebuffer;
	void *pFuncFramebufferTexture2D;
	void *pFuncDrawBuffers;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create graphic api opengl. */
	deoxrGraphicApiOpenGL( deVROpenXR &oxr );
	
	/** Clean up instance. */
	~deoxrGraphicApiOpenGL();
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
	
#ifdef OS_UNIX_X11
	/** Get current drawable. */
	GLXDrawable GetCurrentDrawable();
	
	/** Make current. */
	void MakeCurrent( Display *dpy, GLXDrawable drawable, GLXContext ctx );
	
#elif defined OS_W32
	/** Make current. */
	void MakeCurrent( HDC hDc, HGLRC context );
#endif
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pLoadLibrary();
	void pGetFunctions();
	void *pGetFunction( const char *name );
	void pEnable( uint32_t capability, bool enable );
};

#endif

