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
 * \brief Render window peer.
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
	/** @name Constructors and Destructors */
	/*@{*/
	/** \brief Create peer. */
	deoglRenderWindow( deGraphicOpenGl &ogl, deRenderWindow &renderWindow );
	
	/** \brief Clean up peer. */
	virtual ~deoglRenderWindow();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief OpenGL module. */
	inline deGraphicOpenGl &GetOGL() const{ return pOgl; }
	
	/** \brief Render window engine resource. */
	inline deRenderWindow &GetRenderWindow() const{ return pRenderWindow; }
	
	/** \brief Canvas view. */
	inline deoglCanvasView *GetCanvasView() const{ return pCanvasView; }
	
	/** \brief Render render window. */
	inline deoglRRenderWindow *GetRRenderWindow() const{ return pRRenderWindow; }
	
	
	
	/** \brief Update render thread counterpart if required. */
	void SyncToRender();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Host window changed. */
	virtual void HostWindowChanged();
	
	/** \brief Size of render window changed. */
	virtual void SizeChanged();
	
	/** \brief Title of render window changed. */
	virtual void TitleChanged();
	
	/** \brief Full screen changed. */
	virtual void FullScreenChanged();
	
	/** \brief Notification of script on resize changed. */
	virtual void ScriptNotifyChanged();
	
	/** \brief Painting enabled changed. */
	virtual void PaintEnabledChanged();
	
	/** \brief Icon changed. */
	virtual void IconChanged();
	/*@}*/
	
private:
	void pCleanUp();
};

#endif
