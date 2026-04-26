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

#ifdef OS_MACOS

#include "deoglRTCBMacOS.h"

#include <dragengine/app/deOSMacOS.h>

#include "../deoglRTContext.h"
#include "../deoglRenderThread.h"
#include "../deoglRTLogger.h"
#include "../../deGraphicOpenGl.h"
#include "../../configuration/deoglConfiguration.h"
#include "../../extensions/macosfix.h"
#include "../../window/deoglRRenderWindow.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/parallel/deParallelProcessing.h>
#include <dragengine/resources/rendering/deRenderWindow.h>

#import <Cocoa/Cocoa.h>


// Class deoglRTCBMacOS
/////////////////////////

// Constructor, destructor
////////////////////////////

deoglRTCBMacOS::deoglRTCBMacOS(deoglRTContext &context) :
deoglRTCBackend(context),
pOSMacOS(context.GetRenderThread().GetOgl().GetOS()->CastToOSMacOS()),
pPixelFormat(nullptr),
pContext(nullptr),
pLoaderContext(nullptr)
{
	int i;
	for(i=0; i<MaxCompileContextCount; i++){
		pCompileContext[i] = nullptr;
	}
}

deoglRTCBMacOS::~deoglRTCBMacOS() = default;


// Management
///////////////

void deoglRTCBMacOS::InitPhase2(){
	pCreateContext();
}

void deoglRTCBMacOS::AssignOSWindow(deRenderWindow *renderWindow){
	pOSMacOS->SetWindow(renderWindow->GetWindow());
}

void deoglRTCBMacOS::CleanUp(){
	pOSMacOS->SetWindow(nullptr);
	pFreeContext();
}

void deoglRTCBMacOS::ActivateContext(deoglRRenderWindow *window){
	pContextMakeCurrent(window->GetView());
}

void deoglRTCBMacOS::DeactivateContext(){
	pContextMakeCurrent(nullptr);
}

bool deoglRTCBMacOS::HasLoaderContext() const{
	return pLoaderContext != nullptr;
}

void *deoglRTCBMacOS::GetFunctionPointer(const char *funcName){
	return (void*)macosGetProcAddress(funcName);
}

void deoglRTCBMacOS::ActivateLoaderContext(){
	DEASSERT_NOTNULL(pLoaderContext)
	
	[pLoaderContext makeCurrentContext];
	pContextMakeCurrent(pRTContext.GetActiveRRenderWindow()->GetView());
}

void deoglRTCBMacOS::DeactivateLoaderContext(){
	DEASSERT_NOTNULL(pLoaderContext)
	
	pContextMakeCurrent(nullptr);
}

void deoglRTCBMacOS::ActivateCompileContext(unsigned int index){
	DEASSERT_TRUE(index >= 0)
	DEASSERT_TRUE(index < MaxCompileContextCount)
	DEASSERT_NOTNULL(pCompileContext[index])
	
	[pCompileContext[index] makeCurrentContext];
	pContextMakeCurrent(pRTContext.GetActiveRRenderWindow()->GetView());
}

void deoglRTCBMacOS::DeactivateCompileContext(unsigned int index){
	pContextMakeCurrent(nullptr);
}

void deoglRTCBMacOS::SwapBuffers(deoglRRenderWindow &window){
	[pContext flushBuffer];
	[pView setNeedsDisplay:YES];
}


// Protected Functions
////////////////////////

void deoglRTCBMacOS::pCreateContext(){
	// NOTE called from the render thread after starting initialization.
	//      do here everything required to be linked to the render thread
	//      after prerequisited have been done in the main thread
	deoglRTLogger &logger = pRTContext.GetRenderThread().GetLogger();
	
	// create render context
	logger.LogInfo("Creating OpenGL Context");
	
	NSOpenGLPixelFormatAttribute contextAttribs[] = {
		NSOpenGLPFADoubleBuffer,
		NSOpenGLPFAAccelerated,
		NSOpenGLPFAColorSize, 24,
		NSOpenGLPFAAlphaSize, 8,
		NSOpenGLPFADepthSize, 24,
		// macos is very restrictive. there exist only 2 possible profiles to use:
		// - NSOpenGLProfileVersion3_2Core
		// - NSOpenGLProfileVersion4_1Core
		// we use 3.3 core which macos has no direct constant for. example code uses 3.2.
		// there is no documentation for 4.1 so i am not sure if this is viable. for the
		// time being sticking to 3.2 should work but can lead to problems
		NSOpenGLPFAOpenGLProfile, NSOpenGLProfileVersion3_2Core,
		0};
	
	if(pRTContext.GetRenderThread().GetConfiguration().GetDebugContext()){
		logger.LogInfo("Creating debug context not supported");
		// contextAttribs[ contextAttribCount++ ] = GLX_CONTEXT_FLAGS_ARB;
		// contextAttribs[ contextAttribCount++ ] = GLX_CONTEXT_DEBUG_BIT_ARB;
	}
	
	pPixelFormat = [[NSOpenGLPixelFormat alloc] initWithAttributes:contextAttribs];
	if(!pPixelFormat){
		logger.LogError("Failed creating pixel format!");
		DETHROW(deeInvalidAction);
	}
	
	pContext = [[NSOpenGLContext alloc] initWithFormat:pPixelFormat shareContext:nil];
	if(!pContext){
		logger.LogInfo("Failed creating OpenGL Context");
		DETHROW(deeInvalidAction);
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

void deoglRTCBMacOS::pContextMakeCurrent(NSView *view){
	if(view){
		[pContext makeCurrentContext];
		[pContext setView:view];
		
	}else{
		[pContext setView:nullptr];
		[NSOpenGLContext clearCurrentContext];
	}
}

void deoglRTCBMacOS::pFreeContext(){
	// this is called in the render thread but during cleanup where the main thread is waiting for
	// the render thread. thus certain main thread access is allowed here
	printf("deoglRTContext::pFreeContext enter\n");
	
	if(pContext){
		[pContext release];
		pContext = nullptr;
	}
	
	if(pPixelFormat){
		[pPixelFormat release];
		pPixelFormat = nullptr;
	}
	
	printf("deoglRTContext::pFreeContext exit\n");
}

#endif // OS_MACOS
