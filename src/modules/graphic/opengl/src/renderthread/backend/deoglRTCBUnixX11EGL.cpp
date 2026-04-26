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

#if defined(OS_UNIX_X11) && defined(BACKEND_OPENGL)

#include "deoglRTCBUnixX11EGL.h"

#include <dlfcn.h>

#include "../deoglRTContext.h"
#include "../deoglRenderThread.h"
#include "../deoglRTLogger.h"
#include "../../deGraphicOpenGl.h"
#include "../../configuration/deoglConfiguration.h"
#include "../../window/deoglRRenderWindow.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/parallel/deParallelProcessing.h>


struct sOpenGlVersionUsingEGL{
	int major;
	int minor;
};
static const int vOpenGLVersionCountUsingEGL = 9;
static const sOpenGlVersionUsingEGL vOpenGLVersionsUsingEGL[vOpenGLVersionCountUsingEGL] = {
	{4, 6}, {4, 5}, {4, 4}, {4, 3}, {4, 2}, {4, 1}, {4, 0}, {3, 3}, {3, 2} };


// Class deoglRTCBUnixX11EGL
//////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRTCBUnixX11EGL::deoglRTCBUnixX11EGL(deoglRTContext &context) :
deoglRTCBUnixX11(context),
pLibHandle(nullptr),
pEGLDisplay(EGL_NO_DISPLAY),
pEGLConfig(nullptr),
pEGLContext(EGL_NO_CONTEXT),
pEGLLoaderContext(EGL_NO_CONTEXT),
pEGLLoaderSurface(EGL_NO_SURFACE),
pEglGetDisplay(nullptr),
pEglInitialize(nullptr),
pEglTerminate(nullptr),
pEglChooseConfig(nullptr),
pEglCreateWindowSurface(nullptr),
pEglCreatePbufferSurface(nullptr),
pEglDestroySurface(nullptr),
pEglCreateContext(nullptr),
pEglDestroyContext(nullptr),
pEglMakeCurrent(nullptr),
pEglGetProcAddress(nullptr),
pEglQuerySurface(nullptr),
pEglBindAPI(nullptr),
pEglSwapBuffers(nullptr),
pEglSwapInterval(nullptr)
{
	int i;
	for(i=0; i<MaxCompileContextCount; i++){
		pEGLCompileContext[i] = EGL_NO_CONTEXT;
		pEGLCompileSurface[i] = EGL_NO_SURFACE;
	}
}

deoglRTCBUnixX11EGL::~deoglRTCBUnixX11EGL(){
	CleanUp();
}


// Management
///////////////

bool deoglRTCBUnixX11EGL::TryInit(){
	deoglRTLogger &logger = pRTContext.GetRenderThread().GetLogger();
	
	// try libEGL.so.1 first, then libEGL.so
	void *handle = dlopen("libEGL.so.1", RTLD_LAZY | RTLD_LOCAL);
	if(!handle){
		handle = dlopen("libEGL.so", RTLD_LAZY | RTLD_LOCAL);
	}
	if(!handle){
		logger.LogInfo("deoglRTCBUnixX11EGL: libEGL.so not found, will try libGLX.so");
		return false;
	}
	
	// load all required EGL symbols
	pEglGetDisplay = (PFNEGLGETDISPLAYPROC)dlsym(handle, "eglGetDisplay");
	pEglInitialize = (PFNEGLINITIALIZEPROC)dlsym(handle, "eglInitialize");
	pEglTerminate = (PFNEGLTERMINATEPROC)dlsym(handle, "eglTerminate");
	pEglChooseConfig = (PFNEGLCHOOSECONFIGPROC)dlsym(handle, "eglChooseConfig");
	pEglCreateWindowSurface = (PFNEGLCREATEWINDOWSURFACEPROC)dlsym(handle, "eglCreateWindowSurface");
	pEglCreatePbufferSurface = (PFNEGLCREATEPBUFFERSURFACEPROC)dlsym(handle, "eglCreatePbufferSurface");
	pEglDestroySurface = (PFNEGLDESTROYSURFACEPROC)dlsym(handle, "eglDestroySurface");
	pEglCreateContext = (PFNEGLCREATECONTEXTPROC)dlsym(handle, "eglCreateContext");
	pEglDestroyContext = (PFNEGLDESTROYCONTEXTPROC)dlsym(handle, "eglDestroyContext");
	pEglMakeCurrent = (PFNEGLMAKECURRENTPROC)dlsym(handle, "eglMakeCurrent");
	pEglGetProcAddress = (PFNEGLGETPROCADDRESSPROC)dlsym(handle, "eglGetProcAddress");
	pEglQuerySurface = (PFNEGLQUERYSURFACEPROC)dlsym(handle, "eglQuerySurface");
	pEglBindAPI = (PFNEGLBINDAPIPROC)dlsym(handle, "eglBindAPI");
	pEglSwapBuffers = (PFNEGLSWAPBUFFERSPROC)dlsym(handle, "eglSwapBuffers");
	pEglSwapInterval = (PFNEGLSWAPINTERVALPROC)dlsym(handle, "eglSwapInterval");
	pEglGetConfigAttrib = (PFNEGLGETCONFIGATTRIBPROC)dlsym(handle, "eglGetConfigAttrib");
	
	if(!pEglGetDisplay || !pEglInitialize || !pEglTerminate || !pEglChooseConfig
	|| !pEglCreateWindowSurface || !pEglCreatePbufferSurface || !pEglDestroySurface
	|| !pEglCreateContext || !pEglDestroyContext || !pEglMakeCurrent || !pEglGetProcAddress
	|| !pEglQuerySurface || !pEglBindAPI || !pEglSwapBuffers || !pEglSwapInterval
	|| !pEglGetConfigAttrib){
		logger.LogWarn("deoglRTCBUnixX11EGL: libEGL.so found but required symbols are missing.");
		dlclose(handle);
		return false;
	}
	
	pLibHandle = handle;
	logger.LogInfo("deoglRTCBUnixX11EGL: Using EGL backend (libEGL.so)");
	return true;
}

void deoglRTCBUnixX11EGL::InitPhase2(){
	deoglRTCBUnixX11::InitPhase2();
	pCreateContext();
}

void deoglRTCBUnixX11EGL::CleanUp(){
	AssignOSWindow(nullptr);
	pFreeContext();
	pUnloadLibrary();
	deoglRTCBUnixX11::CleanUp();
}

void deoglRTCBUnixX11EGL::DropCompileContexts(int count){
	deoglRTLogger &logger = pRTContext.GetRenderThread().GetLogger();
	int i;
	
	for(i=count; i<pCompileContextCount; i++){
		logger.LogInfoFormat("Drop compile context %d", i);
		if(pEGLCompileContext[i] != EGL_NO_CONTEXT){
			pEglDestroyContext(pEGLDisplay, pEGLCompileContext[i]);
			pEGLCompileContext[i] = EGL_NO_CONTEXT;
		}
		if(pEGLCompileSurface[i] != EGL_NO_SURFACE){
			pEglDestroySurface(pEGLDisplay, pEGLCompileSurface[i]);
			pEGLCompileSurface[i] = EGL_NO_SURFACE;
		}
	}
	
	pCompileContextCount = count;
}

void deoglRTCBUnixX11EGL::ActivateContext(deoglRRenderWindow &window){
	DEASSERT_FALSE(window.GetEGLSurface() == EGL_NO_SURFACE)
	
	if(pEglMakeCurrent(pEGLDisplay, window.GetEGLSurface(), window.GetEGLSurface(), pEGLContext) == EGL_FALSE){
		pRTContext.GetRenderThread().GetLogger().LogError("eglMakeCurrent failed");
		DETHROW(deeInvalidParam);
	}
}

void deoglRTCBUnixX11EGL::DeactivateContext(){
	pEglMakeCurrent(pEGLDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
}

bool deoglRTCBUnixX11EGL::HasLoaderContext() const{
	return pEGLLoaderContext != EGL_NO_CONTEXT;
}

void *deoglRTCBUnixX11EGL::GetFunctionPointer(const char *funcName){
	return (void*)pEglGetProcAddress(funcName);
}

void deoglRTCBUnixX11EGL::ActivateLoaderContext(){
	DEASSERT_FALSE(pEGLLoaderContext == EGL_NO_CONTEXT)
	DEASSERT_FALSE(pEGLLoaderSurface == EGL_NO_SURFACE)
	
	DEASSERT_TRUE(pEglMakeCurrent(pEGLDisplay, pEGLLoaderSurface, pEGLLoaderSurface, pEGLLoaderContext));
}

void deoglRTCBUnixX11EGL::DeactivateLoaderContext(){
	DEASSERT_TRUE(pEglMakeCurrent(pEGLDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT));
}

void deoglRTCBUnixX11EGL::ActivateCompileContext(int index){
	DEASSERT_TRUE(index >= 0)
	DEASSERT_TRUE(index < MaxCompileContextCount)
	DEASSERT_FALSE(pEGLCompileContext[index] == EGL_NO_CONTEXT)
	DEASSERT_FALSE(pEGLCompileSurface[index] == EGL_NO_SURFACE)
	
	DEASSERT_TRUE(pEglMakeCurrent(pEGLDisplay, pEGLCompileSurface[index],
		pEGLCompileSurface[index], pEGLCompileContext[index]));
}

void deoglRTCBUnixX11EGL::DeactivateCompileContext(int){
	DEASSERT_TRUE(pEglMakeCurrent(pEGLDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT));
}

void deoglRTCBUnixX11EGL::SwapBuffers(deoglRRenderWindow &window){
	if(window.GetEGLSurface() != EGL_NO_SURFACE){
		pEglSwapBuffers(pEGLDisplay, window.GetEGLSurface());
	}
}

void deoglRTCBUnixX11EGL::ApplyVSync(deoglRRenderWindow&, deoglConfiguration::eVSyncMode vsyncMode){
	deoglRTLogger &logger = pRTContext.GetRenderThread().GetLogger();
	
	switch(vsyncMode){
		case deoglConfiguration::evsmAdaptive:
		case deoglConfiguration::evsmOn:
			logger.LogInfo("RenderWindow: Enable V-Sync");
			pEglSwapInterval(pEGLDisplay, 1);
			break;
			
		case deoglConfiguration::evsmOff:
			logger.LogInfo("RenderWindow: Disable VSync");
			pEglSwapInterval(pEGLDisplay, 0);
			break;
	}
}


EGLContext deoglRTCBUnixX11EGL::GetEGLCompileContextAt(int index) const{
	return pEGLCompileContext[index];
}

EGLSurface deoglRTCBUnixX11EGL::GetEGLCompileSurfaceAt(int index) const{
	return pEGLCompileSurface[index];
}


void deoglRTCBUnixX11EGL::CreateWindowSurface(deoglRRenderWindow &window){
	if(window.GetEGLSurface() != EGL_NO_SURFACE){
		return;
	}
	
	EGLSurface surface = pEglCreateWindowSurface(pEGLDisplay, pEGLConfig,
		(EGLNativeWindowType)window.GetWindow(), nullptr);
	if(surface == EGL_NO_SURFACE){
		pRTContext.GetRenderThread().GetLogger().LogError(
			"eglCreateWindowSurface failed for render window");
		DETHROW(deeInvalidAction);
	}
	
	window.SetEGLSurface(surface);
}

void deoglRTCBUnixX11EGL::DestroyWindowSurface(deoglRRenderWindow &window){
	if(window.GetEGLSurface() == EGL_NO_SURFACE){
		return;
	}
	
	pEglDestroySurface(pEGLDisplay, window.GetEGLSurface());
	window.SetEGLSurface(EGL_NO_SURFACE);
}


// Protected Functions
////////////////////////

void deoglRTCBUnixX11EGL::pFreeContext(){
	if(pEGLDisplay == EGL_NO_DISPLAY || !pEglMakeCurrent){
		return;
	}
	
	pEglMakeCurrent(pEGLDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
	
	int i;
	for(i=0; i<pCompileContextCount; i++){
		if(pEGLCompileSurface[i] != EGL_NO_SURFACE){
			pEglDestroySurface(pEGLDisplay, pEGLCompileSurface[i]);
			pEGLCompileSurface[i] = EGL_NO_SURFACE;
		}
		if(pEGLCompileContext[i] != EGL_NO_CONTEXT){
			pEglDestroyContext(pEGLDisplay, pEGLCompileContext[i]);
			pEGLCompileContext[i] = EGL_NO_CONTEXT;
		}
	}
	
	if(pEGLLoaderSurface != EGL_NO_SURFACE){
		pEglDestroySurface(pEGLDisplay, pEGLLoaderSurface);
		pEGLLoaderSurface = EGL_NO_SURFACE;
	}
	if(pEGLLoaderContext != EGL_NO_CONTEXT){
		pEglDestroyContext(pEGLDisplay, pEGLLoaderContext);
		pEGLLoaderContext = EGL_NO_CONTEXT;
	}
	if(pEGLContext != EGL_NO_CONTEXT){
		pRTContext.GetRenderThread().GetLogger().LogInfo("Free EGL Context");
		pEglDestroyContext(pEGLDisplay, pEGLContext);
		pEGLContext = EGL_NO_CONTEXT;
	}
	
	pEglTerminate(pEGLDisplay);
	pEGLDisplay = EGL_NO_DISPLAY;
}

void deoglRTCBUnixX11EGL::pChooseConfig(){
	deoglRTLogger &logger = pRTContext.GetRenderThread().GetLogger();
	
	pEGLDisplay = pEglGetDisplay((EGLNativeDisplayType)pDisplay);
	if(pEGLDisplay == EGL_NO_DISPLAY){
		DETHROW_INFO(deeInvalidAction, "eglGetDisplay failed");
	}
	
	EGLint eglMajor, eglMinor;
	if(pEglInitialize(pEGLDisplay, &eglMajor, &eglMinor) == EGL_FALSE){
		DETHROW_INFO(deeInvalidAction, "eglInitialize failed");
	}
	logger.LogInfoFormat("EGL version: %d.%d", eglMajor, eglMinor);
	
	if(pEglBindAPI(EGL_OPENGL_API) == EGL_FALSE){
		DETHROW_INFO(deeInvalidAction, "eglBindAPI(EGL_OPENGL_API) failed");
	}
	
	const EGLint configAttribs[] = {
		EGL_SURFACE_TYPE, EGL_WINDOW_BIT | EGL_PBUFFER_BIT,
		EGL_RENDERABLE_TYPE, EGL_OPENGL_BIT,
		EGL_RED_SIZE, 8,
		EGL_GREEN_SIZE, 8,
		EGL_BLUE_SIZE, 8,
		EGL_ALPHA_SIZE, 8,
		EGL_DEPTH_SIZE, 24,
		EGL_STENCIL_SIZE, 8,
		EGL_NONE
	};
	
	EGLint numConfigs = 0;
	if(pEglChooseConfig(pEGLDisplay, configAttribs, &pEGLConfig, 1, &numConfigs) == EGL_FALSE
	|| numConfigs == 0){
		DETHROW_INFO(deeInvalidAction, "eglChooseConfig failed");
	}
	
	if(pRTContext.GetRenderThread().GetConfiguration().GetDoLogDebug()){
		logger.LogInfo("EGL config selected");
	}
}

void deoglRTCBUnixX11EGL::pChooseVisual(){
	EGLint nativeVisualId = 0;
	pEglGetConfigAttrib(pEGLDisplay, pEGLConfig, EGL_NATIVE_VISUAL_ID, &nativeVisualId);
	
	if(nativeVisualId != 0){
		XVisualInfo visTemplate;
		visTemplate.visualid = (VisualID)nativeVisualId;
		int numVisuals = 0;
		pVisInfo = XGetVisualInfo(pDisplay, VisualIDMask, &visTemplate, &numVisuals);
	}
	
	if(!pVisInfo){
		// fall back to DefaultVisual if EGL_NATIVE_VISUAL_ID did not work
		XVisualInfo visTemplate;
		visTemplate.screen = pScreen;
		visTemplate.depth = DefaultDepth(pDisplay, pScreen);
		int numVisuals = 0;
		pVisInfo = XGetVisualInfo(pDisplay, VisualScreenMask | VisualDepthMask,
			&visTemplate, &numVisuals);
	}
	
	if(!pVisInfo){
		DETHROW_INFO(deeInvalidAction, "Failed to find XVisualInfo for EGL config");
	}
	
	pChooseConfig();
}

void deoglRTCBUnixX11EGL::pCreateContext(){
	deoglRTLogger &logger = pRTContext.GetRenderThread().GetLogger();
	logger.LogInfo("Creating EGL Context");
	
	const int compileContextCount = decMath::min(MaxCompileContextCount,
		pRTContext.GetRenderThread().GetOgl().GetGameEngine()->GetParallelProcessing().GetCoreCount());
	
	int debugFlags = 0;
	
	if(pRTContext.GetRenderThread().GetConfiguration().GetDebugContext()){
		logger.LogInfo("Enable debug context");
		debugFlags = EGL_CONTEXT_OPENGL_DEBUG;
	}
	
	EGLint contextAttribs[] = {
		EGL_CONTEXT_MAJOR_VERSION_KHR, 4,
		EGL_CONTEXT_MINOR_VERSION_KHR, 6,
		EGL_CONTEXT_OPENGL_PROFILE_MASK_KHR, EGL_CONTEXT_OPENGL_CORE_PROFILE_BIT_KHR,
		EGL_NONE, EGL_NONE,
		EGL_NONE
	};
	
	if(debugFlags){
		contextAttribs[6] = debugFlags;
		contextAttribs[7] = EGL_TRUE;
	}
	
	int pbufAttribs[] = {
		EGL_WIDTH, 1,
		EGL_HEIGHT, 1,
		EGL_NONE
	};
	
	int i;
	for(i=0; i<vOpenGLVersionCountUsingEGL; i++){
		contextAttribs[1] = vOpenGLVersionsUsingEGL[i].major;
		contextAttribs[3] = vOpenGLVersionsUsingEGL[i].minor;
		
		pEGLContext = pEglCreateContext(pEGLDisplay, pEGLConfig, EGL_NO_CONTEXT, contextAttribs);
		if(pEGLContext != EGL_NO_CONTEXT){
			logger.LogInfoFormat("- Trying %d.%d Core... Success",
				vOpenGLVersionsUsingEGL[i].major, vOpenGLVersionsUsingEGL[i].minor);
			
			pEGLLoaderContext = pEglCreateContext(pEGLDisplay, pEGLConfig, pEGLContext, contextAttribs);
			pEGLLoaderSurface = pEglCreatePbufferSurface(pEGLDisplay, pEGLConfig, pbufAttribs);
			
			int j;
			for(j=0; j<compileContextCount; j++){
				pEGLCompileContext[j] = pEglCreateContext(pEGLDisplay, pEGLConfig, pEGLContext, contextAttribs);
				if(pEGLCompileContext[j] == EGL_NO_CONTEXT){
					break;
				}
				pEGLCompileSurface[j] = pEglCreatePbufferSurface(pEGLDisplay, pEGLConfig, pbufAttribs);
				if(pEGLCompileSurface[j] == EGL_NO_SURFACE){
					pEglDestroyContext(pEGLDisplay, pEGLCompileContext[j]);
					pEGLCompileContext[j] = EGL_NO_CONTEXT;
					break;
				}
			}
			pCompileContextCount = j;
			logger.LogInfoFormat("Created %d compile contexts", pCompileContextCount);
			break;
		}
		
		logger.LogInfoFormat("- Trying %d.%d Core... Failed",
			vOpenGLVersionsUsingEGL[i].major, vOpenGLVersionsUsingEGL[i].minor);
	}
	
	if(pEGLContext == EGL_NO_CONTEXT){
		DETHROW_INFO(deeInvalidAction, "No supported EGL OpenGL context could be created");
	}
}

void deoglRTCBUnixX11EGL::pUnloadLibrary(){
	if(pLibHandle){
		dlclose(pLibHandle);
		pLibHandle = nullptr;
	}
}

#endif
