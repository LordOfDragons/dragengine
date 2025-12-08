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

// include only once
#ifndef _MEUOBJECTDROPTOGROUND_H_
#define _MEUOBJECTDROPTOGROUND_H_

// includes
#include <deigde/undo/igdeUndo.h>

#include <dragengine/common/math/decMath.h>

// predefinitions
#include "../../../world/meWorld.h"
#include "../../../world/object/meObject.h"
class meObjectList;



/**
 * \brief Drop objects to ground undo action.
 *
 * Undo action dropping a list of objects individually onto the ground.
 */
class meUObjectDropToGround : public igdeUndo{
private:
	struct sObject{
		meObject::Ref object;
		decDVector oldpos;
		decVector oldrot;
	};
	
private:
	meWorld::Ref pWorld;
	
	sObject *pObjects;
	int pObjectCount;
	
	bool pDropOnObjects;
	bool pAlign;
	
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<meUObjectDropToGround> Ref;
	
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create object. */
	meUObjectDropToGround(meWorld *world, const meObjectList &objects);
	/** \brief Clean up object. */
	virtual ~meUObjectDropToGround();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Sets if the objects can be dropped on other objects. */
	void SetDropOnObjects(bool dropOnObjects);
	/** Sets if the objects are aligned with the ground or just dropped down. */
	void SetAlign(bool align);
	
	/** \brief Undo. */
	virtual void Undo();
	/** \brief Redo. */
	virtual void Redo();
	/*@}*/
	
private:
	void pCleanUp();
};

// end of include only once
#endif
