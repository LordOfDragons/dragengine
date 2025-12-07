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

#ifndef _MEOBJECTSHAPESELECTION_H_
#define _MEOBJECTSHAPESELECTION_H_

#include "meObjectShapeList.h"

class decIntList;



/**
 * @brief Object Shape Selection.
 */
class meObjectShapeSelection{
private:
	meObjectShapeList pSelection;
	meObjectShape *pActive;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new list. */
	meObjectShapeSelection();
	/** Cleans up the list. */
	~meObjectShapeSelection();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the list of selected object shapes. */
	inline const meObjectShapeList &GetSelected() const{ return pSelection; }
	/** Adds an object shape to the selection if not included already. */
	void Add(meObjectShape *objectShape);
	/** Removes an object shape from the selection if included. */
	void Remove(meObjectShape *objectShape);
	/** Removes all object shapes from the selection. */
	void RemoveAll();
	
	/** Retrieves the active object shape or NULL if there is none. */
	inline meObjectShape *GetActive() const{ return pActive; }
	/** Determines if there is an active object shape. */
	bool HasActive() const;
	/** Sets the active object shape or NULL if there is none. */
	void SetActive(meObjectShape *object);
	/** Activate the first object shape in the selection. */
	void ActivateNext();
	
	/** Resets the selection removing all object shapes and setting the active one to NULL. */
	void Reset();
	
	/** Backup the selection and active as indicies relative to a list of shapes. */
	void Backup(const meObjectShapeList &shapeList, decIntList &indicesSelected, int &indexActive);
	/** Restores the selection and active from indices relative to a list of shapes if possible. */
	void Restore(const meObjectShapeList &shapeList, const decIntList &indicesSelected, int indexActive);
};

#endif
