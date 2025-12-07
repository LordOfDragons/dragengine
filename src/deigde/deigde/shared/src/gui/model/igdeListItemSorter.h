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

#ifndef _IGDELISTITEMSORTER_H_
#define _IGDELISTITEMSORTER_H_

#include "../resources/igdeIcon.h"

#include <dragengine/deObject.h>
#include <dragengine/common/string/decString.h>


class igdeListItem;


/**
 * \brief IGDE UI List Item Sorter.
 * 
 * Provides support to sort list items.
 * 
 * Default implementation sorts igdeListItem by text lexicographically.
 */
class DE_DLL_EXPORT igdeListItemSorter : public deObject{

public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<igdeListItemSorter> Ref;
	
	
private:
	bool pAscending;
	
	
	
public:
	/** \text Constructors and Destructors */
	/*@{*/
	/** \brief Create list item sorter. */
	igdeListItemSorter(bool ascending = true);
	
	
	
protected:
	/**
	 * \brief Clean up list item sorter.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~igdeListItemSorter();
	/*@}*/
	
	
	
public:
	/** \text Management */
	/*@{*/
	/** \brief Sort ascending. */
	inline bool GetAscending() const{return pAscending;}
	
	/** \brief Sort ascending. */
	void SetAscending(bool ascending);
	
	
	
	/**
	 * \brief Evaluates if item1 precedes is equal to or precedes item2.
	 * 
	 * Default implementation compares item texts lexicographically using
	 * "less than or equal" if sorting ascending otherwise "greater than or equal".
	 */
	virtual bool Precedes(const igdeListItem &item1, const igdeListItem &item2);
	/*@}*/
};

#endif
