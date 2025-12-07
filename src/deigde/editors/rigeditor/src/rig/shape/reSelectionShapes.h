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

// predefinitions
class reRig;
class reRigShape;
class reRigShapeList;
class deColliderVolume;



/**
 * @brief Rig Shape Selection.
 * Stores a list of selected rig shapes as well as the active rig shape.
 */
class reSelectionShapes{
private:
	reRig *pRig;
	
	reRigShape **pShapes;
	int pShapeCount, pShapeSize;
	reRigShape *pActiveShape;
	
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
	inline reRig *GetParentRig() const{return pRig;}
	
	/** Retrieves the number of selected shapes. */
	inline int GetShapeCount() const{return pShapeCount;}
	/** Retrieves the shape at the given index. */
	reRigShape *GetShapeAt(int index) const;
	/** Determines if the given shape exists. */
	bool HasShape(reRigShape *shape) const;
	/** Retrieves the index of the given shape or -1 if not found. */
	int IndexOfShape(reRigShape *shape) const;
	/** Retrieves the index of the shape with the given collider or -1 if not found. */
	int IndexOfShapeWith(deColliderVolume *collider) const;
	/** Adds a shape if not existing already. */
	void AddShape(reRigShape *shape);
	/** Removes a shape if existing. */
	void RemoveShape(reRigShape *shape);
	/** Removes all shapes. */
	void RemoveAllShapes();
	
	/** Retrieves the active shape or NULL. */
	inline reRigShape *GetActiveShape() const{return pActiveShape;}
	/** Determines if an active shape exists. */
	bool HasActiveShape() const;
	/** Sets the acitve shape or NULL. */
	void SetActiveShape(reRigShape *shape);
	
	/** Removes all shapes and sets the active shape to NULL. */
	void Reset();
	
	/** Adds all selected and visible shapes to the given list. */
	void AddVisibleShapesTo(reRigShapeList &list) const;
	/*@}*/
};

// end of include only once
#endif
