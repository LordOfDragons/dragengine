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

#include <dragengine/resources/collider/deCollider.h>
#include <dragengine/resources/canvas/deCanvasPaint.h>
#include <dragengine/resources/canvas/deCanvasText.h>

class meCLSelect;
class meCLCollect;
class deFontSize;


/**
 * View editor for selecting scene elements.
 */
class meViewEditorSelect : public meViewEditorNavigation{
private:
	meCLSelect *pCLSelect, *pCLBubbleInfo;
	deCollider::Ref pColVol;
	
	deCanvasPaint::Ref pCanvasSelect;
	
	meInfoBubble::Ref pInfoBubble;
	deCanvasText::Ref pInfoBubbleText;
	deFontSize *pFontSize;
	
	decPoint pLastMousePosition;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create view editor. */
	meViewEditorSelect(meView3D &view);
	
	/** Clean up view editor. */
	~meViewEditorSelect() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Update rectangular selection. */
	void UpdateRectSelection();
	/*@}*/
	
	
	
	/** \name Events */
	/*@{*/
	/** \brief A key on the keyboard has been pressed. Return true if handled. */
	bool OnKeyPress(deInputEvent::eKeyCodes key, bool shift, bool control) override;
	
	/** \brief A key on the keyboard has been released. Return true if handled. */
	bool OnKeyRelease(deInputEvent::eKeyCodes key, bool shift, bool control) override;
	
	/** View size changed. */
	void OnResize() override;
	
	/** The left mouse button has been pressed. Return true if handled. */
	void OnLeftMouseButtonPress(int x, int y, bool shift, bool control) override;
	
	/** The left mouse button has been released. Return true if handled. */
	void OnLeftMouseButtonRelease(int x, int y, bool shift, bool control) override;
	
	/** The right mouse button has been pressed. Return true if handled. */
	void OnRightMouseButtonPress(int x, int y, bool shift, bool control) override;
	
	/** The right mouse button has been released. Return true if handled. */
	void OnRightMouseButtonRelease(int x, int y, bool shift, bool control) override;
	
	/** The mouse has been moved. Return true if handled. */
	void OnMouseMove(int x, int y, bool shift, bool control) override;
	
	/** The mouse wheel has been used. Steps contains the number of steps up (positive) or down (negative). Return true if handled. */
	void OnMouseWheel(int steps, bool shift, bool control) override;
	
	/** The mouse left view. */
	void OnMousLeave() override;
	/*@}*/
	
private:
	void pCleanUp();
	void pUpdateInfoBubble(int x, int y, bool singleElement);
};

#endif
