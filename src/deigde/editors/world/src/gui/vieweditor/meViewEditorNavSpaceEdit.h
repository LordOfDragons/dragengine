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

#ifndef _MEVIEWEDITORNAVSPACEEDIT_H_
#define _MEVIEWEDITORNAVSPACEEDIT_H_

#include "meViewEditorNavigation.h"

#include <dragengine/resources/collider/deCollider::Ref.h>

class meCLSelect;



/**
 * \brief View editor navigation space edit.
 */
class meViewEditorNavSpaceEdit : public meViewEditorNavigation{
private:
	deCollider::Ref pColVol;
	meCLSelect *pCLSelect;
	float pSelectDistance;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create view editor. */
	meViewEditorNavSpaceEdit( meView3D &view );
	
	/** \brief Clean up view editor. */
	virtual ~meViewEditorNavSpaceEdit();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/*@}*/
	
	
	
	/** \name Events */
	/*@{*/
	/** \brief View size changed. */
	virtual void OnResize();
	
	/** \brief Left mouse button has been pressed. */
	virtual void OnLeftMouseButtonPress( int x, int y, bool shift, bool control );
	
	/** \brief Left mouse button has been released. */
	virtual void OnLeftMouseButtonRelease( int x, int y, bool shift, bool control );
	
	/** \brief Mouse has been moved. */
	virtual void OnMouseMove( int x, int y, bool shift, bool control );
	
	/** \brief The mouse wheel has been used. Steps contains the number of steps up (positive) or down (negative). Return true if handled. */
	virtual void OnMouseWheel( int steps, bool shift, bool control );
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif
