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

#ifndef _CESTRIPLIST_H_
#define _CESTRIPLIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class ceStrip;



/**
 * \brief Conversation action strip List.
 */
class ceStripList{
private:
	decObjectOrderedSet pList;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new string duration list. */
	ceStripList();
	/** Cleans up the string duration list. */
	~ceStripList();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the number of entries. */
	int GetCount() const;
	/** Retrieves the entry at the given duration. */
	ceStrip *GetAt(int index) const;
	/** Retrieves the index of the entry or -1 if not found. */
	int IndexOf(ceStrip::Ref entry) const;
	/** Determines if an entry exists. */
	bool Has(ceStrip::Ref entry) const;
	/** Adds an entry. */
	void Add(ceStrip::Ref entry);
	/** Inserts an entry. */
	void InsertAt(ceStrip::Ref entry, int index);
	/** Moves an entry to a new position. */
	void MoveTo(ceStrip::Ref entry, int index);
	/** Removes an entry. */
	void Remove(ceStrip::Ref entry);
	/** Removes all entries. */
	void RemoveAll();
	
	/** Adds copies of entries of another list to this list. */
	void AddCopyFrom(const ceStripList &list);
	/*@}*/
	
	/** \name Operators */
	/*@{*/
	/** Set this list to the content of another list. */
	ceStripList &operator=(const ceStripList &list);
	/*@}*/
};

#endif
