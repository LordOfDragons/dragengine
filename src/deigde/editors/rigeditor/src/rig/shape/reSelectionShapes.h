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
#ifndef _RESELECTIONSHAPES_H_
#define _RESELECTIONSHAPES_H_

#include "reRigShape.h"

// predefinitions
class reRig;
class deColliderVolume;



/**
 * @brief Rig Shape Selection.
 * Stores a list of selected rig shapes as well as the active rig shape.
 */
class reSelectionShapes{
private:
	reRig *pRig;
	
	reRigShape::List pShapes;
	reRigShape::Ref pActiveShape;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new rig shape selection. */
	reSelectionShapes(reRig *rig);
	/** Cleans up the rig shape selection. */
	~reSelectionShapes();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the parent rig. */
	inline reRig *GetParentRig() const{ return pRig; }
	
	/** List of selected shapes. */
	inline const reRigShape::List &GetShapes() const{ return pShapes; }
	
	/** Retrieves the index of the shape with the given collider or -1 if not found. */
	int IndexOfShapeWith(deColliderVolume *collider) const;
	/** Adds a shape if not existing already. */
	void AddShape(reRigShape *shape);
	/** Removes a shape if existing. */
	void RemoveShape(reRigShape *shape);
	/** Removes all shapes. */
	void RemoveAllShapes();
	
	/** Retrieves the active shape or nullptr. */
	inline const reRigShape::Ref &GetActiveShape() const{ return pActiveShape; }
	/** Determines if an active shape exists. */
	bool HasActiveShape() const;
	/** Sets the acitve shape or nullptr. */
	void SetActiveShape(reRigShape *shape);
	
	/** Removes all shapes and sets the active shape to nullptr. */
	void Reset();
	
	/** Adds all selected and visible shapes to the given list. */
	void AddVisibleShapesTo(reRigShape::List &list) const;
	/*@}*/
};

// end of include only once
#endif
