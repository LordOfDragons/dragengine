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

#ifndef _MEUNDODATAOBJECT_H_
#define _MEUNDODATAOBJECT_H_

#include "../../../world/object/meObject.h"

#include <dragengine/common/math/decMath.h>


/**
 * \brief Object data for undo actions.
 * 
 * Stores information about an object suitable for undo actions. The object
 * is stored as a pointer which is either owned or not. If the object is owned
 * it will be freed once this object is freed. If not owned the object is just
 * a pointer which will not be freed. By default the object is not owned.
 */
class meUndoDataObject : public deObject{
public:
	/** Reference. */
	typedef deTObjectReference<meUndoDataObject> Ref;
	
	
private:
	meObject::Ref pObject;
	decDVector pOldPosition;
	decVector pOldOrientation;
	decVector pOldSize;
	meObject::Ref pAttachedTo;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create undo data object. */
	meUndoDataObject( meObject *object );
	
protected:
	/** \brief Clean up data object. */
	virtual ~meUndoDataObject();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Object pointer. */
	inline const meObject::Ref &GetObject() const{ return pObject; }
	
	/** \brief Position before redo action. */
	inline const decDVector &GetOldPosition() const{ return pOldPosition; }
	
	/** \brief Orientation before redo action. */
	inline const decVector &GetOldOrientation() const{ return pOldOrientation; }
	
	/** \brief Size before redo action. */
	inline const decVector &GetOldSize() const{ return pOldSize; }
	
	/** \brief Attached to before redo action. */
	inline const meObject::Ref &GetAttachedTo() const{ return pAttachedTo; }
	
	/** \brief Set attached to before redo action. */
	void SetAttachedTo( meObject *object );
	/*@}*/
};

#endif
