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
	long onResize( FXObject*, FXSelector, void* );
	long onMap( FXObject*, FXSelector, void* );
	long onUnmap( FXObject*, FXSelector, void* );
	long onPaint( FXObject*, FXSelector, void* );
	
	long onKeyPress( FXObject*, FXSelector, void* );
	long onKeyRelease( FXObject*, FXSelector, void* );
	long onLeftMouseDown( FXObject*, FXSelector, void* );
	long onLeftMouseUp( FXObject*, FXSelector, void* );
	long onRightMouseDown( FXObject*, FXSelector, void* );
	long onRightMouseUp( FXObject*, FXSelector, void* );
	long onMiddleMouseDown( FXObject*, FXSelector, void* );
	long onMiddleMouseUp( FXObject*, FXSelector, void* );
	long onMouseMove( FXObject*, FXSelector, void* );
	long onMouseWheel( FXObject*, FXSelector, void* );
	long onDoubleClicked( FXObject*, FXSelector, void* );
	long onMouseEnter( FXObject*, FXSelector, void* );
	long onMouseLeave( FXObject*, FXSelector, void* );
	
	long onTimeoutRetryMap( FXObject*, FXSelector, void* );
	/*@}*/
	
	
	
private:
	void pProcessMap();
	void pSyncSizes();
};

typedef igdeNativeFoxRenderView igdeNativeRenderView;

#endif
