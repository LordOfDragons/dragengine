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

#ifndef _MEVIEWEDITORADDNEW_H_
#define _MEVIEWEDITORADDNEW_H_

#include "meViewEditorNavigation.h"
#include "../../filter/meFilterObjectsByClass.h"

#include <deigde/undo/igdeUndo.h>

#include <dragengine/resources/collider/deCollider.h>

class meCLAddDecal;
class meCLClosestElement;
class meCLSnapPoint;



/**
 * \brief View editor add new element.
 */
class meViewEditorAddNew : public meViewEditorNavigation{
public:
	typedef deTObjectReference<meViewEditorAddNew> Ref;
	
private:
	meCLClosestElement *pClosestElement;
	meCLSnapPoint *pCLSnapPoint;
	meFilterObjectsByClass::Ref pFilterObjectsByClass;
	meCLAddDecal *pAddDecal;
	igdeUndo::Ref pUndoAddObject;
	igdeUndo::Ref pUndoAddNavSpace;
	deCollider::Ref pCLCollider;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create view editor. */
	meViewEditorAddNew(meView3D &view);
	
protected:
	/** \brief Clean up view editor. */
	~meViewEditorAddNew() override;
	
public:
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/*@}*/
	
	/** \name Events */
	/*@{*/
	/** \brief A key on the keyboard has been pressed. Return true if handled. */
	bool OnKeyPress(deInputEvent::eKeyCodes key, bool shift, bool control) override;
	
	/** \brief The left mouse button has been pressed. Return true if handled. */
	void OnLeftMouseButtonPress(int x, int y, bool shift, bool control) override;
	
	/** \brief The left mouse button has been released. Return true if handled. */
	void OnLeftMouseButtonRelease(int x, int y, bool shift, bool control) override;
	
	/** \brief The mouse has been moved. Return true if handled. */
	void OnMouseMove(int x, int y, bool shift, bool control) override;
	/*@}*/
	
	
	
private:
	struct sCastIntoWorldParams{
		decDMatrix originalMatrix;
		decDVector position; //!< out
		decVector rotation; //!< out
		decDVector rayDirection;
		decDVector rayPosition;
		bool shift = false;
		bool control = false;
		bool useSnapPoint = false;
	};
	
	void pUpdateUndo(bool shift, bool control);
	void pCreateClosestElementVisitor();
	void pCastIntoWorld(sCastIntoWorldParams &params);
};

#endif
