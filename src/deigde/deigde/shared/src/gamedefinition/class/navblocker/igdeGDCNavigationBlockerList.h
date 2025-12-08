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

#ifndef _IGDEGDCNAVIGATIONBLOCKERLIST_H_
#define _IGDEGDCNAVIGATIONBLOCKERLIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class igdeGDCNavigationBlocker;



/**
 * \brief Game definition class navigation blocker list.
 */
class DE_DLL_EXPORT igdeGDCNavigationBlockerList{
private:
	decObjectOrderedSet pNavigationBlockers;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create list. */
	igdeGDCNavigationBlockerList();
	
	/** \brief Create copy of a game definition class navigation blocker list. */
	igdeGDCNavigationBlockerList(const igdeGDCNavigationBlockerList &list);
	
	/** \brief Clean up list. */
	~igdeGDCNavigationBlockerList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of navigation blockers. */
	int GetCount() const;
	
	/** \brief Navigation blocker by index. */
	igdeGDCNavigationBlocker *GetAt(int index) const;
	
	/** \brief Index of an navigation blocker or -1 if not found. */
	int IndexOf(igdeGDCNavigationBlocker::Ref blocker) const;
	
	/** \brief Determines if an navigation blocker exists. */
	bool Has(igdeGDCNavigationBlocker::Ref blocker) const;
	
	/** \brief Adds an navigation blocker. */
	void Add(igdeGDCNavigationBlocker::Ref blocker);
	
	/** \brief Inserts a new navigation blocker. */
	void InsertAt(igdeGDCNavigationBlocker::Ref blocker, int index);
	
	/** \brief Moves an navigation blocker to a new position. */
	void MoveTo(igdeGDCNavigationBlocker::Ref blocker, int index);
	
	/** \brief Removes an navigation blocker. */
	void Remove(igdeGDCNavigationBlocker::Ref blocker);
	
	/** \brief Removes all navigation blockers. */
	void RemoveAll();
	
	/** \brief Set this list to a deep copy of another list. */
	void SetToDeepCopyFrom(const igdeGDCNavigationBlockerList &list);
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Set list from another list. */
	igdeGDCNavigationBlockerList &operator=(const igdeGDCNavigationBlockerList &list);
	/*@}*/
};

#endif
