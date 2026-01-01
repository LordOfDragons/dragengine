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

#ifndef _MEUNDODATADECAL_H_
#define _MEUNDODATADECAL_H_

#include "../../../world/decal/meDecal.h"
#include "../../../world/object/meObject.h"

#include <dragengine/common/collection/decTOrderedSet.h>
#include <dragengine/common/math/decMath.h>



/**
 * \brief Decal data for undo actions.
 * Stores information about a decal suitable for undo actions.
 */
class meUndoDataDecal : public deObject{
public:
	typedef deTObjectReference<meUndoDataDecal> Ref;
	typedef decTObjectOrderedSet<meUndoDataDecal> List;
	
private:
	meDecal::Ref pDecal;
	meObject::Ref pParentObject;
	int pIndex;
	
	decDVector pOldPosition;
	decVector pOldOrientation;
	decVector pOldSize;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new undo data object. */
	meUndoDataDecal(meDecal *decal);
	
protected:
	/** \brief Clean up data object. */
	~meUndoDataDecal() override;
	/*@}*/
	
public:
	/** \name Management */
	/*@{*/
	/** Retrieves the decal pointer. */
	inline const meDecal::Ref &GetDecal() const{ return pDecal; }
	
	/** Retrieves the parent object or nullptr. */
	inline const meObject::Ref &GetParentObject() const{ return pParentObject; }
	
	/** Retrieves the index. */
	inline int GetIndex() const{ return pIndex; }
	
	/** Retrieves the position before the action too place. */
	inline const decDVector &GetOldPosition() const{ return pOldPosition; }
	/** Retrieves the orientation before the action too place. */
	inline const decVector &GetOldOrientation() const{ return pOldOrientation; }
	/** Retrieves the size before the action too place. */
	inline const decVector &GetOldSize() const{ return pOldSize; }
	/*@}*/
};

#endif
