/* 
 * Drag[en]gine IGDE
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

#ifndef _IGDENATIVEFOXRENDERVIEW_H_
#define _IGDENATIVEFOXRENDERVIEW_H_

#include "foxtoolkit.h"


class igdeViewRenderWindow;


/**
 * \brief FOX toolkit Native Render View.
 * 
 * Shows a Drag[en]gine render window. This widget is a container for an FXFrame representing
 * the window. The main problem is that the graphic module creates a window which can be placed
 * in a parent window. In FOX we exchange the FXFrame created window with the graphic module
 * created window. To avoid FOX layout containers obtaining problems the native widget is the
 * container inside which the FXFrame is located we can mess around with.
 */

class igdeNativeFoxRenderView : public FXFrame{
	FXDECLARE( igdeNativeFoxRenderView )
	
public:
	enum eFoxIDs{
		ID_SELF = FXFrame::ID_LAST,
		ID_TIMEOUT_RETRY_MAP,
		ID_LAST
	};
	
protected:
	igdeNativeFoxRenderView();
	
private:
	igdeViewRenderWindow *pOwner;
	bool pCanAttachRenderWindow;
	bool pRenderWindowAttached;
	bool pCanRender;
	bool pErrorRenderWindow;
	
#ifdef OS_W32
	LONG_PTR pOrgWindowProc;
#endif
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create widget. */
	igdeNativeFoxRenderView( igdeViewRenderWindow &owner, FXComposite *parent, int layoutFlags );
	
	/** \brief Clean up widget. */
	virtual ~igdeNativeFoxRenderView();
	
	/** \brief Create native widget. */
	static igdeNativeFoxRenderView* CreateNativeWidget( igdeViewRenderWindow &owner );
	
	/** \brief Post create native widget. */
	virtual void PostCreateNativeWidget();
	
	/** \brief Destroy native widget. */
	virtual void DestroyNativeWidget();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	virtual void destroy();
	FXbool canFocus() const;
	
	inline bool GetCanRender() const{ return pCanRender; }
	inline bool GetRenderWindowAttached() const{ return pRenderWindowAttached; }
	
	virtual void DropNativeWindow();
	virtual bool IsReallyVisible() const;
	virtual bool IsShown() const;
	virtual decPoint GetSize() const;
	virtual void OnFrameUpdate();
	
	
	/**
	 * \brief Attach render window if present.
	 * 
	 * Uses owner RenderWindow from stored reference if present. Destroys the FOX created
	 * window and attaches instead the render window window.
	 */
	virtual void AttachRenderWindow();
	
	/**
	 * \brief Detach render window if present.
	 * 
	 * Uses owner RenderWindow from stored reference if present. Detaches render window window
	 * and creates a new FOX window.
	 */
	virtual void DetachRenderWindow();
	
	void DrawEngineUnavailable( FXDCWindow &dc );
	void DrawErrorRenderWindow( FXDCWindow &dc );
	void DrawErrorRendering( FXDCWindow &dc );
	
	virtual void GrabInput();
	virtual void ReleaseInput();
	/*@}*/
	
	
	
	/** \name Events */
	/*@{*/
	long onResize( FXObject *sender, FXSelector selector, void *data );
	long onMap( FXObject *sender, FXSelector selector, void *data );
	long onUnmap( FXObject *sender, FXSelector selector, void *data );
	long onPaint( FXObject *sender, FXSelector selector, void *data );
	
	long onKeyPress( FXObject *sender, FXSelector selector, void *data );
	long onKeyRelease( FXObject *sender, FXSelector selector, void *data );
	long onLeftMouseDown( FXObject *sender, FXSelector selector, void *data );
	long onLeftMouseUp( FXObject *sender, FXSelector selector, void *data );
	long onRightMouseDown( FXObject *sender, FXSelector selector, void *data );
	long onRightMouseUp( FXObject *sender, FXSelector selector, void *data );
	long onMiddleMouseDown( FXObject *sender, FXSelector selector, void *data );
	long onMiddleMouseUp( FXObject *sender, FXSelector selector, void *data );
	long onMouseMove( FXObject *sender, FXSelector selector, void *data );
	long onMouseWheel( FXObject *sender, FXSelector selector, void *data );
	long onDoubleClicked( FXObject *sender, FXSelector selector, void *data );
	
	long onTimeoutRetryMap( FXObject *sender, FXSelector selector, void *data );
	/*@}*/
	
	
	
private:
	void pProcessMap();
	void pSyncSizes();
};

typedef igdeNativeFoxRenderView igdeNativeRenderView;

#endif
