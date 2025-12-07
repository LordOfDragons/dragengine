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

// include only once
#ifndef _RECLHITLIST_H_
#define _RECLHITLIST_H_

// includes
#include "dragengine/common/math/decMath.h"

// predefinitions
class reCLHitListEntry;



/**
 * List of elements hit during a test.
 */
class reCLHitList{
private:
	reCLHitListEntry **pEntries;
	int pEntryCount;
	int pEntrySize;
	
public:
	// constructor, destructor
	reCLHitList();
	~reCLHitList();
	
	/** @name Entries */
	/*@{*/
	/** Retrieves the number of entries. */
	inline int GetEntryCount() const{ return pEntryCount; }
	/** Retrieves the entry at the given index. */
	reCLHitListEntry *GetEntryAt(int index) const;
	/** Retrieves the index of the entry of -1 if not found. */
	int IndexOfEntry(reCLHitListEntry *entry) const;
	/** Determines if the entry exists. */
	bool HasEntry(reCLHitListEntry *entry) const;
	/** Adds a entry. */
	void AddEntry(reCLHitListEntry *entry);
	/** Removes a entry. */
	void RemoveEntry(reCLHitListEntry *entry);
	/** Removes all entries. */
	void RemoveAllEntries();
	
	/** Sorts entries by distance. */
	void SortByDistance();
	/*@}*/
};

// end of include only once
#endif
