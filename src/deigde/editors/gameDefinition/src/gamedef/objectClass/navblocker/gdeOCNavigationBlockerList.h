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

#ifndef _GDEOCNAVIGATIONBLOCKERLIST_H_
#define _GDEOCNAVIGATIONBLOCKERLIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class gdeOCNavigationBlocker;



/**
 * \brief Object class navigation blocker list.
 */
class gdeOCNavigationBlockerList{
private:
	decObjectOrderedSet pNavigationBlockers;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create object class navigation blocker list. */
	gdeOCNavigationBlockerList();
	
	/** \brief Create copy of object class navigation blocker list. */
	gdeOCNavigationBlockerList(const gdeOCNavigationBlockerList &list);
	
	/** \brief Clean up object class navigation blocker list. */
	~gdeOCNavigationBlockerList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of object class navigation blockers. */
	int GetCount() const;
	
	/** \brief Object class navigation blocker at position. */
	gdeOCNavigationBlocker *GetAt(int index) const;
	
	/** \brief Index of named object class navigation blocker or -1 if absent. */
	int IndexOf(gdeOCNavigationBlocker *blocker) const;
	
	/** \brief Object class navigation blocker is present. */
	bool Has(gdeOCNavigationBlocker *blocker) const;
	
	/** \brief Add object class navigation blocker. */
	void Add(gdeOCNavigationBlocker *blocker);
	
	/** \brief Remove object class navigation blocker. */
	void Remove(gdeOCNavigationBlocker *blocker);
	
	/** \brief Remove all object class navigation blockers. */
	void RemoveAll();
	
	/** \brief Set from another object class navigation blocker list. */
	gdeOCNavigationBlockerList &operator=(const gdeOCNavigationBlockerList &list);
	/*@}*/
};

#endif
