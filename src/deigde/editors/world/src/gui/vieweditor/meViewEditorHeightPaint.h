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

#ifndef _MEVIEWEDITORHEIGHTPAINT_H_
#define _MEVIEWEDITORHEIGHTPAINT_H_

#include "meViewEditorNavigation.h"

class meCLHeightPaint;



/**
 * @brief View Editor Height Paint.
 * View editor for height painting terrain.
 */
class meViewEditorHeightPaint : public meViewEditorNavigation{
private:
	float pPainterElapsedTime;
	bool pPainting;
	decPoint pPaintPosition;
	
	meCLHeightPaint *pHeightPainter;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new view editor. */
	meViewEditorHeightPaint( meView3D &view );
	/** Cleans up the view editor. */
	virtual ~meViewEditorHeightPaint();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Game like frame update. */
	virtual void OnFrameUpdate( float elapsed );
	/*@}*/
	
	/** \name Events */
	/*@{*/
	/** The left mouse button has been pressed. Return true if handled. */
	virtual void OnLeftMouseButtonPress( int x, int y, bool shift, bool control );
	/** The left mouse button has been released. Return true if handled. */
	virtual void OnLeftMouseButtonRelease( int x, int y, bool shift, bool control );
	/** The mouse has been moved. Return true if handled. */
	virtual void OnMouseMove( int x, int y, bool shift, bool control );
	/*@}*/
};

#endif
