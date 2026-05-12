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

#ifndef _DEOGLRRENDERWINDOW_H_
#define _DEOGLRRENDERWINDOW_H_

#include "../configuration/deoglConfiguration.h"
#include "../canvas/render/deoglRCanvasView.h"
#include "../target/deoglRenderTargetArray.h"
#include "../texture/pixelbuffer/deoglPixelBuffer.h"

#include <dragengine/deObject.h>
#include <dragengine/deTUniqueReference.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/threading/deMutex.h>

#ifdef OS_W32
#include "../include_windows.h"

#elif defined OS_BEOS
#include <DirectWindow.h>
#include <GLView.h>

#elif defined OS_MACOS
#ifdef __OBJC__
@class NSWindow;
@class NSView;
#else
class NSWindow;
class NSView;
#endif

#elif defined OS_UNIX_X11
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#	ifdef BACKEND_OPENGL
#		include "../extensions/egl.h"
#		ifdef OS_UNIX_WAYLAND
#			include "../extensions/wayland/wayland-defs.h"
#		endif
#	endif
#endif

class deoglRenderThread;


/**
 * Render window target.
 */
class deoglRRenderWindow : public deObject{
public:
	/** \brief Type holding strong reference. */
	using Ref = deTObjectReference<deoglRRenderWindow>;
	
	
private:
	deoglRenderThread &pRenderThread;
	
#ifdef OS_BEOS
	class cGLView;
	class cGLWindow;
	
	class cGLView : public BGLView{
	private:
		cGLWindow &pWindow;
	public:
		cGLView(cGLWindow &widow, const BRect &frame);
		void KeyDown(const char *bytes, int32 numBytes) override;
		void KeyUp(const char *bytes, int32 numBytes) override;
		void MouseDown(BPoint point) override;
		void MouseMoved(BPoint point, uint32 transit, const BMessage *message) override;
		void MouseUp(BPoint point) override;
	};
	
	class cGLWindow : public BDirectWindow{
	private:
		deoglRRenderWindow &pWindow;
		cGLView *pGLView;
		BCursor *pCursor;
		bool pBlockQuitRequested;
	public:
		cGLWindow(deoglRRenderWindow &window);
		~cGLWindow() override;
		inline deoglRRenderWindow &GetWindow() const{ return pWindow; }
		inline cGLView *GetGLView() const{ return pGLView; }
		void SendCurMessageToEngine();
		void DirectConnected(direct_buffer_info *info) override;
		void WindowActivated(bool active) override;
		void MessageReceived(BMessage *message) override;
		void FrameResized(float newWidth, float newHeight) override;
		bool QuitRequested() override;
		void SetBlockQuitRequested(bool blockQuitRequested);
	};
	
	BWindow *pHostWindow;
	cGLWindow *pWindow;
	
#elif defined OS_ANDROID
	
#elif defined OS_WEBWASM
	
#elif defined OS_W32
	HWND pHostWindow;
	HWND pWindow;
	HDC pWindowDC;
	HICON pWindowIcon;
	
#elif defined OS_MACOS
	NSWindow *pHostWindow;
	NSWindow *pWindow;
	NSView *pView;
	
#elif defined OS_UNIX_X11
	Window pHostWindow;
	Window pWindow;
	Cursor pNullCursor;
	
#ifdef OS_UNIX_WAYLAND
	wl_surface *pWlSurface;
	xdg_surface *pXdgSurface;
	xdg_toplevel *pXdgToplevel;
	wl_egl_window *pWlEglWindow;
	wp_fractional_scale_v1 *pWpFractionalScale;
	wp_viewport *pWpViewport;
	wp_color_management_surface_v1 *pWpColorSurface;
	
	wl_shm_pool *pWlIconShmPool;
	wl_buffer *pWlIconBuffer;
	void *pWlIconShmData;
	size_t pWlIconShmSize;
	int pWlIconShmFd;
	
	struct sConfigureResize{
		uint32_t serial = 0;
		decPoint viewportSize{}, renderSize{};
		bool requested = false;
	};
	deMutex pMutexCommitConfigure;
	sConfigureResize pLastConfigureResize, pCommitConfigureResize, pRenderConfigureResize;
#endif
	
#	ifdef BACKEND_OPENGL
		EGLSurface pEGLSurface;
#	endif
#endif
	
	int pX;
	int pY;
	int pWidth;
	int pHeight;
	decString pTitle;
	bool pFullScreen;
	bool pPaint;
	deoglPixelBuffer::Ref pIcon;
	
	deoglRCanvasView::Ref pRCanvasView;
	
	bool pSwapBuffers;
	bool pNotifyPositionChanged;
	bool pNotifySizeChanged;
	
	deoglConfiguration::eVSyncMode pVSyncMode;
	bool pInitSwapInterval;
	
	deoglRenderTargetArray::Ref pRenderTarget;
	
	int pAfterCreateScaleFactor;
	bool pUseHdrOutput;
	int pHdrMaxNits, pHdrReferenceNits;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create render render target. */
	explicit deoglRRenderWindow(deoglRenderThread &renderThread);
	
protected:
	/** Clean up render render target. */
	~deoglRRenderWindow() override;
	/*@}*/
	
	
public:
	/** \name Management */
	/*@{*/
	/** Render thread. */
	inline deoglRenderThread &GetRenderThread() const{ return pRenderThread; }
	
	/** OS specific. */
#if defined  OS_ANDROID || defined OS_WEBWASM
	inline void *GetHostWindow() const{ return 0; }
	inline void *GetWindow() const{ return 0; }
	void SetHostWindow(void *window);
	
#elif defined OS_BEOS
	inline BWindow *GetHostWindow() const{ return pHostWindow; }
	inline cGLWindow *GetWindow() const{ return pWindow; }
	void SetHostWindow(BWindow *window);
	BGLView *GetGLView() const;
	
#elif defined OS_W32
	inline HWND GetHostWindow() const{ return pHostWindow; }
	inline HWND GetWindow() const{ return pWindow; }
	inline HDC GetWindowDC() const{ return pWindowDC; }
	void SetHostWindow(HWND window);
	decPoint GetInnerSize() const;
	
#elif defined OS_MACOS
	inline NSWindow *GetHostWindow() const{ return pHostWindow; }
	inline NSWindow *GetWindow() const{ return pWindow; }
	void SetHostWindow(NSWindow *window);
	inline NSView *GetView() const{ return pView; }
	
#elif defined OS_UNIX_X11
	inline Window GetHostWindow() const{ return pHostWindow; }
	inline Window GetWindow() const{ return pWindow; }
	void SetHostWindow(Window window);
	
#ifdef OS_UNIX_WAYLAND
	inline wl_surface *GetWlSurface() const{ return pWlSurface; }
	void SetWlSurface(wl_surface *surface);
	
	inline xdg_surface *GetXdgSurface() const{ return pXdgSurface; }
	void SetXdgSurface(xdg_surface *surface);
	
	inline xdg_toplevel *GetXdgToplevel() const{ return pXdgToplevel; }
	void SetXdgToplevel(xdg_toplevel *toplevel);
	
	inline wl_egl_window *GetWlEglWindow() const{ return pWlEglWindow; }
	void SetWlEglWindow(wl_egl_window *eglWindow);
	
	inline wp_fractional_scale_v1 *GetWpFractionalScale() const{ return pWpFractionalScale; }
	void SetWpFractionalScale(wp_fractional_scale_v1 *scale);
	
	inline wp_viewport *GetWpViewport() const{ return pWpViewport; }
	void SetWpViewport(wp_viewport *viewport);
	
	inline wp_color_management_surface_v1 *GetWpColorSurface() const{ return pWpColorSurface; }
	void SetWpColorSurface(wp_color_management_surface_v1 *surface);
	
	static void OnXdgSurfaceConfigure(void *data, xdg_surface *xdgSurface, uint32_t serial);
	
	static void OnXdgToplevelConfigure(void *data, xdg_toplevel *toplevel,
		int32_t width, int32_t height, wl_array *states);
	
	static void OnXdgToplevelClose(void *data, xdg_toplevel *toplevel);
	
	static void OnXdgToplevelConfigureBounds(void *data, xdg_toplevel *toplevel,
		int32_t width, int32_t height);
	
	static void OnXdgToplevelWmCapabilities(void *data, xdg_toplevel *toplevel,
		wl_array *capabilities);
	
	static void OnWpFractionalScalePreferredScale(void *data,
		wp_fractional_scale_v1 *fractionalScale, uint32_t scale);
#endif
	
#ifdef BACKEND_OPENGL
	inline EGLSurface GetEGLSurface() const{ return pEGLSurface; }
	void SetEGLSurface(EGLSurface surface);
#endif
#endif
	
	
	/** X position of the window. */
	inline int GetX() const{ return pX; }
	
	/** Y position of the window. */
	inline int GetY() const{ return pY; }
	
	/** Set window position. */
	void SetPosition(int x, int y);

	/** Width of the window. */
	inline int GetWidth() const{ return pWidth; }
	
	/** Height of the window. */
	inline int GetHeight() const{ return pHeight; }
	
	/** Set window size. */
	void SetSize(int width, int height);
	
	/** Window title. */
	inline const decString &GetTitle() const{ return pTitle; }
	
	/** Set window title. */
	void SetTitle(const char *title);
	
	/** Full screen mode enabled. */
	inline bool GetFullScreen() const{ return pFullScreen; }
	
	/** Set if full screen mode is enabled. */
	void SetFullScreen(bool fullScreen);
	
	/** Painting enabled. */
	inline bool GetPaint() const{ return pPaint; }
	
	/** Set painting enabled. */
	void SetPaint(bool paint);
	
	/** Icon. */
	inline const deoglPixelBuffer::Ref &GetIcon() const{ return pIcon; }
	
	/** Set icon. */
	void SetIcon(deoglPixelBuffer *icon);
	
	/** Scale factor stored during CreateWindow. */
	inline int GetAfterCreateScaleFactor() const{ return pAfterCreateScaleFactor; }
	
	/** Use HDR output. */
	inline bool GetUseHdrOutput() const{ return pUseHdrOutput; }
	void SetUseHdrOutput(bool useHdrOutput);
	
	/** HDR maximum nits. */
	inline int GetHdrMaxNits() const{ return pHdrMaxNits; }
	void SetHdrMaxNits(int hdrMaxNits);
	
	/** HDR reference nits. */
	inline int GetHdrReferenceNits() const{ return pHdrReferenceNits; }
	void SetHdrReferenceNits(int hdrReferenceNits);
	
	/** Render target or nullptr. */
	inline const deoglRenderTargetArray::Ref &GetRenderTarget() const{ return pRenderTarget; }
	
	
	/** Render canvas view or \em NULL if not set. */
	inline const deoglRCanvasView::Ref &GetRCanvasView() const{ return pRCanvasView; }
	
	/** Set render canvas view or \em NULL if not set. */
	void SetRCanvasView(deoglRCanvasView *rcanvasView);
	
	/** Drop render canvas view if not \em NULL. */
	void DropRCanvasView();
	
	
	
	/** Create render window. */
	void CreateWindow();
	
	/** Swap buffers. Called from render thread. */
	void SwapBuffers();
	
	/** Render window if visible. Called from render thread. */
	void Render();
	
	/** Capture content by capture canvas if any matching one is pending. */
	void Capture();
	
	/** Center window on screen. */
	void CenterOnScreen();
	
	/** Windows repositioned (render thread event loop). */
	void OnReposition(int x, int y);
	
	/** Windows resized (render thread event loop). */
	void OnResize(int width, int height);
	
	
	
	/**
	 * Notify scripting module size changed due to OS event.
	 * \note Clears flag before returning if set.
	 */
	bool GetNotifySizeChanged();
	/*@}*/
	
	
	
private:
	void pUpdateVSync();
	
#if defined OS_UNIX_X11
	void pCreateNullCursor();
	
#if defined(OS_UNIX_WAYLAND) && defined(BACKEND_OPENGL)
	void pWlCleanupIconBuffer();
#endif
#endif
	
	void pDestroyWindow();
	
	void pRepositionWindow();
	void pResizeWindow();
	void pCreateRenderTarget();
	void pResizeRenderTarget();
	void pSetWindowTitle();
	void pUpdateFullScreen();
	void pSetIcon();
	int pGetDisplayScaleFactor();

#ifdef OS_MACOS
	void pMacOSCreateWindow();
	void pMacOSDestroyWindow();
	void pMacOSCenterOnScreen();
	void pMacOSResizeWindow();
	void pMacOSSetWindowTitle();
	void pMacOSUpdateFullscreen();
public:
	void pMacOSDelegateWindowClosed();
	void pMacOSDelegateWindowActivated();
	void pMacOSDelegateWindowDeactivated();
	void pMacOSDelegateWindowResized();
#endif
};

#endif
