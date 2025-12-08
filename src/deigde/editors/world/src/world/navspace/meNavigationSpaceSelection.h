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

#ifndef _MENAVIGATIONSPACESELECTION_H_
#define _MENAVIGATIONSPACESELECTION_H_

#include "meNavigationSpaceList.h"



/**
 * @brief Navigation Space Selection.
 */
class meNavigationSpaceSelection{
private:
	meNavigationSpaceList pSelection;
	meNavigationSpace::Ref pActive;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new list. */
	meNavigationSpaceSelection();
	/** Cleans up the list. */
	~meNavigationSpaceSelection();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the list of selected navigation spacess. */
	inline const meNavigationSpaceList &GetSelected() const{ return pSelection; }
	/** Adds a navigation spaces to the selection if not included already. */
	void Add(meNavigationSpace::Ref navspaces);
	/** Removes a navigation spaces from the selection if included. */
	void Remove(meNavigationSpace::Ref navspaces);
	/** Removes all navigation spacess from the selection. */
	void RemoveAll();
	
	/** Retrieves the active navigation spaces or NULL if there is none. */
	inline meNavigationSpace *GetActive() const{ return pActive; }
	/** Determines if there is an active navigation spaces. */
	bool HasActive() const;
	/** Sets the active navigation spaces or NULL if there is none. */
	void SetActive(meNavigationSpace::Ref navspaces);
	/** Activate the first navigation spaces in the selection. */
	void ActivateNext();
	
	/** Resets the selection removing all navigation spacess and setting the active one to NULL. */
	void Reset();
};

#endif
