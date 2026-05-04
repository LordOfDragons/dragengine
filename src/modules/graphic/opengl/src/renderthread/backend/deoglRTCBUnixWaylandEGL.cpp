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

#if defined(OS_UNIX_X11) && defined(OS_UNIX_WAYLAND) && defined(BACKEND_OPENGL)

#include "deoglRTCBUnixWaylandEGL.h"

#include <dlfcn.h>
#include <string.h>

#include "../deoglRTContext.h"
#include "../deoglRenderThread.h"
#include "../deoglRTLogger.h"
#include "../../deGraphicOpenGl.h"
#include "../../configuration/deoglConfiguration.h"
#include "../../window/deoglRRenderWindow.h"

#include <dragengine/app/deOSUnix.h>
#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>


// Class deoglRTCBUnixWaylandEGL
//////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRTCBUnixWaylandEGL::deoglRTCBUnixWaylandEGL(deoglRTContext &context) :
deoglRTCBUnixX11EGL(context),
pWlCompositor(nullptr),
pXdgWmBase(nullptr),
pDecorationManager(nullptr),
pDecoration(nullptr),
pWpFractionalScaleManager(nullptr),
pColorManager(nullptr),
pWpViewporter(nullptr),
pWlCompositorId(0),
pXdgWmBaseId(0),
pXdgWmBaseVersion(0),
pDecorationManagerId(0),
pWpFractionalScaleManagerId(0),
pColorManagerId(0),
pWpViewporterId(0),
pWaylandReady(false){
}

deoglRTCBUnixWaylandEGL::~deoglRTCBUnixWaylandEGL(){
	CleanUp();
}


// Management
///////////////

bool deoglRTCBUnixWaylandEGL::TryInit(){
	deoglRTLogger &logger = pRTContext.GetRenderThread().GetLogger();
	
	if(!pOSUnix->GetWaylandDisplay()){
		logger.LogInfo("RTCBUnixWaylandEGL: Wayland display not available");
		return false;
	}
	
	if(!pOSUnix->GetEnableWayland()){
		logger.LogInfo("RTCBUnixWaylandEGL: Wayland display available but disabled by application");
		return false;
	}
	
	logger.LogInfo("RTCBUnixWaylandEGL: Wayland display available, load wayland client library");
	
	if(!pInitWayland()){
		return false;
	}
	
	// try loading EGL
	if(!deoglRTCBUnixX11EGL::TryInit()){
		return false;
	}
	
	logger.LogInfo("RTCBUnixWaylandEGL: Using Wayland + EGL backend");
	pWaylandReady = true;
	return true;
}

void deoglRTCBUnixWaylandEGL::InitPhase2(){
	deoglRTLogger &logger = pRTContext.GetRenderThread().GetLogger();
	logger.LogInfo("RTCBUnixWaylandEGL: Initializing Wayland compositor");
	
	pRegisterWaylandCompositor();
	pChooseVisual();
	pCreateContext();
	
	logger.LogInfo("deoglRTCBUnixWaylandEGL::InitPhase2: Done");
}

void deoglRTCBUnixWaylandEGL::CleanUp(){
	pUnregisterWaylandCompositor();
	pFreeContext();
	deoglRTCBUnixX11EGL::CleanUp();
}

void deoglRTCBUnixWaylandEGL::ProcessEventLoop(){
	if(!pWaylandReady){
		return;
	}
	
	if(pOSUnix){
		wl_display_flush(pOSUnix->GetWaylandDisplay());
		wl_display_dispatch_pending(pOSUnix->GetWaylandDisplay());
	}
}

void deoglRTCBUnixWaylandEGL::AssignOSWindow(deRenderWindow*){
	// for Wayland, there is no X11 window to assign to the OS. the Wayland surface is managed
	// entirely by the render window
}

void deoglRTCBUnixWaylandEGL::CreateWindowSurface(deoglRRenderWindow &window){
	if(window.GetEGLSurface() != EGL_NO_SURFACE){
		return;
	}
	
	if(!pWlCompositor || !pXdgWmBase){
		DETHROW_INFO(deeInvalidAction, "Wayland compositor not initialized");
	}
	
	deoglRTLogger &logger = pRTContext.GetRenderThread().GetLogger();
	
	// create Wayland surface
	auto wlSurface = wl_compositor_create_surface(pWlCompositor);
	if(!wlSurface){
		DETHROW_INFO(deeInvalidAction, "wl_compositor_create_surface failed");
	}
	
	// create xdg_surface from xdg_wm_base
	auto xdgSurface = (xdg_surface*)xdg_wm_base_get_xdg_surface(pXdgWmBase, wlSurface);
	if(!xdgSurface){
		wl_proxy_destroy((wl_proxy*)wlSurface);
		DETHROW_INFO(deeInvalidAction, "xdg_wm_base_get_xdg_surface failed");
	}
	
	// create xdg_toplevel from xdg_surface
	auto xdgToplevel = (xdg_toplevel*)xdg_surface_get_toplevel(xdgSurface);
	if(!xdgToplevel){
		wl_proxy_destroy((wl_proxy*)xdgSurface);
		wl_proxy_destroy((wl_proxy*)wlSurface);
		DETHROW_INFO(deeInvalidAction, "xdg_surface_get_toplevel failed");
	}
	
	// create viewport for window surface
	auto viewport = wp_viewporter_get_viewport(pWpViewporter, wlSurface);
	wl_surface_set_buffer_scale(wlSurface, 1);
	
	// set window title on xdg_toplevel
	xdg_toplevel_set_title(xdgToplevel, window.GetTitle());
	decString appId(pRTContext.GetRenderThread().GetOgl().GetGameEngine()->GetCacheAppID());
	if(appId.IsEmpty()){
		appId.Set("default");
	}
	xdg_toplevel_set_app_id(xdgToplevel, decString::Formatted("ch.dragondreams.dragengine.{0}", appId));
	
	// store Wayland objects in render window
	window.SetWlSurface(wlSurface);
	window.SetXdgSurface(xdgSurface);
	window.SetXdgToplevel(xdgToplevel);
	window.SetWpViewport(viewport);
	
	// add xdg_surface configure listener
	static const xdg_surface_listener xdgSurfaceListener = {
		deoglRRenderWindow::OnXdgSurfaceConfigure
	};
	wl_proxy_add_listener((wl_proxy*)xdgSurface, (void(**)(void))&xdgSurfaceListener, &window);
	
	// add xdg_toplevel configure/close listener
	static const xdg_toplevel_listener xdgToplevelListener = {
		deoglRRenderWindow::OnXdgToplevelConfigure,
		deoglRRenderWindow::OnXdgToplevelClose,
		deoglRRenderWindow::OnXdgToplevelConfigureBounds,
		deoglRRenderWindow::OnXdgToplevelWmCapabilities
	};
	wl_proxy_add_listener((wl_proxy*)xdgToplevel, (void(**)(void))&xdgToplevelListener, &window);
	
	// create fractional scale object if compositor supports the protocol.
	// must be done before the commit so the compositor can send preferred_scale during roundtrip
	CreateFractionalScaleObject(window);
	
	// request server side decoration if supported
	if(pDecorationManager){
		pDecoration = zxdg_decoration_manager_v1_get_toplevel_decoration(
			pDecorationManager, xdgToplevel);
		if(pDecoration){
			zxdg_toplevel_decoration_v1_set_mode(pDecoration,
				ZXDG_TOPLEVEL_DECORATION_V1_MODE_SERVER_SIDE);
			logger.LogInfo("RTCBUnixWaylandEGL.CreateWindowSurface: Requested server side decorations");
			
		}else{
			logger.LogInfo("RTCBUnixWaylandEGL.CreateWindowSurface: Failed to create decoration manager");
		}
	}
	
	// fullscreen
	if(window.GetFullScreen()){
		xdg_toplevel_set_fullscreen(xdgToplevel, nullptr);
	}
	
	// commit surface to trigger initial configure. attached managers are then frozen
	wl_surface_commit(wlSurface);
	if(pOSUnix){
		wl_display_roundtrip(pOSUnix->GetWaylandDisplay());
	}
	
	// calculate window size respecting global scaling
	int renderWidth = window.GetWidth();
	int renderHeight = window.GetHeight();
	int viewportWidth = renderWidth;
	int viewportHeight = renderHeight;
	int scaleFactor = 100;
	
	if(viewport){
		scaleFactor = window.GetAfterCreateScaleFactor();
		viewportWidth = renderWidth * 100 / scaleFactor;
		viewportHeight = renderHeight * 100 / scaleFactor;
		
		wp_viewport_set_destination(viewport, viewportWidth, viewportHeight);
		xdg_surface_set_window_geometry(xdgSurface, 0, 0, viewportWidth, viewportHeight);
	}
	
	// create wl_egl_window
	wl_egl_window * const wlEglWindow = deoglWlEglWindowCreate(wlSurface, renderWidth, renderHeight);
	if(!wlEglWindow){
		wl_proxy_destroy((wl_proxy*)xdgToplevel);
		wl_proxy_destroy((wl_proxy*)xdgSurface);
		wl_proxy_destroy((wl_proxy*)wlSurface);
		window.SetWlSurface(nullptr);
		window.SetXdgSurface(nullptr);
		window.SetXdgToplevel(nullptr);
		DETHROW_INFO(deeInvalidAction, "wl_egl_window_create failed");
	}
	
	window.SetWlEglWindow(wlEglWindow);
	
	// create EGL surface from wl_egl_window
	EGLSurface surface = pEglCreateWindowSurface(pEGLDisplay, pEGLConfig,
		(EGLNativeWindowType)wlEglWindow, nullptr);
	if(surface == EGL_NO_SURFACE){
		deoglWlEglWindowDestroy(wlEglWindow);
		wl_proxy_destroy((wl_proxy*)xdgToplevel);
		wl_proxy_destroy((wl_proxy*)xdgSurface);
		wl_proxy_destroy((wl_proxy*)wlSurface);
		window.SetWlSurface(nullptr);
		window.SetXdgSurface(nullptr);
		window.SetXdgToplevel(nullptr);
		window.SetWlEglWindow(nullptr);
		DETHROW_INFO(deeInvalidAction, "eglCreateWindowSurface failed for Wayland render window");
	}
	
	window.SetEGLSurface(surface);
	
	//deoglWlEglWindowResize(wlEglWindow, renderWidth, renderHeight, 0, 0);
	wl_surface_commit(wlSurface);
	if(pOSUnix){
		wl_display_roundtrip(pOSUnix->GetWaylandDisplay());
	}
	
	logger.LogInfoFormat(
		"RTCBUnixWaylandEGL.CreateWindowSurface: Created (scale=%d size=%dx%d render=%dx%d)",
		scaleFactor, viewportWidth, viewportHeight, renderWidth, renderHeight);
}

void deoglRTCBUnixWaylandEGL::DestroyWindowSurface(deoglRRenderWindow &window){
	if(window.GetEGLSurface() != EGL_NO_SURFACE){
		pEglDestroySurface(pEGLDisplay, window.GetEGLSurface());
		window.SetEGLSurface(EGL_NO_SURFACE);
	}
	
	if(window.GetWlEglWindow()){
		deoglWlEglWindowDestroy(window.GetWlEglWindow());
		window.SetWlEglWindow(nullptr);
	}
	
	if(window.GetWpViewport()){
		wp_viewport_destroy(window.GetWpViewport());
		window.SetWpViewport(nullptr);
	}
	if(window.GetWpFractionalScale()){
		wp_fractional_scale_v1_destroy(window.GetWpFractionalScale());
		window.SetWpFractionalScale(nullptr);
	}
	if(window.GetXdgToplevel()){
		xdg_toplevel_destroy(window.GetXdgToplevel());
		window.SetXdgToplevel(nullptr);
	}
	if(window.GetXdgSurface()){
		xdg_surface_destroy(window.GetXdgSurface());
		window.SetXdgSurface(nullptr);
	}
	if(window.GetWlSurface()){
		wl_proxy_destroy((wl_proxy*)window.GetWlSurface());
		window.SetWlSurface(nullptr);
	}
}


// Protected Functions
////////////////////////

void deoglRTCBUnixWaylandEGL::pOpenDisplay(){
	// wayland display is managed by deOSUnix. x11 pDisplay stays nullptr
}

void deoglRTCBUnixWaylandEGL::pChooseVisual(){
	// skip X11 visual selection and call EGL config selection directly. pVisInfo stays null
	pChooseConfig();
}

void deoglRTCBUnixWaylandEGL::pChooseConfig(){
	if(!pOSUnix){
		DETHROW_INFO(deeInvalidAction, "deOSUnix not available for Wayland EGL display");
	}
	
	// get EGL display using Wayland platform. try first EGL_EXT_platform_base extension for
	// proper platform selection. if this fails fall back to eglGetDisplay with wl_display cast
	pEGLDisplay = pEglGetDisplay((EGLNativeDisplayType)pOSUnix->GetWaylandDisplay());
	
	if(pEGLDisplay == EGL_NO_DISPLAY){
		DETHROW_INFO(deeInvalidAction, "eglGetDisplay with Wayland display failed");
	}
	
	deoglRTLogger &logger = pRTContext.GetRenderThread().GetLogger();
	
	EGLint eglMajor, eglMinor;
	if(pEglInitialize(pEGLDisplay, &eglMajor, &eglMinor) == EGL_FALSE){
		DETHROW_INFO(deeInvalidAction, "eglInitialize with Wayland display failed");
	}
	logger.LogInfoFormat("RTCBUnixWaylandEGL: EGL version: %d.%d (Wayland platform)", eglMajor, eglMinor);
	
	if(pEglBindAPI(EGL_OPENGL_API) == EGL_FALSE){
		DETHROW_INFO(deeInvalidAction, "eglBindAPI(EGL_OPENGL_API) failed");
	}
	
	const EGLint configAttribs[] = {
		EGL_SURFACE_TYPE, EGL_WINDOW_BIT /*| EGL_PBUFFER_BIT*/,
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
	if(pEglChooseConfig(pEGLDisplay, configAttribs, &pEGLConfig, 1, &numConfigs) == EGL_FALSE){
		DETHROW_INFO(deeInvalidAction, decString::Formatted(
			"eglChooseConfig for Wayland failed (0x{:x})", pEglGetError()));
	}
	if(numConfigs == 0){
		DETHROW_INFO(deeInvalidAction, "eglChooseConfig for Wayland return 0 entries");
	}
	
	logger.LogInfo("RTCBUnixWaylandEGL: EGL config selected (Wayland)");
}


// Private Functions
//////////////////////

bool deoglRTCBUnixWaylandEGL::pInitWayland(){
	deoglRTLogger &logger = pRTContext.GetRenderThread().GetLogger();
	
	bool v = true;
	v &= deWaylandEglLib.RequiredLog(logger);
	v &= deoglWlEglWindowCreate.RequiredLog(logger);
	v &= deoglWlEglWindowDestroy.RequiredLog(logger);
	v &= deoglWlEglWindowResize.RequiredLog(logger);
	
	if(!v){
		logger.LogInfo("RTCBUnixWaylandEGL: Wayland libraries not available");
		return false;
	}
	
	logger.LogInfo("RTCBUnixWaylandEGL: Wayland libraries loaded");
	return true;
}

void deoglRTCBUnixWaylandEGL::pRegisterWaylandCompositor(){
	deoglRTLogger &logger = pRTContext.GetRenderThread().GetLogger();
	
	if(!pOSUnix || !pOSUnix->GetWaylandDisplay()){
		DETHROW_INFO(deeInvalidAction, "Wayland display not available");
	}
	
	auto wlDisplay = pOSUnix->GetWaylandDisplay();
	
	// get Wayland registry
	auto registry = wl_display_get_registry(wlDisplay);
	if(!registry){
		DETHROW_INFO(deeInvalidAction, "wl_display_get_registry failed");
	}
	
	// add registry listener to discover globals
	static const wl_registry_listener registryListener = {
		pOnRegistryGlobal,
		pOnRegistryGlobalRemove
	};
	wl_registry_add_listener(registry, &registryListener, this);
	
	// roundtrip to get all globals
	wl_display_roundtrip(wlDisplay);
	
	// all required data collected. registry proxy is not needed anymore
	wl_proxy_destroy((wl_proxy*)registry);
	
	// roundtrip to process any deferred binding responses
	wl_display_roundtrip(wlDisplay);
	
	if(!pWlCompositor){
		DETHROW_INFO(deeInvalidAction, "wl_compositor not found in Wayland registry");
	}
	if(!pXdgWmBase){
		DETHROW_INFO(deeInvalidAction, "xdg_wm_base not found in Wayland registry");
	}
	
	// add xdg_wm_base ping listener (required by protocol)
	static const xdg_wm_base_listener xdgWmBaseListener = {
		pOnXdgWmBasePing
	};
	xdg_wm_base_add_listener(pXdgWmBase, &xdgWmBaseListener, this);
	
	logger.LogInfo("RTCBUnixWaylandEGL: Wayland compositor and xdg_wm_base ready");
}

void deoglRTCBUnixWaylandEGL::pUnregisterWaylandCompositor(){
	if(pDecoration){
		zxdg_toplevel_decoration_v1_destroy(pDecoration);
		pDecoration = nullptr;
	}
	if(pDecorationManager){
		zxdg_decoration_manager_v1_destroy(pDecorationManager);
		pDecorationManager = nullptr;
	}
	if(pXdgWmBase){
		xdg_wm_base_destroy(pXdgWmBase);
		pXdgWmBase = nullptr;
	}
	
	if(pWpFractionalScaleManager){
		wp_fractional_scale_manager_v1_destroy(pWpFractionalScaleManager);
		pWpFractionalScaleManager = nullptr;
	}
	
	if(pWlCompositor){
		wl_compositor_destroy(pWlCompositor);
		pWlCompositor = nullptr;
	}
}


// Registry listener callbacks
////////////////////////////////

void deoglRTCBUnixWaylandEGL::CreateFractionalScaleObject(deoglRRenderWindow &window){
	if(!pWpFractionalScaleManager || !window.GetWlSurface()){
		return;
	}
	
	auto fractionalScale = wp_fractional_scale_manager_v1_get_fractional_scale(
		pWpFractionalScaleManager, window.GetWlSurface());
	if(!fractionalScale){
		return; // optional feature
	}
	
	static const wp_fractional_scale_v1_listener fractionalScaleListener = {
		deoglRRenderWindow::OnWpFractionalScalePreferredScale
	};
	wp_fractional_scale_v1_add_listener(fractionalScale, &fractionalScaleListener, &window);
	
	window.SetWpFractionalScale(fractionalScale);
}

void deoglRTCBUnixWaylandEGL::pOnRegistryGlobal(void *data, wl_registry *registry,
uint32_t name, const char *interface, uint32_t version){
	auto backend = (deoglRTCBUnixWaylandEGL*)data;
	
	if(strcmp(interface, wl_compositor_interface.name) == 0 && !backend->pWlCompositor){
		backend->pWlCompositor = (wl_compositor*)wl_registry_bind(registry, name,
			&wl_compositor_interface, decMath::min(version, 4));
		backend->pWlCompositorId = name;
		backend->pRTContext.GetRenderThread().GetLogger().LogInfo(
			"RTCBUnixWaylandEGL: Found interface wl_compositor");
		
	}else if(strcmp(interface, xdg_wm_base_interface.name) == 0 && !backend->pXdgWmBase){
		const uint32_t bindVersion = (uint32_t)decMath::min(version, 4);
		backend->pXdgWmBase = (xdg_wm_base*)wl_registry_bind(registry, name,
			&xdg_wm_base_interface, bindVersion);
		backend->pXdgWmBaseId = name;
		backend->pXdgWmBaseVersion = bindVersion;
		backend->pRTContext.GetRenderThread().GetLogger().LogInfo(
			"RTCBUnixWaylandEGL: Found interface xdg_wm_base");
		
	}else if(strcmp(interface, wp_fractional_scale_manager_v1_interface.name) == 0
	&& !backend->pWpFractionalScaleManager){
		backend->pWpFractionalScaleManager = (wp_fractional_scale_manager_v1*)wl_registry_bind(
			registry, name, &wp_fractional_scale_manager_v1_interface, 1);
		backend->pWpFractionalScaleManagerId = name;
		backend->pRTContext.GetRenderThread().GetLogger().LogInfo(
			"RTCBUnixWaylandEGL: Found interface wp_fractional_scale_manager_v1");
		
	}else if(strcmp(interface, zxdg_decoration_manager_v1_interface.name) == 0
	&& !backend->pDecorationManager){
		backend->pDecorationManager = (zxdg_decoration_manager_v1*)wl_registry_bind(
			registry, name, &zxdg_decoration_manager_v1_interface, 1);
		backend->pDecorationManagerId = name;
		backend->pRTContext.GetRenderThread().GetLogger().LogInfo(
			"RTCBUnixWaylandEGL: Found interface zxdg_decoration_manager_v1");
			
	}else if(strcmp(interface, wp_color_manager_v1_interface.name) == 0
	&& !backend->pColorManager){
		backend->pColorManager = (wp_color_manager_v1*)wl_registry_bind(
			registry, name, &wp_color_manager_v1_interface, 1);
		backend->pColorManagerId = name;
		backend->pRTContext.GetRenderThread().GetLogger().LogInfo(
			"RTCBUnixWaylandEGL: Found interface wp_color_manager_v1");
		
	}else if(strcmp(interface, wp_viewporter_interface.name) == 0
	&& !backend->pWpViewporter){
		backend->pWpViewporter = (wp_viewporter*)wl_registry_bind(
			registry, name, &wp_viewporter_interface, 1);
		backend->pWpViewporterId = name;
		backend->pRTContext.GetRenderThread().GetLogger().LogInfo(
			"RTCBUnixWaylandEGL: Found interface wp_viewporter");
	}
}

void deoglRTCBUnixWaylandEGL::pOnRegistryGlobalRemove(void *data, wl_registry*, uint32_t name){
	// handle removal of globals if needed (e.g. compositor disconnected)
	auto backend = (deoglRTCBUnixWaylandEGL*)data;
	
	if(name == backend->pWlCompositorId){
		backend->pWlCompositor = nullptr;
		
	}else if(name == backend->pXdgWmBaseId){
		backend->pXdgWmBase = nullptr;
		
	}else if(name == backend->pWpFractionalScaleManagerId){
		backend->pWpFractionalScaleManager = nullptr;
	}
}


// xdg_wm_base ping handler
/////////////////////////////

void deoglRTCBUnixWaylandEGL::pOnXdgWmBasePing(void*, xdg_wm_base *base, uint32_t serial){
	xdg_wm_base_pong(base, serial);
}

#endif // OS_UNIX_X11 && BACKEND_OPENGL
