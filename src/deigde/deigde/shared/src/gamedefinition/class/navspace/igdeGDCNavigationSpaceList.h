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

#ifndef _IGDEGDCNAVIGATIONSPACELIST_H_
#define _IGDEGDCNAVIGATIONSPACELIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class igdeGDCNavigationSpace;



/**
 * \brief Game definition class navigation space list.
 */
class DE_DLL_EXPORT igdeGDCNavigationSpaceList{
private:
	decObjectOrderedSet pNavigationSpaces;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create list. */
	igdeGDCNavigationSpaceList();
	
	/** \brief Create copy of a game definition class navigation space list. */
	igdeGDCNavigationSpaceList(const igdeGDCNavigationSpaceList &list);
	
	/** \brief Clean up list. */
	~igdeGDCNavigationSpaceList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of navigation spaces. */
	int GetCount() const;
	
	/** \brief Navigation space by index. */
	igdeGDCNavigationSpace *GetAt(int index) const;
	
	/** \brief Index of an navigation space or -1 if not found. */
	int IndexOf(igdeGDCNavigationSpace::Ref navSpace) const;
	
	/** \brief Determines if an navigation space exists. */
	bool Has(igdeGDCNavigationSpace::Ref navSpace) const;
	
	/** \brief Adds an navigation space. */
	void Add(igdeGDCNavigationSpace::Ref navSpace);
	
	/** \brief Inserts a new navigation space. */
	void InsertAt(igdeGDCNavigationSpace::Ref navSpace, int index);
	
	/** \brief Moves an navigation space to a new position. */
	void MoveTo(igdeGDCNavigationSpace::Ref navSpace, int index);
	
	/** \brief Removes an navigation space. */
	void Remove(igdeGDCNavigationSpace::Ref navSpace);
	
	/** \brief Removes all navigation spaces. */
	void RemoveAll();
	
	/** \brief Set this list to a deep copy of another list. */
	void SetToDeepCopyFrom(const igdeGDCNavigationSpaceList &list);
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Set list from another list. */
	igdeGDCNavigationSpaceList &operator=(const igdeGDCNavigationSpaceList &list);
	/*@}*/
};

#endif
