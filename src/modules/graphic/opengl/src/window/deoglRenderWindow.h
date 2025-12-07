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

#ifndef _DEOGLRENDERWINDOW_H_
#define _DEOGLRENDERWINDOW_H_

#include <dragengine/systems/modules/graphic/deBaseGraphicRenderWindow.h>

class deoglCanvasView;
class deoglRRenderWindow;

class deRenderWindow;
class deGraphicOpenGl;



/**
 * Render window peer.
 */
class deoglRenderWindow : public deBaseGraphicRenderWindow{
private:
	deGraphicOpenGl &pOgl;
	deRenderWindow &pRenderWindow;
	
	deoglCanvasView *pCanvasView;
	bool pDirtyParams;
	bool pDirtyPosition;
	bool pDirtySize;
	bool pDirtyWindowTitle;
	bool pDirtyFullScreen;
	bool pDirtyIcon;
	
	deoglRRenderWindow::Ref pRRenderWindow;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create peer. */
	deoglRenderWindow(deGraphicOpenGl &ogl, deRenderWindow &renderWindow);
	
	/** Clean up peer. */
	~deoglRenderWindow() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** OpenGL module. */
	inline deGraphicOpenGl &GetOGL() const{ return pOgl; }
	
	/** Render window engine resource. */
	inline deRenderWindow &GetRenderWindow() const{ return pRenderWindow; }
	
	/** Canvas view. */
	inline deoglCanvasView *GetCanvasView() const{ return pCanvasView; }
	
	/** Render render window. */
	inline const deoglRRenderWindow::Ref &GetRRenderWindow() const{ return pRRenderWindow; }
	
	
	
	/** Update render thread counterpart if required. */
	void SyncToRender();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** Host window changed. */
	void HostWindowChanged() override;
	
	/** Position of render window changed. */
	void PositionChanged() override;

	/** Size of render window changed. */
	void SizeChanged() override;
	
	/** Title of render window changed. */
	void TitleChanged() override;
	
	/** Full screen changed. */
	void FullScreenChanged() override;
	
	/** Notification of script on resize changed. */
	void ScriptNotifyChanged() override;
	
	/** Painting enabled changed. */
	void PaintEnabledChanged() override;
	
	/** Icon changed. */
	void IconChanged() override;
	/*@}*/
	
private:
	void pCleanUp();
};

#endif
