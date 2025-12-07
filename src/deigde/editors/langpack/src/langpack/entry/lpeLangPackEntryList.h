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

#ifndef _LPELANGPACKENTRYLIST_H_
#define _LPELANGPACKENTRYLIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class lpeLangPackEntry;



/**
 * \brief Language pack entry List.
 */
class lpeLangPackEntryList{
private:
	decObjectOrderedSet pEntries;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create list. */
	lpeLangPackEntryList();
	
	/** \brief Create copy of list. */
	lpeLangPackEntryList(const lpeLangPackEntryList &list);
	
	/** \brief Clean up entry list. */
	~lpeLangPackEntryList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Count of entries. */
	int GetCount() const;
	
	/** \brief Entry at index. */
	lpeLangPackEntry *GetAt(int index) const;
	
	/** \brief Named entry or NULL if absent. */
	lpeLangPackEntry *GetNamed(const char *name) const;
	
	/** \brief Index of entry or -1 if absent. */
	int IndexOf(lpeLangPackEntry *entry) const;
	
	/** \brief Index of named entry or -1 if absent. */
	int IndexOfNamed(const char *name) const;
	
	/** \brief Entry is present. */
	bool Has(lpeLangPackEntry *entry) const;
	
	/** \brief Named entry is present. */
	bool HasNamed(const char *name) const;
	
	/** \brief Add entry. */
	void Add(lpeLangPackEntry *entry);
	
	/** \brief Insert entry. */
	void InsertAt(lpeLangPackEntry *entry, int index);
	
	/** \brief Move entry. */
	void MoveTo(lpeLangPackEntry *entry, int index);
	
	/** \brief Remove entry. */
	void Remove(lpeLangPackEntry *entry);
	
	/** \brief Remove all entries. */
	void RemoveAll();
	
	
	
	/** \brief Set from another list. */
	lpeLangPackEntryList &operator=(const lpeLangPackEntryList &list);
	/*@}*/
};

#endif
