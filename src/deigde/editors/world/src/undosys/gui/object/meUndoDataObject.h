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

#include <dragengine/common/collection/decTOrderedSet.h>
#include <dragengine/common/math/decMath.h>

class meWorld;


/**
 * Object data for undo actions.
 * 
 * Stores information about an object suitable for undo actions. The object
 * is stored as a pointer which is either owned or not. If the object is owned
 * it will be freed once this object is freed. If not owned the object is just
 * a pointer which will not be freed. By default the object is not owned.
 */
class meUndoDataObject : public deObject{
public:
	typedef deTObjectReference<meUndoDataObject> Ref;
	typedef decTObjectOrderedSet<meUndoDataObject> List;
	
	
private:
	meObject::Ref pObject;
	decDVector pOldPosition, pNewPosition;
	decVector pOldRotation, pNewRotation;
	decVector pOldSize, pNewSize;
	decVector pOldScaling, pNewScaling;
	meObject::Ref pAttachedTo;
	List pAttachedObjects;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create undo data object. */
	meUndoDataObject(meObject *object);
	
protected:
	/** Clean up data object. */
	~meUndoDataObject() override;
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Object pointer. */
	inline const meObject::Ref &GetObject() const{ return pObject; }
	
	/** Position before redo action. */
	inline const decDVector &GetOldPosition() const{ return pOldPosition; }
	
	/** Rotation before redo action. */
	inline const decVector &GetOldRotation() const{ return pOldRotation; }
	
	/** Size before redo action. */
	inline const decVector &GetOldSize() const{ return pOldSize; }
	
	/** Scaling before redo action. */
	inline const decVector &GetOldScaling() const{ return pOldScaling; }
	
	/** Position after redo action. */
	inline const decDVector &GetNewPosition() const{ return pNewPosition; }
	
	/** Set new position. */
	void SetNewPosition(const decDVector &position);
	
	/** Rotation after redo action. */
	inline const decVector &GetNewRotation() const{ return pNewRotation; }
	
	/** Set new rotation. */
	void SetNewRotation(const decVector &rotation);
	
	/** Size after redo action. */
	inline const decVector &GetNewSize() const{ return pNewSize; }
	
	/** Set new size. */
	void SetNewSize(const decVector &size);
	
	/** Scaling after redo action. */
	inline const decVector &GetNewScaling() const{ return pNewScaling; }
	
	/** Set new scaling. */
	void SetNewScaling(const decVector &scaling);
	
	/** Transform new geometry by matrix. */
	void TransformNew(const decDMatrix &matrix);
	
	/** Old object matrix. */
	decDMatrix GetOldMatrix() const;
	
	/** Inverse old object matrix. */
	decDMatrix GetOldMatrixInverse() const;
	
	/** New object matrix. */
	decDMatrix GetNewMatrix() const;
	
	/** Inverse new object matrix. */
	decDMatrix GetNewMatrixInverse() const;
	
	/** Attached to before redo action. */
	inline const meObject::Ref &GetAttachedTo() const{ return pAttachedTo; }
	
	/** Set attached to before redo action. */
	void SetAttachedTo(meObject *object);
	
	/** Attached objects. */
	inline List &GetAttachedObjects(){ return pAttachedObjects; }
	inline const List &GetAttachedObjects() const{ return pAttachedObjects; }
	
	
	/**
	 * Add objects and all attachments. This takes care of adding only attached objects to
	 * undo objects if they are not also part of the undo object list.
	 */
	static void AddObjectsWithAttachments(const meObject::List &objects, meUndoDataObject::List &list);
	
	/** Restore objects to their original position, rotation and size. */
	static void RestoreOldGeometry(const meUndoDataObject::List &list, meWorld &world);
	
	/** Apply new object position, rotation and size and update attachments. */
	static void ApplyNewGeometry(const meUndoDataObject::List &list, meWorld &world);
	/*@}*/
};

#endif
