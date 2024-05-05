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

#ifndef _MEVIEWEDITORSELECT_H_
#define _MEVIEWEDITORSELECT_H_

#include "meViewEditorNavigation.h"
#include "../meInfoBubble.h"

#include <dragengine/resources/collider/deColliderReference.h>
#include <dragengine/resources/canvas/deCanvasPaint.h>
#include <dragengine/resources/canvas/deCanvasText.h>

class meCLSelect;
class meCLClosestElement;



/**
 * View editor for selecting scene elements.
 */
class meViewEditorSelect : public meViewEditorNavigation{
private:
	meCLSelect *pCLSelect;
	meCLClosestElement *pCLClosest;
	deColliderReference pColVol;
	
	deCanvasPaint::Ref pCanvasSelect;
	
	meInfoBubble::Ref pInfoBubble;
	deCanvasText::Ref pInfoBubbleText;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create view editor. */
	meViewEditorSelect( meView3D &view );
	
	/** Clean up view editor. */
	virtual ~meViewEditorSelect();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Update rectangular selection. */
	void UpdateRectSelection();
	/*@}*/
	
	
	
	/** \name Events */
	/*@{*/
	/** View size changed. */
	virtual void OnResize();
	
	/** The left mouse button has been pressed. Return true if handled. */
	virtual void OnLeftMouseButtonPress( int x, int y, bool shift, bool control );
	
	/** The left mouse button has been released. Return true if handled. */
	virtual void OnLeftMouseButtonRelease( int x, int y, bool shift, bool control );
	
	/** The right mouse button has been pressed. Return true if handled. */
	virtual void OnRightMouseButtonPress( int x, int y, bool shift, bool control );
	
	/** The right mouse button has been released. Return true if handled. */
	virtual void OnRightMouseButtonRelease( int x, int y, bool shift, bool control );
	
	/** The mouse has been moved. Return true if handled. */
	virtual void OnMouseMove( int x, int y, bool shift, bool control );
	
	/** The mouse wheel has been used. Steps contains the number of steps up (positive) or down (negative). Return true if handled. */
	virtual void OnMouseWheel( int steps, bool shift, bool control );
	
	/** The mouse left view. */
	virtual void OnMousLeave();
	/*@}*/
	
private:
	void pCleanUp();
	void pUpdateInfoBubble( int x, int y );
};

#endif
