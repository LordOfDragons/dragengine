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

#ifndef _MEVIEWEDITORNAVIGATION_H_
#define _MEVIEWEDITORNAVIGATION_H_

#include "meViewEditor.h"

#include <deigde/undo/igdeUndo.h>



/**
 * \brief View Editor Navigation.
 * 
 * Base class for view editors with navigation support. Navigation support is done whenever modifier buttons
 * are held while moving with the right mouse button pressed. For subclasses it is marked if navigation is
 * in progress. This way subclasses can do their own processing during mouse movement if no navigation is
 * in progress. Otherwise simply do nothing if the right mouse button is pressed during mouse move.
 */
class meViewEditorNavigation : public meViewEditor{
public:
	typedef deTObjectReference<meViewEditorNavigation> Ref;
	
private:
	float pOldDist;
	float pOldZoom;
	decQuaternion pOldOrientation;
	decVector pOldRotation;
	
	bool pNavigating;
	
	igdeUndo::Ref pUndoCameraMove;
	igdeUndo::Ref pUndoCameraRotate;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new view editor. */
	meViewEditorNavigation(meView3D &view);
	
protected:
	/** Cleans up the view editor. */
	~meViewEditorNavigation() override;
	
public:
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Determines if navigation is in progress. */
	inline bool GetNavigating() const{ return pNavigating; }
	/*@}*/
	
	/** \name Events */
	/*@{*/
	/** The right mouse button has been pressed. Return true if handled. */
	void OnRightMouseButtonPress(int x, int y, bool shift, bool control) override;
	/** The right mouse button has been released. Return true if handled. */
	void OnRightMouseButtonRelease(int x, int y, bool shift, bool control) override;
	/** The mouse has been moved. Return true if handled. */
	void OnMouseMove(int x, int y, bool shift, bool control) override;
	/** The mouse wheel has been used. Steps contains the number of steps up (positive) or down (negative). Return true if handled. */
	void OnMouseWheel(int steps, bool shift, bool control) override;
	/*@}*/
};

#endif
