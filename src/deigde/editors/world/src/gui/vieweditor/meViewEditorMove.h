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

#ifndef _MEVIEWEDITORMOVE_H_
#define _MEVIEWEDITORMOVE_H_

#include "meViewEditorNavigation.h"

#include <deigde/undo/igdeUndoReference.h>

#include <dragengine/resources/collider/deColliderReference.h>

class meCLSnapPoint;



/**
 * \brief View editor for moving scene elements.
 */
class meViewEditorMove : public meViewEditorNavigation{
private:
	igdeUndoReference pUndoMove;
	meCLSnapPoint *pCLSnapPoint;
	deColliderReference pCLCollider;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create view editor. */
	meViewEditorMove( meView3D &view );
	
	/** \brief Clean up view editor. */
	virtual ~meViewEditorMove();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/*@}*/
	
	/** \name Events */
	/*@{*/
	/** \brief A key on the keyboard has been pressed. Return true if handled. */
	virtual bool OnKeyPress( deInputEvent::eKeyCodes key, bool shift, bool control );
	
	/** \brief The left mouse button has been pressed. Return true if handled. */
	virtual void OnLeftMouseButtonPress( int x, int y, bool shift, bool control );
	
	/** \brief The left mouse button has been released. Return true if handled. */
	virtual void OnLeftMouseButtonRelease( int x, int y, bool shift, bool control );
	
	/** \brief The mouse has been moved. Return true if handled. */
	virtual void OnMouseMove( int x, int y, bool shift, bool control );
	/*@}*/
};

#endif
