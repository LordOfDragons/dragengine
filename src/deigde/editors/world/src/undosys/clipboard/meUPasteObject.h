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

#ifndef _MEUPASTEOBJECT_H_
#define _MEUPASTEOBJECT_H_

#include "../../world/object/meObjectList.h"

#include <deigde/undo/igdeUndo.h>

#include <dragengine/common/math/decMath.h>
#include <dragengine/common/collection/decIntList.h>

class meClipboardDataObject;
class meWorld;



/**
 * \brief Undo action for objects pasted from the clipboard.
 * 
 * Creates a copy of each object in the clipboard and then inserts or removes this copy
 * from the current map.
 */
class meUPasteObject : public igdeUndo{
private:
	meWorld::Ref pWorld;
	meObjectList pObjects;
	decIntList pAttachedToIndexList;
	
	
	
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<meUPasteObject> Ref;
	
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create undo object. */
	meUPasteObject(meWorld *world, meClipboardDataObject *clip);
	
	/** \brief Clean up undo object. */
	virtual ~meUPasteObject();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Undo action. */
	virtual void Undo();
	
	/** \brief Redo action. */
	virtual void Redo();
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif
