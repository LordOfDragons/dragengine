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

#ifndef _MEOBJECTSELECTION_H_
#define _MEOBJECTSELECTION_H_

#include "meObjectList.h"



/**
 * @brief Object Selection.
 */
class meObjectSelection{
private:
	meObjectList pSelection;
	meObject *pActive;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new list. */
	meObjectSelection();
	/** Cleans up the list. */
	~meObjectSelection();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the list of selected objects. */
	inline const meObjectList &GetSelected() const{return pSelection;}
	/** Adds a objects to the selection if not included already. */
	void Add(meObject *object);
	/** Removes a objects from the selection if included. */
	void Remove(meObject *object);
	/** Removes all objects from the selection. */
	void RemoveAll();
	
	/** Retrieves the active objects or NULL if there is none. */
	inline meObject *GetActive() const{return pActive;}
	/** Determines if there is an active objects. */
	bool HasActive() const;
	/** Sets the active objects or NULL if there is none. */
	void SetActive(meObject *object);
	/** Activate the first objects in the selection. */
	void ActivateNext();
	
	/** Resets the selection removing all objects and setting the active one to NULL. */
	void Reset();
};

#endif
