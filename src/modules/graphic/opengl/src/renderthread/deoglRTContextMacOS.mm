/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2015, Plüss Roland ( roland@rptd.ch )
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deoglRTContext.h"
#include "deoglRenderThread.h"
#include "deoglRTLogger.h"
#include "../deGraphicOpenGl.h"
#include "../configuration/deoglConfiguration.h"
#include "../extensions/deoglExtensions.h"
#include "../window/deoglRenderWindow.h"
#include "../window/deoglRRenderWindow.h"

#include <dragengine/app/deOSMacOS.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/rendering/deRenderWindow.h>

#import <Cocoa/Cocoa.h>



// MacOS Specific deoglRTContext
//////////////////////////////////

void deoglRTContext::pCreateGLContext(){
	// NOTE called from the render thread after starting initialization. do here everything required
	//      to be linked to the render thread after prerequisited have been done in the main thread
	deoglRTLogger &logger = pRenderThread.GetLogger();
	
	// create render context
	logger.LogInfo( "Creating OpenGL Context" );
	
	NSOpenGLPixelFormatAttribute contextAttribs[] = {
		NSOpenGLPFADoubleBuffer,
		NSOpenGLPFAAccelerated,
		NSOpenGLPFAColorSize, 24,
		NSOpenGLPFAAlphaSize, 8,
		NSOpenGLPFADepthSize, 24,
		// macos is very restrictive. there exist only 2 possible profiles to use:
		// - NSOpenGLProfileVersion3_2Core
		// - NSOpenGLProfileVersion4_1Core
		// we use 3.3 core which macos has no direct constant for. example code uses 3.2 .
		// there is no documentation for 4.1 so i am not sure if this is viable. for the
		// time being sticking to 3.2 should work but can lead to problems
		NSOpenGLPFAOpenGLProfile, NSOpenGLProfileVersion3_2Core,
		0 };
	
	if( pRenderThread.GetConfiguration().GetDebugContext() ){
		logger.LogInfo( "Creating debug context not supported" );
		// contextAttribs[ contextAttribCount++ ] = GLX_CONTEXT_FLAGS_ARB;
		// contextAttribs[ contextAttribCount++ ] = GLX_CONTEXT_DEBUG_BIT_ARB;
	}
	
	pPixelFormat = [[NSOpenGLPixelFormat alloc] initWithAttributes:contextAttribs];
	if( ! pPixelFormat ){
		logger.LogError( "Failed creating pixel format!" );
		DETHROW( deeInvalidAction );
	}
	
	pContext = [[NSOpenGLContext alloc] initWithFormat:pPixelFormat shareContext:nil];
	if( ! pContext ){
		logger.LogInfo( "Failed creating OpenGL Context" );
		DETHROW( deeInvalidAction );
	}
	
	// mac os example states this can be used to force a crash on opengl calls not supported
	// in core profile. otherwise invalid operation is called.
	// CGLEnable([pContext CGLContextObj], kCGLCECrashOnRemovedFunctions);
	
	// some retina stuff code. no idea what it is supposed to do
	// [view setWantsBestResolutionOpenGLSurface:YES];
	
	// synchronize buffer swaps with vertical refresh rate
	GLint swapInt = 1;
	[pContext setValues:&swapInt forParameter:NSOpenGLCPSwapInterval];
}

void deoglRTContext::pGLContextMakeCurrent( NSView *view ){
	if( view ){
		[pContext makeCurrentContext];
		[pContext setView:view];
		
	}else{
		[pContext setView:NULL];
		[NSOpenGLContext clearCurrentContext];
	}
}

void deoglRTContext::pFreeContext(){
	// this is called in the render thread but during cleanup where the main thread is waiting for
	// the render thread. thus certain main thread access is allowed here
	printf("deoglRTContext::pFreeContext enter\n");
	
	if( pContext ){
		[pContext release];
		pContext = NULL;
	}
	
	if( pPixelFormat ){
		[pPixelFormat release];
		pPixelFormat = NULL;
	}

	printf("deoglRTContext::pFreeContext exit\n");
}
