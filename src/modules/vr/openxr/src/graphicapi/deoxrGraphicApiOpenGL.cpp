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
typedef unsigned char GLboolean;
typedef int GLsizei;
typedef int GLint;

#define GL_FRONT 0x0404
#define GL_BACK 0x0405

#define GL_FRAMEBUFFER_BINDING 0x8CA6
#define GL_FRAMEBUFFER 0x8D40
#define GL_READ_FRAMEBUFFER 0x8CA8
#define GL_DRAW_FRAMEBUFFER 0x8CA9
#define GL_COLOR_ATTACHMENT0 0x8CE0
#define GL_TEXTURE_2D 0x0DE1

#ifdef OS_ANDROID
	// nothing
#elif defined OS_UNIX
	typedef GLXDrawable ( *PFNGLXGETCURRENTDRAWABLE )();
	typedef Bool ( *PFNGLXMAKECURRENT )( Display *dpy, GLXDrawable drawable, GLXContext ctx );
#elif defined OS_W32
	typedef BOOL ( *PFNWGLMAKECURRENTPROC )( HDC hDc, HGLRC newContext );
#endif
typedef void ( *PFNGLGET )( GLenum mode );
typedef void ( *PFNGLENABLE )( GLenum cap );
typedef void ( *PFNGLDISABLE )( GLenum cap );
typedef GLboolean ( *PFNGLISENABLED )( GLenum cap );

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
pFuncGetIntegerv( nullptr ),
pFuncEnable( nullptr ),
pFuncDisable( nullptr ),
pFuncIsEnabled( nullptr ),

pFuncGenFramebuffers( nullptr ),
pFuncBindFramebuffer( nullptr ),
pFuncDeleteFramebuffers( nullptr ),
pFuncBlitFramebuffer( nullptr ),
pFuncFramebufferTexture2D( nullptr ),
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

#ifdef OS_ANDROID
	// nothing

#elif defined OS_UNIX
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

#elif defined OS_W32
void deoxrGraphicApiOpenGL::MakeCurrent( HDC hDc, HGLRC context ){
	if( ! pFuncMakeCurrent ){
		DETHROW( deeInvalidParam );
	}
	( ( PFNWGLMAKECURRENTPROC )pFuncMakeCurrent )( hDc, context );
}
#endif



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
	pLibHandle = LoadLibrary( L"OpenGL32" );
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
	#ifdef OS_UNIX
		pFuncGetCurrentDrawable = pGetFunction( "glXGetCurrentDrawable" );
		pFuncMakeCurrent = pGetFunction( "glXMakeCurrent" );
	#elif defined OS_W32
		pFuncMakeCurrent = pGetFunction( "wglMakeCurrent" );
	#endif
	pFuncGetIntegerv = pGetFunction( "glGetIntegerv" );
	pFuncEnable = pGetFunction( "glEnable" );
	pFuncDisable = pGetFunction( "glDisable" );
	pFuncIsEnabled = pGetFunction( "glIsEnabled" );
	
// 	pFuncGenFramebuffers = pGetFunction( "glGenFramebuffers" );
// 	pFuncBindFramebuffer = pGetFunction( "glBindFramebuffer" );
// 	pFuncDeleteFramebuffers = pGetFunction( "glDeleteFramebuffers" );
// 	pFuncBlitFramebuffer = pGetFunction( "glBlitFramebuffer" );
// 	pFuncFramebufferTexture2D = pGetFunction( "glFramebufferTexture2D" );
// 	pFuncDrawBuffers = pGetFunction( "glDrawBuffers" );
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
	func = ( LPVOID )GetProcAddress( pLibHandle, name );
#endif
	
	if( ! func ){
		DETHROW_INFO( deeInvalidAction, decString( "Function not found: " ) + name );
	}
	
	return func;
}

void deoxrGraphicApiOpenGL::pEnable( uint32_t capability, bool enable ){
	if( enable ){
		if( ! pFuncEnable ){
			DETHROW( deeInvalidParam );
		}
		( ( PFNGLENABLE )pFuncEnable )( capability );
		
	}else{
		if( ! pFuncDisable ){
			DETHROW( deeInvalidParam );
		}
		( ( PFNGLDISABLE )pFuncDisable )( capability );
	}
}
