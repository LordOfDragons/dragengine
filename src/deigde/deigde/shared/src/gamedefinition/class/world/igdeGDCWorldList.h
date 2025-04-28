/*
 * MIT License
 *
 * Copyright (C) 2025, DragonDreams GmbH (info@dragondreams.ch)
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

#ifndef _IGDEGDCWORLDLIST_H_
#define _IGDEGDCWORLDLIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class igdeGDCWorld;


/**
 * \brief Game definition class world list.
 */
class DE_DLL_EXPORT igdeGDCWorldList{
private:
	decObjectOrderedSet pWorlds;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create list. */
	igdeGDCWorldList();
	
	/** \brief Create copy of a World list. */
	igdeGDCWorldList(const igdeGDCWorldList &list);
	
	/** \brief Clean up list. */
	~igdeGDCWorldList();
	/*@}*/
	
	
	/** \name Management */
	/*@{*/
	/** \brief Count of Worlds. */
	int GetCount() const;
	
	/** \brief World at index. */
	igdeGDCWorld *GetAt(int index) const;
	
	/** \brief Index of world or -1 if absent. */
	int IndexOf(igdeGDCWorld *world) const;
	
	/** \brief World is present. */
	bool Has(igdeGDCWorld *world) const;
	
	/** \brief Add world. */
	void Add(igdeGDCWorld *world);
	
	/** \brief Insert world at index. */
	void InsertAt(igdeGDCWorld *world, int index);
	
	/** \brief Move world to new position. */
	void MoveTo(igdeGDCWorld *world, int index);
	
	/** \brief Remove world. */
	void Remove(igdeGDCWorld *world);
	
	/** \brief Remove all worlds. */
	void RemoveAll();
	
	/** \brief Set list to deep copy of another list. */
	void SetToDeepCopyFrom(const igdeGDCWorldList &list);
	
	/** \brief Set list from another list. */
	igdeGDCWorldList &operator=(const igdeGDCWorldList &list);
	/*@}*/
};

#endif
