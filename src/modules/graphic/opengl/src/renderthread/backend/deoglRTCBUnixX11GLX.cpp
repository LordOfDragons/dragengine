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

#include <dlfcn.h>

#include "deoglRTCBUnixX11GLX.h"
#include "../deoglRTContext.h"
#include "../deoglRenderThread.h"
#include "../deoglRTLogger.h"
#include "../../deGraphicOpenGl.h"
#include "../../configuration/deoglConfiguration.h"
#include "../../extensions/deoglExtensions.h"
#include "../../extensions/deoglXExtResult.h"
#include "../../window/deoglRRenderWindow.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/parallel/deParallelProcessing.h>


struct sOpenGlVersionUsingGL{
	int major;
	int minor;
};
static const int vOpenGLVersionCountUsingGL = 9;
static const sOpenGlVersionUsingGL vOpenGLVersionsUsingGL[vOpenGLVersionCountUsingGL] = {
	{4, 6}, {4, 5}, {4, 4}, {4, 3}, {4, 2}, {4, 1}, {4, 0}, {3, 3}, {3, 2} };


// Class deoglRTCBUnixX11GLX
//////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRTCBUnixX11GLX::deoglRTCBUnixX11GLX(deoglRTContext &context) :
deoglRTCBUnixX11(context),
pLibHandle(nullptr),
pGLXContext(nullptr),
pGLXLoaderContext(nullptr),
pGLXBestFBConfig(nullptr),
pGlXCreateNewContext(nullptr),
pGlXCreateContextAttribs(nullptr),
pGlXDestroyContext(nullptr),
pGlXMakeCurrent(nullptr),
pGlXIsDirect(nullptr),
pGlXChooseFBConfig(nullptr),
pGlXGetFBConfigAttrib(nullptr),
pGlXChooseVisual(nullptr),
pGlXGetConfig(nullptr),
pGlXGetProcAddress(nullptr),
pGlxSwapBuffers(nullptr)
{
	int i;
	for(i=0; i<MaxCompileContextCount; i++){
		pGLXCompileContext[i] = nullptr;
		pCompileContextDisplay[i] = nullptr;
	}
}

deoglRTCBUnixX11GLX::~deoglRTCBUnixX11GLX(){
	CleanUp();
}


// Management
///////////////

bool deoglRTCBUnixX11GLX::TryInit(){
	deoglRTLogger &logger = pRTContext.GetRenderThread().GetLogger();
	
	// try libGLX.so.0 first, then libGLX.so
	void *handle = dlopen("libGLX.so.0", RTLD_LAZY | RTLD_LOCAL);
	if(!handle){
		handle = dlopen("libGLX.so", RTLD_LAZY | RTLD_LOCAL);
	}
	if(!handle){
		logger.LogError("deoglRTCBUnixX11GLX: libGLX.so not found");
		return false;
	}
	
	// load all required GLX symbols
	pGlXCreateNewContext = (PFNGLXCREATENEWCONTEXTPROC)dlsym(handle, "glXCreateNewContext");
	pGlXCreateContextAttribs = (PFNGLXCREATECONTEXTATTRIBSARBPROC2)dlsym(handle, "glXCreateContextAttribsARB");
	if(!pGlXCreateContextAttribs){
		pGlXCreateContextAttribs = (PFNGLXCREATECONTEXTATTRIBSARBPROC2)dlsym(handle, "glXCreateContextAttribs");
	}
	pGlXDestroyContext = (PFNGLXDESTROYCONTEXTPROC)dlsym(handle, "glXDestroyContext");
	pGlXMakeCurrent = (PFNGLXMAKECURRENTPROC)dlsym(handle, "glXMakeCurrent");
	pGlXIsDirect = (PFNGLXISDIRECTPROC)dlsym(handle, "glXIsDirect");
	pGlXChooseFBConfig = (PFNGLXCHOOSEFBCONFIGPROC)dlsym(handle, "glXChooseFBConfig");
	pGlXGetFBConfigAttrib = (PFNGLXGETFBCONFIGATTRIBPROC)dlsym(handle, "glXGetFBConfigAttrib");
	pGlXChooseVisual = (PFNGLXCHOOSEVISUALPROC)dlsym(handle, "glXChooseVisual");
	pGlXGetConfig = (PFNGLXGETCONFIGPROC)dlsym(handle, "glXGetConfig");
	pGlXGetProcAddress = (PFNGLXGETPROCADDRESSPROC)dlsym(handle, "glXGetProcAddressARB");
	if(!pGlXGetProcAddress){
		pGlXGetProcAddress = (PFNGLXGETPROCADDRESSPROC)dlsym(handle, "glXGetProcAddress");
	}
	pGlxSwapBuffers = (PFNGLXSWAPBUFFERSPROC)dlsym(handle, "glXSwapBuffers");
	
	if(!pGlXCreateNewContext || !pGlXDestroyContext || !pGlXMakeCurrent || !pGlXIsDirect
	|| !pGlXChooseFBConfig || !pGlXGetFBConfigAttrib || !pGlXChooseVisual
	|| !pGlXGetProcAddress || !pGlxSwapBuffers){
		logger.LogError("deoglRTCBUnixX11GLX: libGLX.so found but required symbols are missing");
		dlclose(handle);
		return false;
	}
	
	pLibHandle = handle;
	logger.LogInfo("deoglRTCBUnixX11GLX: Using GLX backend (libGLX.so)");
	
	pOpenDisplay();
	pCreateAtoms();
	pChooseVisual();
	pChooseFBConfig();
	pCreateColorMap();
	pCreateContext();
	return true;
}

void deoglRTCBUnixX11GLX::InitPhase2(){
	deoglRTCBUnixX11::InitPhase2();
	pCreateContext();
}

void deoglRTCBUnixX11GLX::CleanUp(){
	AssignOSWindow(nullptr);
	pFreeContext();
	pUnloadLibrary();
	deoglRTCBUnixX11::CleanUp();
}

void deoglRTCBUnixX11GLX::DropCompileContexts(int count){
	deoglRTLogger &logger = pRTContext.GetRenderThread().GetLogger();
	int i;
	
	for(i=count; i<pCompileContextCount; i++){
		if(pGLXCompileContext[i]){
			logger.LogInfoFormat("Drop compile context %d", i);
			pGlXDestroyContext(pDisplay, pGLXCompileContext[i]);
			pGLXCompileContext[i] = nullptr;
		}
	}
	
	pCompileContextCount = count;
}

void deoglRTCBUnixX11GLX::ActivateContext(deoglRRenderWindow &window){
	// printf("glXMakeCurrent(%lu,%p) previous(%lu,%p)\n",
	// 	window.GetWindow(), pGLXContext, glXGetCurrentDrawable(), glXGetCurrentContext());
	OGLX_CHECK(pRTContext.GetRenderThread(),
		pGlXMakeCurrent(pDisplay, window.GetWindow(), pGLXContext));
}

void deoglRTCBUnixX11GLX::DeactivateContext(){
	// printf("glXMakeCurrent(clear) previous(%lu,%p)\n",
	//	glXGetCurrentDrawable(), glXGetCurrentContext());
	OGLX_CHECK(pRTContext.GetRenderThread(),
		pGlXMakeCurrent(pDisplay, None, nullptr));
}

bool deoglRTCBUnixX11GLX::HasLoaderContext() const{
	return pGLXLoaderContext != nullptr;
}

void *deoglRTCBUnixX11GLX::GetFunctionPointer(const char *funcName){
	if(pGlXGetProcAddress){
		return (void*)pGlXGetProcAddress((const GLubyte*)funcName);
	}
	return nullptr;
}

void deoglRTCBUnixX11GLX::ActivateLoaderContext(){
	DEASSERT_NOTNULL(pGLXLoaderContext)
	
	OGLX_CHECK(pRTContext.GetRenderThread(), pGlXMakeCurrent(pDisplay,
		pRTContext.GetActiveRRenderWindow()->GetWindow(), pGLXLoaderContext));
}

void deoglRTCBUnixX11GLX::DeactivateLoaderContext(){
	OGLX_CHECK(pRTContext.GetRenderThread(), pGlXMakeCurrent(pDisplay, None, nullptr));
}

void deoglRTCBUnixX11GLX::ActivateCompileContext(int index){
	DEASSERT_TRUE(index >= 0)
	DEASSERT_TRUE(index < MaxCompileContextCount)
	DEASSERT_NOTNULL(pGLXCompileContext[index])
	
	// on nVidia there can be strange segfaults if using context display connection.
	// to be on the safe side use a unique display connection.
	const char *dispName = getenv("DISPLAY");
	if(!dispName){
		dispName = "";
	}
	pCompileContextDisplay[index] = XOpenDisplay(dispName);
	DEASSERT_NOTNULL(pCompileContextDisplay[index])
	
	OGLX_CHECK(pRTContext.GetRenderThread(), pGlXMakeCurrent(
		/*pDisplay*/ pCompileContextDisplay[index],
		pRTContext.GetActiveRRenderWindow()->GetWindow(), pGLXCompileContext[index]));
	
	/*
	OGL_CHECK_WRTC(pRTContext.GetRenderThread(), false, pGlXMakeCurrent(pDisplay,
		pRTContext.GetActiveRRenderWindow()->GetWindow(), pGLXCompileContext[index]));
	*/
}

void deoglRTCBUnixX11GLX::DeactivateCompileContext(int index){
	DEASSERT_TRUE(index >= 0)
	DEASSERT_TRUE(index < MaxCompileContextCount)
	
	if(pCompileContextDisplay[index]){
		pGlXMakeCurrent(pCompileContextDisplay[index], None, nullptr);
		XCloseDisplay(pCompileContextDisplay[index]);
		pCompileContextDisplay[index] = nullptr;
	}
}

void deoglRTCBUnixX11GLX::SwapBuffers(deoglRRenderWindow &window){
	OGL_CHECK(pRTContext.GetRenderThread(), pGlxSwapBuffers(pDisplay, window.GetWindow()));
}

void deoglRTCBUnixX11GLX::ApplyVSync(deoglRRenderWindow &window, deoglConfiguration::eVSyncMode vsyncMode){
	const deoglExtensions &ext = pRTContext.GetRenderThread().GetExtensions();
	deoglRTLogger &logger = pRTContext.GetRenderThread().GetLogger();
	
	if(ext.GetHasExtension(deoglExtensions::ext_GLX_EXT_swap_control)){
		switch(vsyncMode){
		case deoglConfiguration::evsmAdaptive:
			if(ext.GetHasExtension(deoglExtensions::ext_GLX_EXT_swap_control_tear)){
				logger.LogInfo("RenderWindow: Enable Adaptive V-Sync");
				OGL_CHECK(pRTContext.GetRenderThread(), pglXSwapInterval(
					pRTContext.GetRenderThread().GetContext().GetBackend()->GetDisplay(),
					window.GetWindow(), -1));
				
			}else{
				logger.LogInfo("RenderWindow: Enable V-Sync");
				OGL_CHECK(pRTContext.GetRenderThread(), pglXSwapInterval(
					pRTContext.GetRenderThread().GetContext().GetBackend()->GetDisplay(),
					window.GetWindow(), 1));
			}
			break;
			
		case deoglConfiguration::evsmOn:
			logger.LogInfo("RenderWindow: Enable V-Sync");
			OGL_CHECK(pRTContext.GetRenderThread(), pglXSwapInterval(
				pRTContext.GetRenderThread().GetContext().GetBackend()->GetDisplay(),
				window.GetWindow(), 1));
			break;
			
		case deoglConfiguration::evsmOff:
			logger.LogInfo("RenderWindow: Disable VSync");
			OGL_CHECK(pRTContext.GetRenderThread(), pglXSwapInterval(
				pRTContext.GetRenderThread().GetContext().GetBackend()->GetDisplay(),
				window.GetWindow(), 0));
		}
	}
}


GLXContext deoglRTCBUnixX11GLX::GetGLXCompileContextAt(int index) const{
	return pGLXCompileContext[index];
}


// Protected Functions
////////////////////////

void deoglRTCBUnixX11GLX::pFreeContext(){
	if(!pDisplay || !pGlXDestroyContext){
		return;
	}
	
	int i;
	for(i=0; i<pCompileContextCount; i++){
		if(pGLXCompileContext[i]){
			pGlXDestroyContext(pDisplay, pGLXCompileContext[i]);
			pGLXCompileContext[i] = nullptr;
		}
	}
	if(pGLXLoaderContext){
		pGlXDestroyContext(pDisplay, pGLXLoaderContext);
		pGLXLoaderContext = nullptr;
	}
	if(pGLXContext){
		pRTContext.GetRenderThread().GetLogger().LogInfo("Free GLX Context");
		pGlXDestroyContext(pDisplay, pGLXContext);
		pGLXContext = nullptr;
	}
}

void deoglRTCBUnixX11GLX::pChooseVisual(){
	int attrList[] = {
		GLX_RGBA,
		GLX_RED_SIZE, 8,
		GLX_GREEN_SIZE, 8,
		GLX_BLUE_SIZE, 8,
		GLX_ALPHA_SIZE, 8,
		GLX_DEPTH_SIZE, 24,
		GLX_STENCIL_SIZE, 8,
		GLX_DOUBLEBUFFER,
		None
	};
	
	pVisInfo = pGlXChooseVisual(pDisplay, pScreen, attrList);
	if(!pVisInfo){
		DETHROW(deeInvalidAction);
	}
	
	if(pRTContext.GetRenderThread().GetConfiguration().GetDoLogDebug()){
		pPrintVisualInfo();
	}
	
	pChooseFBConfig();
}

void deoglRTCBUnixX11GLX::pPrintVisualInfo(){
	deoglRTLogger &logger = pRTContext.GetRenderThread().GetLogger();
	int value;
	
	logger.LogInfo("Visual Infos:");
	
	pGlXGetConfig(pDisplay, pVisInfo, GLX_RED_SIZE, &value);
	logger.LogInfoFormat("  Red Bit Count:      %d", value);
	
	pGlXGetConfig(pDisplay, pVisInfo, GLX_GREEN_SIZE, &value);
	logger.LogInfoFormat("  Green Bit Count:    %d", value);
	
	pGlXGetConfig(pDisplay, pVisInfo, GLX_BLUE_SIZE, &value);
	logger.LogInfoFormat("  Blue Bit Count:     %d", value);
	
	pGlXGetConfig(pDisplay, pVisInfo, GLX_ALPHA_SIZE, &value);
	logger.LogInfoFormat("  Alpha Bit Count:    %d", value);
	
	pGlXGetConfig(pDisplay, pVisInfo, GLX_DEPTH_SIZE, &value);
	logger.LogInfoFormat("  Depth Bit Count:    %d", value);
	
	pGlXGetConfig(pDisplay, pVisInfo, GLX_STENCIL_SIZE, &value);
	logger.LogInfoFormat("  Stencil Bit Count:  %d", value);
	
	pGlXGetConfig(pDisplay, pVisInfo, GLX_ACCUM_RED_SIZE, &value);
	logger.LogInfoFormat("  Accumulation Buffer Red Bit Count:    %d", value);
	
	pGlXGetConfig(pDisplay, pVisInfo, GLX_ACCUM_GREEN_SIZE, &value);
	logger.LogInfoFormat("  Accumulation Buffer Green Bit Count:  %d", value);
	
	pGlXGetConfig(pDisplay, pVisInfo, GLX_ACCUM_BLUE_SIZE, &value);
	logger.LogInfoFormat("  Accumulation Buffer Blue Bit Count:   %d", value);
	
	pGlXGetConfig(pDisplay, pVisInfo, GLX_ACCUM_ALPHA_SIZE, &value);
	logger.LogInfoFormat("  Accumulation Buffer Alpha Bit Count:  %d", value);
}

void deoglRTCBUnixX11GLX::pChooseFBConfig(){
	deoglRTLogger &logger = pRTContext.GetRenderThread().GetLogger();
	GLXFBConfig *configs = nullptr;
	int configCount = 0;
	int configValue;
	
	int attribs[] = {
		GLX_X_RENDERABLE, True,
		GLX_DRAWABLE_TYPE, GLX_PBUFFER_BIT | GLX_WINDOW_BIT,
		GLX_X_VISUAL_TYPE, GLX_TRUE_COLOR,
		GLX_CONFIG_CAVEAT, GLX_NONE,
		GLX_RENDER_TYPE, GLX_RGBA_BIT,
		GLX_RED_SIZE, 8,
		GLX_GREEN_SIZE, 8,
		GLX_BLUE_SIZE, 8,
		GLX_ALPHA_SIZE, 8,
		GLX_DEPTH_SIZE, 24,
		GLX_STENCIL_SIZE, 8,
		GLX_DOUBLEBUFFER, True,
		None
	};
	
	try{
		// try to find a matching fbconfig
		configs = pGlXChooseFBConfig(pDisplay, pScreen, attribs, &configCount);
		if(configCount == 0 || !configs){
			DETHROW_INFO(deeInvalidAction, "glXChooseFBConfig failed");
		}
		
		// find the best config that is not over-sized
		const int bestConfig = configCount - 1;
		/*
		for(bestConfig=0; bestConfig<configCount; bestConfig++){
			glXGetFBConfigAttrib(pDisplay, configs[0], GLX_RED_SIZE, &configValue);
			if(configValue > maxBitsRed) continue;
			glXGetFBConfigAttrib(pDisplay, configs[0], GLX_GREEN_SIZE, &configValue);
			if(configValue > maxBitsGreen) continue;
			glXGetFBConfigAttrib(pDisplay, configs[0], GLX_BLUE_SIZE, &configValue);
			if(configValue > maxBitsBlue) continue;
			glXGetFBConfigAttrib(pDisplay, configs[0], GLX_ALPHA_SIZE, &configValue);
			if(configValue > maxBitsAlpha) continue;
			glXGetFBConfigAttrib(pDisplay, configs[0], GLX_DEPTH_SIZE, &configValue);
			if(configValue > maxBitsDepth) continue;
			glXGetFBConfigAttrib(pDisplay, configs[0], GLX_STENCIL_SIZE, &configValue);
			if(configValue > maxBitsStencil) continue;
			// good
			break;
		}
		if(bestConfig == configCount) bestConfig = configCount - 1;
		*/
		
		// store the best config away and free memory
		pGLXBestFBConfig = configs[bestConfig];
		XFree(configs);
		configs = nullptr;
		
		// print out the stats of the best config we have chosen
		if(pRTContext.GetRenderThread().GetConfiguration().GetDoLogDebug()){
			logger.LogInfo("glXChooseFBConfig:");
			
			pGlXGetFBConfigAttrib(pDisplay, pGLXBestFBConfig, GLX_RED_SIZE, &configValue);
			logger.LogInfoFormat("  Red Bit Count:      %d", configValue);
			
			pGlXGetFBConfigAttrib(pDisplay, pGLXBestFBConfig, GLX_GREEN_SIZE, &configValue);
			logger.LogInfoFormat("  Green Bit Count:    %d", configValue);
			
			pGlXGetFBConfigAttrib(pDisplay, pGLXBestFBConfig, GLX_BLUE_SIZE, &configValue);
			logger.LogInfoFormat("  Blue Bit Count:     %d", configValue);
			
			pGlXGetFBConfigAttrib(pDisplay, pGLXBestFBConfig, GLX_ALPHA_SIZE, &configValue);
			logger.LogInfoFormat("  Alpha Bit Count:    %d", configValue);
			
			pGlXGetFBConfigAttrib(pDisplay, pGLXBestFBConfig, GLX_DEPTH_SIZE, &configValue);
			logger.LogInfoFormat("  Depth Bit Count:    %d", configValue);
			
			pGlXGetFBConfigAttrib(pDisplay, pGLXBestFBConfig, GLX_STENCIL_SIZE, &configValue);
			logger.LogInfoFormat("  Stencil Bit Count:  %d", configValue);
		}
		
	}catch(const deException &){
		if(configs){
			XFree(configs);
		}
		throw;
	}
}

void deoglRTCBUnixX11GLX::pCreateContext(){
	deoglRTLogger &logger = pRTContext.GetRenderThread().GetLogger();
	
	const int compileContextCount = decMath::min(MaxCompileContextCount,
		pRTContext.GetRenderThread().GetOgl().GetGameEngine()->GetParallelProcessing().GetCoreCount());
	
	// create render context. try new version first then the old one if not possible
	// NOTE it is important to try to find first the "ARB" version of the function.
	//      some driver implementations have bugged "No-ARB" version of the function
	//      returning NULL. the "ARB" version seems less error prone
	if(pGlXCreateContextAttribs){
		logger.LogInfo("Creating OpenGL Context using new method");
		
		int contextFlags = 0;
		
		if(pRTContext.GetRenderThread().GetConfiguration().GetDebugContext()){
			logger.LogInfo("Enable debug context");
			contextFlags |= GLX_CONTEXT_DEBUG_BIT_ARB;
		}
		
		// this one here is really the biggest mess of all times. AMD does it right in that
		// requesting a 3.3 context (as minimum) gives you up to 4.6 context if supported.
		// this is how it is correctly done. and then we have nVidia which (again) fails to
		// to it correctly. it gives you exactly a 3.3 context instead of the highest supported
		// context. this causes problems since nVidia (again) fails compiling shaders if the
		// shader version is not set high enough. this is again wrong in many ways since using
		// the #extension directive in the shader overrules the #version directive but nVidia
		// doesn't give a damn about OpenGL specs. Intel is in the same boat but nobody expects
		// any sane 3D drivers from them anyway. so how to solve this mess? we do it ugly by
		// simply trying all possible OpenGL context versions from the highest down to the
		// lowest until we find the first one that works. and yes, this is really, really,
		// REALLY ugly. thanks nVidia for nothing
		int contextAttribs[] = {
			GLX_CONTEXT_MAJOR_VERSION_ARB, 3,
			GLX_CONTEXT_MINOR_VERSION_ARB, 3,
			GLX_RENDER_TYPE, GLX_RGBA_TYPE,
			GLX_CONTEXT_PROFILE_MASK_ARB, GLX_CONTEXT_CORE_PROFILE_BIT_ARB,
			GLX_CONTEXT_FLAGS_ARB, contextFlags,
			None, None,
			None
		};
		
		int i;
		for(i=0; i<vOpenGLVersionCountUsingGL; i++){
			contextAttribs[1] = vOpenGLVersionsUsingGL[i].major;
			contextAttribs[3] = vOpenGLVersionsUsingGL[i].minor;
			
			pGLXContext = pGlXCreateContextAttribs(pDisplay, pGLXBestFBConfig,
				nullptr, True, contextAttribs);
			if(pGLXContext){
				logger.LogInfoFormat("- Trying %d.%d Core... Success",
					vOpenGLVersionsUsingGL[i].major, vOpenGLVersionsUsingGL[i].minor);
				pGLXLoaderContext = pGlXCreateContextAttribs(pDisplay, pGLXBestFBConfig,
					pGLXContext, True, contextAttribs);
				int j;
				for(j=0; j<compileContextCount; j++){
					pGLXCompileContext[j] = pGlXCreateContextAttribs(pDisplay, pGLXBestFBConfig,
						pGLXContext, True, contextAttribs);
					if(!pGLXCompileContext[j]){
						break;
					}
				}
				pCompileContextCount = j;
				logger.LogInfoFormat("Created %d compile contexts", pCompileContextCount);
				break;
			}
			
			logger.LogInfoFormat("- Trying %d.%d Core... Failed",
				vOpenGLVersionsUsingGL[i].major, vOpenGLVersionsUsingGL[i].minor);
		}
		
		if(!pGLXContext){
			logger.LogWarn("No supported OpenGL Context could be created with new method."
				" Creating OpenGL Context using old method");
			pGLXContext = pGlXCreateNewContext(pDisplay,
				pGLXBestFBConfig, GLX_RGBA_TYPE, nullptr, True);
			pGLXLoaderContext = pGlXCreateNewContext(pDisplay,
				pGLXBestFBConfig, GLX_RGBA_TYPE, pGLXContext, True);
			for(i=0; i<compileContextCount; i++){
				pGLXCompileContext[i] = pGlXCreateNewContext(pDisplay,
					pGLXBestFBConfig, GLX_RGBA_TYPE, pGLXContext, True);
				if(!pGLXCompileContext[i]){
					break;
				}
			}
			pCompileContextCount = i;
			logger.LogInfoFormat("Created %d compile contexts", pCompileContextCount);
		}
		
	}else{
		logger.LogInfo("Creating OpenGL Context using old method");
		pGLXContext = pGlXCreateNewContext(pDisplay,
			pGLXBestFBConfig, GLX_RGBA_TYPE, nullptr, True);
		pGLXLoaderContext = pGlXCreateNewContext(pDisplay,
			pGLXBestFBConfig, GLX_RGBA_TYPE, pGLXContext, True);
		int i;
		for(i=0; i<compileContextCount; i++){
			pGLXCompileContext[i] = pGlXCreateNewContext(pDisplay,
				pGLXBestFBConfig, GLX_RGBA_TYPE, pGLXContext, True);
			if(!pGLXCompileContext[i]){
				break;
			}
		}
		pCompileContextCount = i;
		logger.LogInfoFormat("Created %d compile contexts", pCompileContextCount);
	}
	
	DEASSERT_NOTNULL(pGLXContext)
	DEASSERT_NOTNULL(pGLXLoaderContext)
	
	if(!pGlXIsDirect(pDisplay, pGLXContext)){
		logger.LogError("No matching direct rendering context found!");
		DETHROW(deeInvalidAction);
	}
}

void deoglRTCBUnixX11GLX::pUnloadLibrary(){
	if(pLibHandle){
		dlclose(pLibHandle);
		pLibHandle = nullptr;
	}
}

#endif
