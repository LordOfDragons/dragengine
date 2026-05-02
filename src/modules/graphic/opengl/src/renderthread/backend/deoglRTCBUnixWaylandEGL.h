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

#ifndef _DEOGLRTCBUNIXWAYLANDEGL_H_
#define _DEOGLRTCBUNIXWAYLANDEGL_H_

#include <dragengine/dragengine_configuration.h>

#if defined(OS_UNIX_X11) && defined(OS_UNIX_WAYLAND) && defined(BACKEND_OPENGL)

#include "deoglRTCBUnixX11EGL.h"
#include "../../extensions/wayland/wayland-defs.h"


/**
 * Wayland EGL OpenGL context backend for Unix.
 * 
 * Extends deoglRTCBUnixX11EGL to use Wayland for display and window management.
 */
class deoglRTCBUnixWaylandEGL : public deoglRTCBUnixX11EGL{
private:
	// Wayland compositor global objects
	wl_compositor *pWlCompositor;
	xdg_wm_base *pXdgWmBase;
	zxdg_decoration_manager_v1 *pDecorationManager;
	zxdg_toplevel_decoration_v1 *pDecoration;
	wp_fractional_scale_manager_v1 *pWpFractionalScaleManager;
	wp_color_manager_v1 *pColorManager;
	uint32_t pWlCompositorId;
	uint32_t pXdgWmBaseId;
	uint32_t pXdgWmBaseVersion;
	uint32_t pDecorationManagerId;
	uint32_t pWpFractionalScaleManagerId;
	uint32_t pColorManagerId;
	
	bool pWaylandReady;
	
	

public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create Wayland EGL backend. */
	explicit deoglRTCBUnixWaylandEGL(deoglRTContext &context);
	
	/** Clean up Wayland EGL backend. */
	~deoglRTCBUnixWaylandEGL() override;
	/*@}*/
	
	
	/** \name Management */
	/*@{*/
	/**
	 * Try to initialize. Returns false if the required library is not available
	 * or required symbols are missing. Throws on other failures.
	 */
	bool TryInit() override;
	
	/** Initialize phase 2. */
	void InitPhase2() override;
	
	/** Clean up all resources. */
	void CleanUp() override;
	
	/** Process event loop. */
	void ProcessEventLoop() override;
	
	/** Assign OS window after render window is created. */
	void AssignOSWindow(deRenderWindow *renderWindow) override;
	
	/** Create surface for window. */
	void CreateWindowSurface(deoglRRenderWindow &window) override;
	
	/** Destroy surface for window. */
	void DestroyWindowSurface(deoglRRenderWindow &window) override;
	
	/** Create fractional scale object for window surface if protocol is available. */
	void CreateFractionalScaleObject(deoglRRenderWindow &window);
	
	
	/** Acknowledge xdg_surface configure event (sends ack_configure). */
	void AckXdgSurfaceConfigure(deoglRRenderWindow &window, uint32_t serial);
	
	
	/** Wayland compositor. */
	inline wl_compositor *GetWlCompositor() const{ return pWlCompositor; }
	
	/** xdg_wm_base global. */
	inline xdg_wm_base *GetXdgWmBase() const{ return pXdgWmBase; }
	
	/** wp_fractional_scale_manager_v1 global (nullptr if protocol not available). */
	inline wp_fractional_scale_manager_v1 *GetWpFractionalScaleManager() const{ return pWpFractionalScaleManager; }
	/*@}*/
	
	
protected:
	void pOpenDisplay() override;
	void pChooseVisual() override;
	void pChooseConfig() override;
	
	
private:
	bool pInitWayland();
	void pRegisterWaylandCompositor();
	void pUnregisterWaylandCompositor();
	
	/** Registry listener callbacks */
	static void pOnRegistryGlobal(void *data, wl_registry *registry,
		uint32_t name, const char *interface, uint32_t version);
	
	static void pOnRegistryGlobalRemove(void *data, wl_registry *registry, uint32_t name);
	
	/** xdg_wm_base listener callbacks */
	static void pOnXdgWmBasePing(void *data, xdg_wm_base *base, uint32_t serial);
};

#endif // OS_UNIX_X11 && BACKEND_OPENGL
#endif // _DEOGLRTCBUNIXWAYLANDEGL_H_
