/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
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
	bool pDirtySize;
	bool pDirtyWindowTitle;
	bool pDirtyFullScreen;
	bool pDirtyIcon;
	
	deoglRRenderWindow *pRRenderWindow;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create peer. */
	deoglRenderWindow( deGraphicOpenGl &ogl, deRenderWindow &renderWindow );
	
	/** Clean up peer. */
	virtual ~deoglRenderWindow();
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
	inline deoglRRenderWindow *GetRRenderWindow() const{ return pRRenderWindow; }
	
	
	
	/** Update render thread counterpart if required. */
	void SyncToRender();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** Host window changed. */
	virtual void HostWindowChanged();
	
	/** Size of render window changed. */
	virtual void SizeChanged();
	
	/** Title of render window changed. */
	virtual void TitleChanged();
	
	/** Full screen changed. */
	virtual void FullScreenChanged();
	
	/** Notification of script on resize changed. */
	virtual void ScriptNotifyChanged();
	
	/** Painting enabled changed. */
	virtual void PaintEnabledChanged();
	
	/** Icon changed. */
	virtual void IconChanged();
	/*@}*/
	
private:
	void pCleanUp();
};

#endif
