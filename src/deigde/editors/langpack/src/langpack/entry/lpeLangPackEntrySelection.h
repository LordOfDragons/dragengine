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

#ifndef _LPELANGPACKENTRYSELECTION_H_
#define _LPELANGPACKENTRYSELECTION_H_

#include "lpeLangPackEntryList.h"



/**
 * \brief Language pack entry selection.
 */
class lpeLangPackEntrySelection{
private:
	lpeLangPackEntryList pSelection;
	lpeLangPackEntry::Ref pActive;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates a new list. */
	lpeLangPackEntrySelection();
	/** \brief Cleans up the list. */
	~lpeLangPackEntrySelection();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Retrieves the list of selected entries. */
	inline const lpeLangPackEntryList &GetSelected() const{ return pSelection; }
	/** \brief Adds a entries to the selection if not included already. */
	void Add(lpeLangPackEntry *entry);
	/** \brief Removes a entries from the selection if included. */
	void Remove(lpeLangPackEntry *entry);
	/** \brief Removes all entries from the selection. */
	void RemoveAll();
	
	/** \brief Retrieves the active entries or NULL if there is none. */
	inline lpeLangPackEntry *GetActive() const{ return pActive; }
	/** \brief Determines if there is an active entries. */
	bool HasActive() const;
	/** \brief Sets the active entries or NULL if there is none. */
	void SetActive(lpeLangPackEntry *entry);
	/** \brief Activate the first entries in the selection. */
	void ActivateNext();
	
	/** \brief Resets the selection removing all entries and setting the active one to NULL. */
	void Reset();
};

#endif
