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

#ifdef OS_ANDROID

#include "deoglRTCBAndroid.h"

#include <dragengine/app/deOSAndroid.h>
#include <android/native_window.h>

#include "../deoglRTContext.h"
#include "../deoglRenderThread.h"
#include "../deoglRTLogger.h"
#include "../deoglLoaderThread.h"
#include "../../deGraphicOpenGl.h"
#include "../../configuration/deoglConfiguration.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/parallel/deParallelProcessing.h>
#include <dragengine/resources/rendering/deRenderWindow.h>


// Class deoglRTCBAndroid
///////////////////////////

// Constructor, destructor
////////////////////////////

deoglRTCBAndroid::deoglRTCBAndroid(deoglRTContext &context) :
deoglRTCBackend(context),
pOSAndroid(context.GetRenderThread().GetOgl().GetOS()->CastToOSAndroid()),
pDisplay(EGL_NO_DISPLAY),
pSurface(EGL_NO_SURFACE),
pContext(EGL_NO_CONTEXT),
pLoaderSurface(EGL_NO_SURFACE),
pLoaderContext(EGL_NO_CONTEXT),
pScreenWidth(0),
pScreenHeight(0)
{
	int i;
	for(i=0; i<MaxCompileContextCount; i++){
		pCompileSurface[i] = EGL_NO_SURFACE;
		pCompileContext[i] = EGL_NO_CONTEXT;
	}
}

deoglRTCBAndroid::~deoglRTCBAndroid() = default;


// Management
///////////////

void deoglRTCBAndroid::InitPhase2(){
	pInitDisplay();
}

void deoglRTCBAndroid::CleanUp(){
	pCloseDisplay();
}

void deoglRTCBAndroid::DropCompileContexts(int count){
	deoglRTLogger &logger = pRTContext.GetRenderThread().GetLogger();
	int i;
	for(i=count; i<pCompileContextCount; i++){
		logger.LogInfoFormat("Drop compile context %d", i);
		if(pCompileContext[i] != EGL_NO_CONTEXT){
			eglDestroyContext(pDisplay, pCompileContext[i]);
			pCompileContext[i] = EGL_NO_CONTEXT;
		}
		if(pCompileSurface[i] != EGL_NO_SURFACE){
			eglDestroySurface(pDisplay, pCompileSurface[i]);
			pCompileSurface[i] = EGL_NO_SURFACE;
		}
	}
	pCompileContextCount = count;
}

void deoglRTCBAndroid::ActivateContext(deoglRRenderWindow&){
	if(eglMakeCurrent(pDisplay, pSurface, pSurface, pContext) == EGL_FALSE){
		DETHROW(deeInvalidParam);
	}
	if(pLoaderContext != EGL_NO_CONTEXT){
		pRTContext.GetRenderThread().GetLoaderThread().EnableContext(true);
	}
}

void deoglRTCBAndroid::DeactivateContext(){
	if(eglMakeCurrent(pDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT) == EGL_FALSE){
		DETHROW(deeInvalidParam);
	}
}

bool deoglRTCBAndroid::HasLoaderContext() const{
	return pLoaderContext != EGL_NO_CONTEXT;
}

void *deoglRTCBAndroid::GetFunctionPointer(const char *funcName){
	return (void*)eglGetProcAddress(funcName);
}

void deoglRTCBAndroid::ActivateLoaderContext(){
	DEASSERT_FALSE(pLoaderContext == EGL_NO_CONTEXT)
	DEASSERT_FALSE(pLoaderSurface == EGL_NO_SURFACE)
	
	DEASSERT_TRUE(eglMakeCurrent(pDisplay, pLoaderSurface, pLoaderSurface, pLoaderContext) == EGL_TRUE)
}

void deoglRTCBAndroid::DeactivateLoaderContext(){
	DEASSERT_TRUE(eglMakeCurrent(pDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT) == EGL_TRUE)
}

void deoglRTCBAndroid::ActivateCompileContext(int index){
	DEASSERT_TRUE(index >= 0)
	DEASSERT_TRUE(index < MaxCompileContextCount)
	DEASSERT_FALSE(pCompileContext[index] == EGL_NO_CONTEXT)
	DEASSERT_FALSE(pCompileSurface[index] == EGL_NO_SURFACE)
	
	DEASSERT_TRUE(eglMakeCurrent(pDisplay, pCompileSurface[index],
		pCompileSurface[index], pCompileContext[index]) == EGL_TRUE)
}

void deoglRTCBAndroid::DeactivateCompileContext(int){
	DEASSERT_TRUE(eglMakeCurrent(pDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT) == EGL_TRUE)
}

void deoglRTCBAndroid::SwapBuffers(deoglRRenderWindow&){
	if(eglSwapBuffers(pDisplay, pSurface) == EGL_FALSE){
		// log failure but do not thow exception
	}
}


void deoglRTCBAndroid::InitAppWindow(){
	pInitDisplay();
}

void deoglRTCBAndroid::TerminateAppWindow(){
	if(pDisplay == EGL_NO_DISPLAY){
		return;
	}
	if(pSurface == EGL_NO_SURFACE){
		return;
	}
	
	pRTContext.GetRenderThread().GetLogger().LogInfo("Destroy surface (close window)");
	eglMakeCurrent(pDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
	
	eglDestroySurface(pDisplay, pSurface);
	pSurface = EGL_NO_SURFACE;
}

void deoglRTCBAndroid::CheckConfigurationChanged(){
	const decBoundary &contentRect = pOSAndroid->GetContentRect();
	
	const int width = contentRect.x2 - contentRect.x1;
	const int height = contentRect.y2 - contentRect.y1;
	
	if(width > 0 || height > 0){
		pScreenWidth = width;
		pScreenHeight = height;
		
	}else{
		// initial situation where content area is not known yet. use screen instead
		eglQuerySurface(pDisplay, pSurface, EGL_WIDTH, &pScreenWidth);
		eglQuerySurface(pDisplay, pSurface, EGL_HEIGHT, &pScreenHeight);
		
		//pScreenWidth = (int)ANativeWindow_getWidth(pOSAndroid->GetNativeWindow());
		//pScreenHeight = (int)ANativeWindow_getHeight(pOSAndroid->GetNativeWindow());
	}
}


// Protected Functions
////////////////////////

void deoglRTCBAndroid::pInitDisplay(){
	if(pSurface != EGL_NO_SURFACE){
		return;
	}
	
	deoglRTLogger &logger = pRTContext.GetRenderThread().GetLogger();
	
	DEASSERT_NOTNULL(pOSAndroid->GetNativeWindow())
	
	// initialize display
	if(pDisplay == EGL_NO_DISPLAY){
		pRTContext.GetRenderThread().GetLogger().LogInfo("Init display");
		pDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
		if(pDisplay == EGL_NO_DISPLAY){
			DETHROW(deeInvalidParam);
		}
		
		eglInitialize(pDisplay, 0, 0);
		
		// choose configuration
		const EGLint attribs[] = {
			EGL_RENDERABLE_TYPE, EGL_OPENGL_ES3_BIT,
			EGL_SURFACE_TYPE, EGL_WINDOW_BIT | EGL_PBUFFER_BIT,
			EGL_BLUE_SIZE, 8,
			EGL_GREEN_SIZE, 8,
			EGL_RED_SIZE, 8,
			EGL_DEPTH_SIZE, 16,
			EGL_NONE
		};
		EGLint configCount;
		eglChooseConfig(pDisplay, attribs, &pConfig, 1, &configCount);
		if(configCount == 0){
			DETHROW(deeInvalidParam);
		}
	}
	
	// set buffer geometry
	/*
	EGLint format;
	eglGetConfigAttrib(pDisplay, pConfig, EGL_NATIVE_VISUAL_ID, &format);
	ANativeWindow_setBuffersGeometry(pOSAndroid->GetNativeWindow(), 0, 0, format);
	*/
	
	// create surface
	pSurface = eglCreateWindowSurface(pDisplay, pConfig, pOSAndroid->GetNativeWindow(), nullptr);
	if(pSurface == EGL_NO_SURFACE){
#ifdef OS_ANDROID_QUEST
		// happens if VR-Mode is enabled. in this case we need a small off-screen surface
		logger.LogInfo("Window surface already connected (VR-Mode). Creating tiny PBuffer surface");
		const EGLint eglBufferAttribList[]{
			EGL_WIDTH, 16,
			EGL_HEIGHT, 16,
			EGL_NONE
		};
		
		pSurface = eglCreatePbufferSurface(pDisplay, pConfig, eglBufferAttribList);
		DEASSERT_FALSE(pSurface == EGL_NO_SURFACE)
#else
		DETHROW(deeInvalidParam);
#endif
	}
	
	// create context if not existing. it can be kept around while frozen
	const int compileContextCount = decMath::min(MaxCompileContextCount,
		pRTContext.GetRenderThread().GetOgl().GetGameEngine()->GetParallelProcessing().GetCoreCount());
	
	if(pContext == EGL_NO_CONTEXT){
		//EGLint contextFlags = 0;
		EGLint valUseDebugContext = EGL_FALSE;
		if(pRTContext.GetRenderThread().GetConfiguration().GetDebugContext()){
			logger.LogInfo("Enable debug context");
			//contextFlags |= EGL_CONTEXT_OPENGL_DEBUG_BIT_KHR;
			valUseDebugContext = EGL_TRUE;
		}
		
		EGLint eglAttribList[] = {
			EGL_CONTEXT_MAJOR_VERSION, 3,
			EGL_CONTEXT_MINOR_VERSION, 0,
			//EGL_CONTEXT_OPENGL_PROFILE_MASK, EGL_CONTEXT_OPENGL_CORE_PROFILE_BIT,
			// ^== fails on meta quest
			EGL_CONTEXT_OPENGL_DEBUG, valUseDebugContext,
			EGL_NONE, EGL_NONE,
			EGL_NONE};
		
		pContext = eglCreateContext(pDisplay, pConfig, EGL_NO_CONTEXT, eglAttribList);
		DEASSERT_FALSE(pContext == EGL_NO_CONTEXT)
		
		// loader context needs also an own surface
		const EGLint eglBufferAttribList[] = {
			EGL_WIDTH, 8,
			EGL_HEIGHT, 8,
			EGL_LARGEST_PBUFFER, EGL_TRUE,
			EGL_TEXTURE_FORMAT, EGL_TEXTURE_RGB,
			EGL_TEXTURE_TARGET, EGL_TEXTURE_2D,
			EGL_NONE
		};
		
		pLoaderSurface = eglCreatePbufferSurface(pDisplay, pConfig, eglBufferAttribList);
		DEASSERT_FALSE(pLoaderSurface == EGL_NO_SURFACE)
		
		pLoaderContext = eglCreateContext(pDisplay, pConfig, pContext, eglAttribList);
		DEASSERT_FALSE(pLoaderContext == EGL_NO_CONTEXT)
		
		int i;
		for(i=0; i<compileContextCount; i++){
			pCompileContext[i] = eglCreateContext(pDisplay, pConfig, pContext, eglAttribList);
			if(pCompileContext[i] == EGL_NO_CONTEXT){
				break;
			}
			
			pCompileSurface[i] = eglCreatePbufferSurface(pDisplay, pConfig, eglBufferAttribList);
			if(pCompileSurface[i] == EGL_NO_SURFACE){
				eglDestroyContext(pDisplay, pCompileContext[i]);
				pCompileContext[i] = EGL_NO_CONTEXT;
				break;
			}
		}
		pCompileContextCount = i;
		pRTContext.GetRenderThread().GetLogger().LogInfoFormat(
			"Created %d compile contexts", pCompileContextCount);
	}
	
	// make surface current. we have to make it current each render loop
	if(eglMakeCurrent(pDisplay, pSurface, pSurface, pContext) == EGL_FALSE){
		DETHROW(deeInvalidParam);
	}
	
	// query display parameters
	eglQuerySurface(pDisplay, pSurface, EGL_WIDTH, &pScreenWidth);
	eglQuerySurface(pDisplay, pSurface, EGL_HEIGHT, &pScreenHeight);
	pRTContext.GetRenderThread().GetLogger().LogInfoFormat(
		"Display size %dx%d", pScreenWidth, pScreenHeight);
}

void deoglRTCBAndroid::pCloseDisplay(){
	if(pDisplay == EGL_NO_DISPLAY){
		return;
	}
	
	TerminateAppWindow();
	
	int i;
	for(i=0; i<pCompileContextCount; i++){
		if(pCompileContext[i] != EGL_NO_CONTEXT){
			eglDestroyContext(pDisplay, pCompileContext[i]);
			pCompileContext[i] = EGL_NO_CONTEXT;
		}
		if(pCompileSurface[i] != EGL_NO_SURFACE){
			eglDestroySurface(pDisplay, pCompileSurface[i]);
			pCompileSurface[i] = EGL_NO_SURFACE;
		}
	}
	
	if(pLoaderContext != EGL_NO_CONTEXT){
		eglDestroyContext(pDisplay, pLoaderContext);
		pLoaderContext = EGL_NO_CONTEXT;
	}
	if(pLoaderSurface != EGL_NO_SURFACE){
		eglDestroySurface(pDisplay, pLoaderSurface);
		pLoaderSurface = EGL_NO_SURFACE;
	}
	
	if(pContext != EGL_NO_CONTEXT){
		eglDestroyContext(pDisplay, pContext);
		pContext = EGL_NO_CONTEXT;
	}
	
	eglTerminate(pDisplay);
	pDisplay = EGL_NO_DISPLAY;
}

#endif // OS_ANDROID
