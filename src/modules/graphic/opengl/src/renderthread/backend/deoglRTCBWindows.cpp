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

#ifdef OS_W32

#include "deoglRTCBWindows.h"

#include <dxgi1_6.h>
#include <dragengine/app/deOSWindows.h>

#include "../deoglRTContext.h"
#include "../deoglRenderThread.h"
#include "../deoglRTLogger.h"
#include "../deoglLoaderThread.h"
#include "../../deGraphicOpenGl.h"
#include "../../configuration/deoglConfiguration.h"
#include "../../extensions/deoglExtensions.h"
#include "../../extensions/wglext.h"
#include "../../extensions/deoglWExtResult.h"
#include "../../window/deoglRenderWindow.h"
#include "../../window/deoglRRenderWindow.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/collection/decTUniqueList.h>
#include <dragengine/parallel/deParallelProcessing.h>
#include <dragengine/resources/rendering/deRenderWindow.h>
#include <dragengine/systems/deInputSystem.h>
#include <dragengine/systems/deScriptingSystem.h>
#include <dragengine/systems/modules/input/deBaseInputModule.h>


// global state for window procedure
static deoglRTCBWindows *vW32WndFuncContext = nullptr;

static LRESULT CALLBACK vW32WndFunc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam){
	if(vW32WndFuncContext){
		return vW32WndFuncContext->ProcessWindowMessage(hwnd, message, wParam, lParam);
		
	}else{
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
}

struct sOpenGlVersionW32{
	int major;
	int minor;
};
static const int vOpenGLVersionCountW32 = 9;
static const sOpenGlVersionW32 vOpenGLVersionsW32[vOpenGLVersionCountW32] = {
	{4, 6}, {4, 5}, {4, 4}, {4, 3}, {4, 2}, {4, 1}, {4, 0}, {3, 3}, {3, 2} };


// Class deoglRTCBWindows
///////////////////////////

// Constructor, destructor
////////////////////////////

deoglRTCBWindows::deoglRTCBWindows(deoglRTContext &context) :
deoglRTCBackend(context),
pWindowClassname("DEOpenGLWindow"),
pOSWindows(context.GetRenderThread().GetOgl().GetOS()->CastToOSWindows()),
pContext(nullptr),
pLoaderContext(nullptr),
pHdrPixelFormat(0)
{
	int i;
	for(i=0; i<MaxCompileContextCount; i++){
		pCompileContext[i] = nullptr;
	}
}

deoglRTCBWindows::~deoglRTCBWindows() = default;


// Management
///////////////

void deoglRTCBWindows::InitPhase3(){
	pRegisterWindowClass();
}

void deoglRTCBWindows::InitPhase4(){
	pCreateContext();
}

void deoglRTCBWindows::AssignOSWindow(deRenderWindow *renderWindow){
	pOSWindows->SetWindow(renderWindow->GetWindow());
}

void deoglRTCBWindows::CleanUp(){
	pOSWindows->SetWindow(nullptr);
	pFreeContext();
	pUnregisterWindowClass();
}

void deoglRTCBWindows::DropCompileContexts(int count){
	deoglRTLogger &logger = pRTContext.GetRenderThread().GetLogger();
	int i;
	for(i=count; i<pCompileContextCount; i++){
		logger.LogInfoFormat("Drop compile context %d", i);
		if(pCompileContext[i]){
			wglDeleteContext(pCompileContext[i]);
			pCompileContext[i] = nullptr;
		}
	}
	pCompileContextCount = count;
}

void deoglRTCBWindows::ActivateContext(deoglRRenderWindow &window){
	if(!wglMakeCurrent(window.GetWindowDC(), pContext)){
		pRTContext.GetRenderThread().GetLogger().LogErrorFormat(
			"wglMakeCurrent failed (%s:%i): error=0x%lx\n", __FILE__, __LINE__, GetLastError());
	}
	if(pLoaderContext){
		pRTContext.GetRenderThread().GetLoaderThread().EnableContext(true);
	}
}

void deoglRTCBWindows::DeactivateContext(){
	if(!wglMakeCurrent(nullptr, nullptr)){
		pRTContext.GetRenderThread().GetLogger().LogErrorFormat(
			"wglMakeCurrent failed (%s:%i): error=0x%lx\n", __FILE__, __LINE__, GetLastError());
	}
}

bool deoglRTCBWindows::HasLoaderContext() const{
	return pLoaderContext != nullptr;
}


LRESULT deoglRTCBWindows::ProcessWindowMessage(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam){
	// WARNING sent by main thread while processing events
	deoglRenderThread &renderThread = pRTContext.GetRenderThread();
	
	switch(message){
	case WM_SETCURSOR:{
		for(const auto &window : renderThread.GetOgl().GetRenderWindowList().GetWindows()){
			if(window->GetRRenderWindow()->GetWindow() == hwnd){
				if(window->GetRRenderWindow()->GetHostWindow()){
					return DefWindowProc(hwnd, message, wParam, lParam);
				}
				break;
			}
		}
		if(LOWORD(lParam) != HTCLIENT){
			return DefWindowProc(hwnd, message, wParam, lParam);
		}
		
		SetCursor(nullptr);
		}return TRUE;
		
	case WM_CLOSE:
		renderThread.GetOgl().GetGameEngine()->GetScriptingSystem()->UserRequestQuit();
		return 0; // do not close
		
	case WM_ACTIVATEAPP:
		// changing this value in the main thread is acceptable
		pRTContext.SetAppActivated(wParam == TRUE);
		
		// this situation is now annoying. the deOSWindow does not receive WM_ACTIVATEAPP
		// only we do. this in turn means the windows input module will not receive this
		// message either but it is required to get it. so we forward the message to
		// the input module
		{
		MSG msg;
		memset(&msg, 0, sizeof(msg));
		msg.hwnd = hwnd;
		msg.message = message;
		msg.wParam = wParam;
		msg.lParam = lParam;
		renderThread.GetOgl().GetGameEngine()->GetInputSystem()->GetActiveModule()->EventLoop(msg);
		}
		return 0;
		
	case WM_ERASEBKGND:
		// according to opengl wiki required to prevent flickering
		return 1;
		
	case WM_SIZE:{
		for(const auto &window : renderThread.GetOgl().GetRenderWindowList().GetWindows()){
			if(window->GetRRenderWindow()->GetWindow() == hwnd){
				// for fullscreen windows the size is incorrect while the window style is changed.
				// to avoid problems ignore size change if the window is full screen
				if(window->GetRRenderWindow()->GetFullScreen()){
					break;
				}
				
				// the size provided in the message are the outer size of the window not the inner size.
				// for non-fullscreen windows we have to calculate the correct inner size
				const decPoint innerSize(window->GetRRenderWindow()->GetInnerSize());
				window->GetRenderWindow().SetSize(innerSize.x, innerSize.y);
				break;
			}
		}
		}return 0;
		
	case WM_DPICHANGED:{
		const int scaleFactor = 100 * HIWORD(wParam) / USER_DEFAULT_SCREEN_DPI;
		const RECT &rect = *((RECT*)lParam);
		
		for(const auto &window : renderThread.GetOgl().GetRenderWindowList().GetWindows()){
			deoglRRenderWindow &rwindow = *window->GetRRenderWindow();
			
			if(rwindow.GetWindow() == hwnd){
				deRenderWindow &engWindow = window->GetRenderWindow();
				
				engWindow.SetScaleFactor(scaleFactor);
				
				if(rwindow.GetFullScreen()){
					engWindow.SetSize(rect.right - rect.left, rect.bottom - rect.top);
					
				}else{
					engWindow.SetPosition(rect.left, rect.top);
					engWindow.SetSize(rect.right - rect.left, rect.bottom - rect.top);
				}
				break;
			}
		}
		}return 0;
	}
	
	return DefWindowProc(hwnd, message, wParam, lParam);
}

void *deoglRTCBWindows::GetFunctionPointer(const char *funcName){
	return (void*)wglGetProcAddress(funcName);
}

void deoglRTCBWindows::ActivateLoaderContext(){
	DEASSERT_NOTNULL(pLoaderContext)
	
	if(!wglMakeCurrent(pRTContext.GetActiveRRenderWindow()->GetWindowDC(), pLoaderContext)){
		pRTContext.GetRenderThread().GetLogger().LogErrorFormat(
			"wglMakeCurrent failed (%s:%i): error=0x%lx\n", __FILE__, __LINE__, GetLastError());
	}
}

void deoglRTCBWindows::DeactivateLoaderContext(){
	if(!wglMakeCurrent(nullptr, nullptr)){
		pRTContext.GetRenderThread().GetLogger().LogErrorFormat(
			"wglMakeCurrent failed (%s:%i): error=0x%lx\n", __FILE__, __LINE__, GetLastError());
	}
}

void deoglRTCBWindows::ActivateCompileContext(int index){
	DEASSERT_TRUE(index >= 0)
	DEASSERT_TRUE(index < MaxCompileContextCount)
	DEASSERT_NOTNULL(pCompileContext[index])
	
	// on windows make current can randomly fail at times. in this case retry the make
	// current call after a short wait to see if it fixes itself. if this goes on for
	// too long call it quits
	auto hdc = pRTContext.GetActiveRRenderWindow()->GetWindowDC();
	auto &logger = pRTContext.GetRenderThread().GetLogger();
	const int retryCounterMax = 10;
	const int retryDelayMs = 100;
	int retryCounter;
	
	for(retryCounter=0; retryCounter<retryCounterMax; retryCounter++){
		if(wglMakeCurrent(hdc, pCompileContext[index])){
			if(retryCounter > 0){
				logger.LogErrorFormat("wglMakeCurrent recovered (%s:%i)[%d]: counter=%d\n",
					__FILE__, __LINE__, index, retryCounter);
			}
			break;
		}
		
		logger.LogErrorFormat("wglMakeCurrent failed (%s:%i)[%d]: error=0x%lx, counter=%d\n",
			__FILE__, __LINE__, index, GetLastError(), retryCounter);
		
		SleepEx(retryDelayMs, false);
	}
	
	if(retryCounter == retryCounterMax){
		DETHROW_INFO(deeInvalidAction, "wglMakeCurrent failed");
	}
}

void deoglRTCBWindows::DeactivateCompileContext(int){
	if(!wglMakeCurrent(nullptr, nullptr)){
		pRTContext.GetRenderThread().GetLogger().LogErrorFormat(
			"wglMakeCurrent failed (%s:%i): error=0x%lx\n", __FILE__, __LINE__, GetLastError());
	}
}

void deoglRTCBWindows::SwapBuffers(deoglRRenderWindow &window){
	if(!::SwapBuffers(window.GetWindowDC())){
		pRTContext.GetRenderThread().GetLogger().LogErrorFormat(
			"SwapBuffers failed (%s:%i): error=0x%lx\n", __FILE__, __LINE__, GetLastError());
	}
}

void deoglRTCBWindows::ApplyVSync(deoglRRenderWindow&, deoglConfiguration::eVSyncMode vsyncMode){
	const deoglExtensions &ext = pRTContext.GetRenderThread().GetExtensions();
	deoglRTLogger &logger = pRTContext.GetRenderThread().GetLogger();
	
	if(ext.GetHasExtension(deoglExtensions::ext_WGL_EXT_swap_control)){
		switch(vsyncMode){
		case deoglConfiguration::evsmAdaptive:
			if(ext.GetHasExtension(deoglExtensions::ext_WGL_EXT_swap_control_tear)){
				logger.LogInfo("RenderWindow: Enable Adaptive V-Sync");
				DEASSERT_TRUE(pwglSwapInterval(-1))
				
			}else{
				logger.LogInfo("RenderWindow: Enable V-Sync");
				DEASSERT_TRUE(pwglSwapInterval(1))
			}
			break;
			
		case deoglConfiguration::evsmOn:
			logger.LogInfo("RenderWindow: Enable V-Sync");
			DEASSERT_TRUE(pwglSwapInterval(1))
			break;
			
		case deoglConfiguration::evsmOff:
			logger.LogInfo("RenderWindow: Disable VSync");
			DEASSERT_TRUE(pwglSwapInterval(0))
		}
	}
}


// Protected Functions
////////////////////////

void deoglRTCBWindows::pRegisterWindowClass(){
	vW32WndFuncContext = this;
	
	WNDCLASSEX wce;
	memset(&wce, 0, sizeof(wce));
	wce.hInstance = pOSWindows->GetInstApp();
	wchar_t wideClassName[200];
	deOSWindows::Utf8ToWide(pWindowClassname, wideClassName, MAX_PATH);
	wce.lpszClassName = wideClassName;
	wce.lpfnWndProc = vW32WndFunc;
	wce.cbSize = sizeof(wce);
	wce.hIcon = nullptr;
	wce.hIconSm = nullptr;
	wce.hCursor = nullptr;
	wce.lpszMenuName = nullptr;
	wce.hbrBackground = nullptr;
	wce.style = CS_OWNDC;
	
	if(!RegisterClassEx(&wce)){
		DETHROW(deeOutOfMemory);
	}
	
	pChooseHdrPixelFormat();
}

void deoglRTCBWindows::pCreateContext(){
	deoglRTLogger &logger = pRTContext.GetRenderThread().GetLogger();
	deoglRRenderWindow * const activeWindow = pRTContext.GetActiveRRenderWindow();
	
	const int compileContextCount = decMath::min(MaxCompileContextCount,
		pRTContext.GetRenderThread().GetOgl().GetGameEngine()->GetParallelProcessing().GetCoreCount());
	
	// create render context. on windows obtaining function pointers only works if a context has
	// been created. unfortunately the wglCreateContextAttribs is such a function pointer so we
	// need a fake context first before we can obtain that method. for this reason we first
	// create a context using the old method
	logger.LogInfo("Creating OpenGL Context using old method");
	pContext = wglCreateContext(activeWindow->GetWindowDC());
	if(!pContext){
		logger.LogErrorFormat("wglCreateContext failed with code %d", (int)GetLastError());
		DETHROW(deeOutOfMemory);
	}
	
	wglMakeCurrent(activeWindow->GetWindowDC(), pContext);
	
	// now we check if we can find the function pointer to create a new version context. if this
	// is the case we destroy the old context and create a new one using the function pointer
	// we just obtained. should this fail we create again an old context
	PFNWGLCREATECONTEXTATTRIBSARBPROC pwglCreateContextAttribs = nullptr;
	pwglCreateContextAttribs = (PFNWGLCREATECONTEXTATTRIBSARBPROC)
		GetFunctionPointer("wglCreateContextAttribs");
	if(!pwglCreateContextAttribs){
		pwglCreateContextAttribs = (PFNWGLCREATECONTEXTATTRIBSARBPROC)
			GetFunctionPointer("wglCreateContextAttribsARB");
	}
	
	if(pwglCreateContextAttribs){
		logger.LogInfoFormat("Re-creating OpenGL Context using new method");
		
		wglDeleteContext(pContext);
		pContext = nullptr;
		
		int contextFlags = 0;
		
		if(pRTContext.GetRenderThread().GetConfiguration().GetDebugContext()){
			logger.LogInfo("Enable debug context");
			contextFlags |= WGL_CONTEXT_DEBUG_BIT_ARB;
		}
		
		int contextAttribs[] = {
			WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
			WGL_CONTEXT_MINOR_VERSION_ARB, 3,
			WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
			WGL_CONTEXT_FLAGS_ARB, contextFlags,
			0, 0,
			0
		};
		
		int i;
		for(i=0; i<vOpenGLVersionCountW32; i++){
			contextAttribs[1] = vOpenGLVersionsW32[i].major;
			contextAttribs[3] = vOpenGLVersionsW32[i].minor;
			
			pContext = pwglCreateContextAttribs(activeWindow->GetWindowDC(), nullptr, contextAttribs);
			if(pContext){
				logger.LogInfoFormat("- Trying %d.%d Core... Success",
					vOpenGLVersionsW32[i].major, vOpenGLVersionsW32[i].minor);
				pLoaderContext = pwglCreateContextAttribs(activeWindow->GetWindowDC(), pContext, contextAttribs);
				int j;
				for(j=0; j<compileContextCount; j++){
					pCompileContext[j] = pwglCreateContextAttribs(activeWindow->GetWindowDC(), pContext, contextAttribs);
					if(!pCompileContext[j]){
						break;
					}
				}
				pCompileContextCount = j;
				logger.LogInfoFormat("Created %d compile contexts", pCompileContextCount);
				break;
			}
			
			logger.LogInfoFormat("- Trying %d.%d Core... Failed",
				vOpenGLVersionsW32[i].major, vOpenGLVersionsW32[i].minor);
		}
		
		if(!pContext){
			logger.LogWarn("No supported OpenGL Context could be created with new method. "
				"Creating OpenGL Context using old method");
			pContext = wglCreateContext(activeWindow->GetWindowDC());
			pLoaderContext = wglCreateContext(activeWindow->GetWindowDC());
			DEASSERT_TRUE(wglShareLists(pLoaderContext, pContext))
			int i;
			for(i=0; i<compileContextCount; i++){
				pCompileContext[i] = wglCreateContext(activeWindow->GetWindowDC());
				if(!pCompileContext[i]){
					break;
				}
				if(!wglShareLists(pCompileContext[i], pContext)){
					wglDeleteContext(pCompileContext[i]);
					pCompileContext[i] = nullptr;
					break;
				}
			}
			pCompileContextCount = i;
			logger.LogInfoFormat("Created %d compile contexts", pCompileContextCount);
		}
		
	}else{
		logger.LogInfoFormat("Functions required for new OpenGL Context creation not found");
	}
	
	if(!pContext){
		logger.LogErrorFormat("wglCreateContext with code %i", (int)GetLastError());
		DETHROW(deeOutOfMemory);
	}
	DEASSERT_NOTNULL(pLoaderContext)
	
	// attach to context
	wglMakeCurrent(activeWindow->GetWindowDC(), pContext);
	
	// enable HDR output if 10-bit pixel format if supported
	const auto &config = pRTContext.GetRenderThread().GetConfiguration();
	if(pHdrPixelFormat > 0 && config.GetEnableHDRMonitor()){
		const int actualFormat = GetPixelFormat(activeWindow->GetWindowDC());
		if(actualFormat == pHdrPixelFormat && deOSWindows::GetDisplaySupportsHdr()){
			logger.LogInfo("RTCBWindows: HDR output enabled");
			activeWindow->SetUseHdrOutput(true);
			pQueryHdrNits(*activeWindow);
			
		}else{
			logger.LogInfo("RTCBWindows: 10-bit pixel format available but HDR output not supported");
		}
	}
	
	// for windows we have to delay starting the make sure the loader thread is enabled after the loader
	// context is ready. before this point ActivateRRenderWindow() is called and then the loader context
	// is nullptr which causes problems. ActivateRRenderWindow() thus avoids enabling the loader if
	// no loader context exists. we thus have to do it here to make sure enabling works
	pRTContext.GetRenderThread().GetLoaderThread().EnableContext(true);
}

void deoglRTCBWindows::pQueryHdrNits(deoglRRenderWindow &window){
	auto &logger = pRTContext.GetRenderThread().GetLogger();
	
	// query min/max nits via DXGI IDXGIOutput6::GetDesc1()
	IDXGIFactory1 *factory = nullptr;
	float minNits = 0.0f;
	if(SUCCEEDED(CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)&factory))){
		bool found = false;
		IDXGIAdapter *adapter = nullptr;
		for(UINT ai = 0; !found && factory->EnumAdapters(ai, &adapter) != DXGI_ERROR_NOT_FOUND; ai++){
			IDXGIOutput *output = nullptr;
			for(UINT oi = 0; !found && adapter->EnumOutputs(oi, &output) != DXGI_ERROR_NOT_FOUND; oi++){
				IDXGIOutput6 *output6 = nullptr;
				if(SUCCEEDED(output->QueryInterface(__uuidof(IDXGIOutput6), (void**)&output6))){
					DXGI_OUTPUT_DESC1 desc = {};
					if(SUCCEEDED(output6->GetDesc1(&desc))
					&& desc.ColorSpace == DXGI_COLOR_SPACE_RGB_FULL_G2084_NONE_P2020){
						minNits = (float)desc.MinLuminance / 10000.0f;
						window.SetHdrMaxNits((int)desc.MaxLuminance);
						found = true;
					}
					output6->Release();
				}
				output->Release();
			}
			adapter->Release();
		}
		factory->Release();
	}
	
	// query SDR reference white via DISPLAYCONFIG_DEVICE_INFO_GET_SDR_WHITE_LEVEL (type 16,
	// available since Windows 10 version 1709). SDRWhiteLevel is in units of
	// (1000 * nits / 80), so nits = SDRWhiteLevel * 80 / 1000. Standard SDR white is
	// 1000 (80 nits); typical Windows HDR setting is 2000-4000 (160-320 nits).
	// PQ HDR reference white 203 nits is returned when the compositor uses that level.
	struct sSDRWhiteLevel{
		DISPLAYCONFIG_DEVICE_INFO_HEADER header;
		UINT32 SDRWhiteLevel;
	};
	static const auto kTypeSDRWhiteLevel = (DISPLAYCONFIG_DEVICE_INFO_TYPE)16;
	
	UINT32 pathCount = 0, modeCount = 0;
	if(GetDisplayConfigBufferSizes(QDC_ONLY_ACTIVE_PATHS, &pathCount, &modeCount) == ERROR_SUCCESS){
		decTUniqueList<DISPLAYCONFIG_PATH_INFO> paths(pathCount);
		decTUniqueList<DISPLAYCONFIG_MODE_INFO> modes(modeCount);
		
		if(QueryDisplayConfig(QDC_ONLY_ACTIVE_PATHS, &pathCount, paths.GetArrayPointer(),
		&modeCount, modes.GetArrayPointer(), nullptr) == ERROR_SUCCESS){
			for(UINT32 i = 0; i < pathCount; i++){
				sSDRWhiteLevel sdrInfo{};
				sdrInfo.header.type = kTypeSDRWhiteLevel;
				sdrInfo.header.size = sizeof(sdrInfo);
				sdrInfo.header.adapterId = paths[i].targetInfo.adapterId;
				sdrInfo.header.id = paths[i].targetInfo.id;
				
				if(DisplayConfigGetDeviceInfo(&sdrInfo.header) == ERROR_SUCCESS){
					const int refNits = (int)(sdrInfo.SDRWhiteLevel * 80 / 1000);
					window.SetHdrReferenceNits(refNits);
					logger.LogInfoFormat(
						"RTCBWindows: HDR capabilities: minNits=%.4f maxNits=%d refNits=%d",
						minNits, window.GetHdrMaxNits(), refNits);
					break;
				}
			}
		}
	}
}

void deoglRTCBWindows::pUnregisterWindowClass(){
	wchar_t wideClassName[200];
	deOSWindows::Utf8ToWide(pWindowClassname, wideClassName, MAX_PATH);
	UnregisterClass(wideClassName, pOSWindows->GetInstApp());
	vW32WndFuncContext = nullptr;
}

void deoglRTCBWindows::pFreeContext(){
	int i;
	for(i=0; i<pCompileContextCount; i++){
		if(pCompileContext[i]){
			wglDeleteContext(pCompileContext[i]);
			pCompileContext[i] = nullptr;
		}
	}
	if(pLoaderContext){
		wglDeleteContext(pLoaderContext);
		pLoaderContext = nullptr;
	}
	if(pContext){
		pRTContext.GetRenderThread().GetLogger().LogInfo("Free Context");
		wglDeleteContext(pContext);
		pContext = nullptr;
	}
}

void deoglRTCBWindows::pChooseHdrPixelFormat(){
	// SetPixelFormat() can be called only once per window DC so the pixel format has to be
	// chosen BEFORE deoglRRenderWindow::CreateWindow() is called. wglChoosePixelFormatARB
	// requires an active OpenGL context to retrieve the function pointer. To resolve this
	// chicken-and-egg problem a temporary dummy window and context is created to probe for
	// the 10-bit pixel format
	deoglRTLogger &logger = pRTContext.GetRenderThread().GetLogger();
	
	// create tiny invisible dummy window
	wchar_t wideClassName[200];
	deOSWindows::Utf8ToWide(pWindowClassname, wideClassName, MAX_PATH);
	const HWND dummyHwnd = CreateWindowExW(0, wideClassName, L"DEOpenGLDummy",
		WS_OVERLAPPED, 0, 0, 1, 1, nullptr, nullptr, pOSWindows->GetInstApp(), nullptr);
	if(!dummyHwnd){
		logger.LogInfo("RTCBWindows.pChooseHdrPixelFormat: CreateWindowEx failed, skipping HDR probe");
		return;
	}
	
	const HDC dummyDC = GetDC(dummyHwnd);
	if(!dummyDC){
		DestroyWindow(dummyHwnd);
		logger.LogInfo("RTCBWindows.pChooseHdrPixelFormat: GetDC failed, skipping HDR probe");
		return;
	}
	
	// set basic pixel format
	PIXELFORMATDESCRIPTOR pfd{};
	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 24;
	pfd.cRedBits = 8;
	pfd.cGreenBits = 8;
	pfd.cBlueBits = 8;
	pfd.cAlphaBits = 8;
	pfd.cDepthBits = 24;
	
	if(!SetPixelFormat(dummyDC, ChoosePixelFormat(dummyDC, &pfd), &pfd)){
		ReleaseDC(dummyHwnd, dummyDC);
		DestroyWindow(dummyHwnd);
		logger.LogInfo("RTCBWindows.pChooseHdrPixelFormat: SetPixelFormat failed, skipping HDR probe");
		return;
	}
	
	// create basic context
	const HGLRC dummyCtx = wglCreateContext(dummyDC);
	if(!dummyCtx){
		ReleaseDC(dummyHwnd, dummyDC);
		DestroyWindow(dummyHwnd);
		logger.LogInfo("RTCBWindows.pChooseHdrPixelFormat: wglCreateContext failed, skipping HDR probe");
		return;
	}
	
	wglMakeCurrent(dummyDC, dummyCtx);
	
	auto pwglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)
		wglGetProcAddress("wglChoosePixelFormatARB");
	
	if(pwglChoosePixelFormatARB){
		// try to find a 10-bit per channel pixel format
		const int attribs[] = {
			WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
			WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
			WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
			WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
			WGL_RED_BITS_ARB, 10,
			WGL_GREEN_BITS_ARB, 10,
			WGL_BLUE_BITS_ARB, 10,
			WGL_ALPHA_BITS_ARB, 2,
			WGL_DEPTH_BITS_ARB, 24,
			WGL_STENCIL_BITS_ARB, 8,
			0
		};
		
		int format = 0;
		UINT numFormats = 0;
		if(pwglChoosePixelFormatARB(dummyDC, attribs, nullptr, 1, &format, &numFormats)
		&& numFormats > 0){
			pHdrPixelFormat = format;
			logger.LogInfo("RTCBWindows.pChooseHdrPixelFormat: 10-bit HDR pixel format");
			
		}else{
			logger.LogInfo("RTCBWindows.pChooseHdrPixelFormat: No 10-bit HDR pixel format available");
		}
		
	}else{
		logger.LogInfo("RTCBWindows.pChooseHdrPixelFormat: wglChoosePixelFormatARB not available, skipping HDR probe");
	}
	
	wglMakeCurrent(nullptr, nullptr);
	wglDeleteContext(dummyCtx);
	ReleaseDC(dummyHwnd, dummyDC);
	DestroyWindow(dummyHwnd);
}

#endif // OS_W32
