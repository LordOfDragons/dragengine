/* 
 * Drag[en]gine OpenXR
 *
 * Copyright (C) 2022, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
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
	void *pFuncGenFramebuffers;
	void *pFuncBindFramebuffer;
	void *pFuncDeleteFramebuffers;
	void *pFuncBlitFramebuffer;
	void *pFuncFramebufferTexture2D;
	void *pFuncGetIntegerv;
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
	
	/** Get current drawable. */
	GLXDrawable GetCurrentDrawable();
	
	/** Make current. */
	void MakeCurrent( Display *dpy, GLXDrawable drawable, GLXContext ctx );
	
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pLoadLibrary();
	void pGetFunctions();
	void *pGetFunction( const char *name );
};

#endif

