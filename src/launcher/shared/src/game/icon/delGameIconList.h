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

#ifndef _DELGAMEICONLIST_H_
#define _DELGAMEICONLIST_H_

#include <dragengine/common/collection/decObjectList.h>

class delGameIcon;


/**
 * \brief Icon icon list.
 */
class DE_DLL_EXPORT delGameIconList{
private:
	decObjectList pIcons;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create list. */
	delGameIconList();
	
	/** \brief Clean up list. */
	~delGameIconList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Count of icons. */
	int GetCount() const;
	
	/** \brief Icon at index. */
	delGameIcon *GetAt(int index) const;
	
	/** \brief Icon with size or nullptr if absent. */
	delGameIcon *GetWithSize(int size) const;
	
	/** \brief Icon with larges size less than or equal or nullptr if absent. */
	delGameIcon *GetLargest(int size) const;
	
	/** \brief Icon with smallest size larger than or equal or nullptr if absent. */
	delGameIcon *GetSmallest(int size) const;
	
	/** \brief Icon with larges size or nullptr if absent. */
	delGameIcon *GetLargest() const;
	
	/** \brief Icon with smallest size larger than or equal or nullptr if absent. */
	delGameIcon *GetSmallest() const;
	
	/** \brief Icon is present. */
	bool Has(delGameIcon *icon) const;
	
	/** \brief Icon with size. */
	bool HasWithSize(int size) const;
	
	/** \brief Index of icon or -1 if absent. */
	int IndexOf(delGameIcon *icon) const;
	
	/** \brief Index of icon with size or -1 if absent. */
	int IndexOfWithSize(int size) const;
	
	/** \brief Add icon. */
	void Add(delGameIcon *icon);
	
	/** \brief Remove icon. */
	void Remove(delGameIcon *icon);
	
	/** \brief Remove all icons. */
	void RemoveAll();
	/*@}*/
};

#endif
