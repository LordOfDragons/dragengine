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
#ifndef _RERIGSHAPELIST_H_
#define _RERIGSHAPELIST_H_

// predefinitions
class reRigShape;



/**
 * @brief Rig Shape List.
 * List of shapes.
 */
class reRigShapeList{
private:
	reRigShape **pShapes;
	int pShapeCount;
	int pShapeSize;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new rig shape list. */
	reRigShapeList();
	/** Cleans up the rig shape list. */
	~reRigShapeList();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the number of shapes. */
	inline int GetShapeCount() const{return pShapeCount;}
	/** Retrieves the shape at the given index. */
	reRigShape *GetShapeAt(int index) const;
	/** Retrieves the index of the shape or -1 if not found. */
	int IndexOfShape(reRigShape *shape) const;
	/** Determines if the shape exists. */
	bool HasShape(reRigShape *shape) const;
	/** Adds a new shape. */
	void AddShape(reRigShape *shape);
	/** Removes the given shape. */
	void RemoveShape(reRigShape *shape);
	/** Removes all shapes. */
	void RemoveAllShapes();
	/*@}*/
};

// end of include only once
#endif
