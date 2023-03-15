/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2023, Roland Plüss (roland@rptd.ch)
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

#include "deoglLoaderThread.h"
#include "deoglRenderThread.h"
#include "deoglRTContext.h"
#include "deoglRTLogger.h"

#include <dragengine/threading/deMutexGuard.h>


// Class deoglLoaderThread
////////////////////////////

// Constructor, destructor
////////////////////////////

deoglLoaderThread::deoglLoaderThread( deoglRenderThread &renderThread ) :
pRenderThread( renderThread ),
pShutdown( false ){
}

deoglLoaderThread::~deoglLoaderThread(){
}



// Management
///////////////

void deoglLoaderThread::Run(){
	pRenderThread.GetLogger().LogInfo( "LoaderThread: Starting" );
	pInit();
	
	while( true ){
		{
		const deMutexGuard guard( pMutex );
		if( pShutdown ){
			break;
		}
		}
		
		try{
			pSemaphore.Wait();
			
			// TODO
			
		}catch( const deException &e ){
			pRenderThread.GetLogger().LogException( e );
		}
	}
	
	pCleanUp();
	pRenderThread.GetLogger().LogInfo( "LoaderThread: Exiting" );
}

void deoglLoaderThread::Shutdown(){
	if( ! IsRunning() ){
		return;
	}
	
	pShutdown = true;
	pSemaphore.Signal();
	
	WaitForExit();
}



// Private Functions
//////////////////////

void deoglLoaderThread::pInit(){
	deoglRTContext &context = pRenderThread.GetContext();
	
	#if defined OS_UNIX && ! defined ANDROID && ! defined OS_BEOS && ! defined OS_MACOS
		OGLX_CHECK( pRenderThread, glXMakeCurrent( context.GetDisplay(),
			/*rrenderWindow->GetWindow()*/ None, context.GetLoaderContext() ) );
	#endif
	
	#ifdef ANDROID
	if( eglMakeCurrent( context.GetDisplay(), pSurface, pSurface, context.GetLoaderContext() ) == EGL_FALSE ){
		DETHROW( deeInvalidParam );
	}
	#endif
	
	#ifdef OS_BEOS
	rrenderWindow->GetGLView()->LockGL();
	#endif
	
	#ifdef OS_MACOS
	pGLContextMakeCurrent( rrenderWindow->GetView() );
	#endif
	
	#ifdef OS_W32
	if( ! wglMakeCurrent( rrenderWindow->GetWindowDC(), context.GetLoaderContext() ) ){
		pRenderThread.GetLogger().LogErrorFormat( "wglMakeCurrent failed (%s:%i): error=0x%lx\n",
			__FILE__, __LINE__, GetLastError() );
		DETHROW_INFO( deeInvalidAction, "wglMakeCurrent failed" );
	}
	#endif
}

void deoglLoaderThread::pCleanUp(){
	deoglRTContext &context = pRenderThread.GetContext();
	
	#if defined OS_UNIX && ! defined ANDROID && ! defined OS_BEOS && ! defined OS_MACOS
		OGLX_CHECK( pRenderThread, glXMakeCurrent( context.GetDisplay(), None, nullptr ) );
	#endif
	
	#ifdef ANDROID
		eglMakeCurrent( context.GetDisplay(), EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT );
	#endif
	
	#ifdef OS_MACOS
		pGLContextMakeCurrent( nullptr );
	#endif
	
	#ifdef OS_W32
		wglMakeCurrent( NULL, NULL );
	#endif
}
