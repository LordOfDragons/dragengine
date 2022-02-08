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

#include <stdlib.h>
#include <string.h>

#include "deoxrGraphicApiOpenGL.h"
#include "../deVROpenXR.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/systems/modules/deBaseModule.h>

#ifdef OS_W32
#include <dragengine/app/deOSWindows.h>
#endif


#define Bool int
typedef unsigned int GLuint;
typedef unsigned int GLenum;
typedef unsigned int GLbitfield;
typedef int GLsizei;
typedef int GLint;

#define GL_FRAMEBUFFER_BINDING 0x8CA6
#define GL_FRAMEBUFFER 0x8D40
#define GL_READ_FRAMEBUFFER 0x8CA8
#define GL_DRAW_FRAMEBUFFER 0x8CA9
#define GL_COLOR_ATTACHMENT0 0x8CE0
#define GL_TEXTURE_2D 0x0DE1

typedef GLXDrawable ( *PFNGLXGETCURRENTDRAWABLE )();
typedef Bool ( *PFNGLXMAKECURRENT )( Display *dpy, GLXDrawable drawable, GLXContext ctx );
typedef void ( *PFNGLBINDFRAMEBUFFERPROC )( GLenum target, GLuint framebuffer );
typedef void ( *PFNGLDELETEFRAMEBUFFERSPROC )( GLsizei n, const GLuint *framebuffers );
typedef void ( *PFNGLGENFRAMEBUFFERSPROC )( GLsizei n, GLuint *framebuffers );
typedef void ( *PFNGLBLITFRAMEBUFFERPROC )( GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1,
	GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter );
typedef void ( *PFNGLFRAMEBUFFERTEXTURE2DPROC )( GLenum target, GLenum attachment,
	GLenum textarget, GLuint texture, GLint level );
typedef void ( *PFNGLGETINTEGERV )( GLenum pname, GLint *params );
typedef void ( *PFNGLDRAWBUFFERSPROC )( GLsizei n, const GLenum *bufs );


// class deoxrGraphicApiOpenGL::Framebuffer
/////////////////////////////////////////////

deoxrGraphicApiOpenGL::Framebuffer::Framebuffer( deoxrGraphicApiOpenGL &gaogl, unsigned int image ) :
pGAOgl( gaogl ),
pFBO( 0 )
{
	GLint prevFbo;
	( ( PFNGLGETINTEGERV )pGAOgl.pFuncGetIntegerv )( GL_FRAMEBUFFER_BINDING, &prevFbo );
	
	( ( PFNGLGENFRAMEBUFFERSPROC )pGAOgl.pFuncGenFramebuffers )( 1, &pFBO );
	( ( PFNGLBINDFRAMEBUFFERPROC )pGAOgl.pFuncBindFramebuffer )( GL_FRAMEBUFFER, pFBO );
	( ( PFNGLFRAMEBUFFERTEXTURE2DPROC )pGAOgl.pFuncFramebufferTexture2D )(
		GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, image, 0 );
	const GLenum buffers = GL_COLOR_ATTACHMENT0;
	( ( PFNGLDRAWBUFFERSPROC )pGAOgl.pFuncDrawBuffers )( 1, &buffers );
	
	( ( PFNGLBINDFRAMEBUFFERPROC )pGAOgl.pFuncBindFramebuffer )( GL_FRAMEBUFFER, prevFbo );
}

deoxrGraphicApiOpenGL::Framebuffer::~Framebuffer(){
	if( pFBO ){
		( ( PFNGLDELETEFRAMEBUFFERSPROC )pGAOgl.pFuncDeleteFramebuffers )( 1, &pFBO );
	}
}


// class deoxrGraphicApiOpenGL
////////////////////////////////

deoxrGraphicApiOpenGL::deoxrGraphicApiOpenGL( deVROpenXR &oxr ) :
pOxr( oxr ),
#ifdef OS_BEOS
pLibHandle( 0 ),
#else
pLibHandle( nullptr ),
#endif
pFuncGetCurrentDrawable( nullptr ),
pFuncMakeCurrent( nullptr ),
pFuncGenFramebuffers( nullptr ),
pFuncBindFramebuffer( nullptr ),
pFuncDeleteFramebuffers( nullptr ),
pFuncBlitFramebuffer( nullptr ),
pFuncFramebufferTexture2D( nullptr ),
pFuncGetIntegerv( nullptr ),
pFuncDrawBuffers( nullptr )
{
}

deoxrGraphicApiOpenGL::~deoxrGraphicApiOpenGL(){
	pCleanUp();
}



// Management
///////////////

void deoxrGraphicApiOpenGL::Load(){
	if( pLibHandle ){
		return;
	}
	
	pLoadLibrary();
	pGetFunctions();
}

void deoxrGraphicApiOpenGL::Unload(){
	if( ! pLibHandle ){
		return;
	}
	
#ifdef OS_BEOS
	unload_add_on( pLibHandle );
	pLibHandle = 0;
	
#elif defined HAS_LIB_DL
	dlclose( pLibHandle );
	pLibHandle = nullptr;
	
#elif defined OS_W32
	FreeLibrary( pLibHandle );
	pLibHandle = nullptr;
#endif
}

GLXDrawable deoxrGraphicApiOpenGL::GetCurrentDrawable(){
	if( ! pFuncGetCurrentDrawable ){
		DETHROW( deeInvalidParam );
	}
	return ( ( PFNGLXGETCURRENTDRAWABLE )pFuncGetCurrentDrawable )();
}

void deoxrGraphicApiOpenGL::MakeCurrent( Display *dpy, GLXDrawable drawable, GLXContext ctx ){
	if( ! pFuncMakeCurrent ){
		DETHROW( deeInvalidParam );
	}
	( ( PFNGLXMAKECURRENT )pFuncMakeCurrent )( dpy, drawable, ctx );
}



// Private Functions
//////////////////////

void deoxrGraphicApiOpenGL::pCleanUp(){
	Unload();
}

void deoxrGraphicApiOpenGL::pLoadLibrary(){
#ifdef OS_BEOS
	pLibHandle = load_add_on( "opengl" );
	if( pLibHandle < 0 ){
		DETHROW_INFO( deeInvalidAction, "Load OpenGL image failed" );
	}
	
#elif defined HAS_LIB_DL
	pLibHandle = dlopen( "libGL.so", RTLD_NOW );
	if( ! pLibHandle ){
		pOxr.LogErrorFormat( "dlerror: %s.", dlerror() );
		DETHROW_INFO( deeInvalidAction, "Load OpenGL library failed" );
	}
	
#elif defined OS_W32
	pLibHandle = LoadLibrary( "OpenGL32" );
	if( ! pLibHandle ){
		int err = GetLastError();
		wchar_t messageBuffer[ 251 ];
		FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, err, MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ), // Default language
			messageBuffer, 250, NULL );
		
		pOxr.LogErrorFormat( "LoadLibrary(err=%i): %s.",
			err, deOSWindows::WideToUtf8( messageBuffer ).GetString() );
		
		DETHROW_INFO( deeInvalidAction, "Load OpenGL DLL failed" );
	}
#endif
}

void deoxrGraphicApiOpenGL::pGetFunctions(){
	pFuncGetCurrentDrawable = pGetFunction( "glXGetCurrentDrawable" );
	pFuncMakeCurrent = pGetFunction( "glXMakeCurrent" );
	pFuncGenFramebuffers = pGetFunction( "glGenFramebuffers" );
	pFuncBindFramebuffer = pGetFunction( "glBindFramebuffer" );
	pFuncDeleteFramebuffers = pGetFunction( "glDeleteFramebuffers" );
	pFuncBlitFramebuffer = pGetFunction( "glBlitFramebuffer" );
	pFuncFramebufferTexture2D = pGetFunction( "glFramebufferTexture2D" );
	pFuncGetIntegerv = pGetFunction( "glGetIntegerv" );
	pFuncDrawBuffers = pGetFunction( "glDrawBuffers" );
}

void *deoxrGraphicApiOpenGL::pGetFunction( const char *name ){
	void *func = nullptr;
#ifdef OS_BEOS
	if( get_image_symbol( pLibHandle, name, B_SYMBOL_TYPE_TEXT, &func ) != B_OK ){
		func = nullptr;
	}
	
#elif defined HAS_LIB_DL
	func = dlsym( pLibHandle, name );
	
#elif defined OS_W32
	func = GetProcAddress( pLibHandle, name );
#endif
	
	if( ! func ){
		DETHROW_INFO( deeInvalidAction, decString( "Function not found: " ) + name );
	}
	
	return func;
}
