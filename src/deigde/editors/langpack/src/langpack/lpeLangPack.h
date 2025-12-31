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

#ifndef _LPELANGPACK_H_
#define _LPELANGPACK_H_

#include "lpeLangPackListener.h"
#include "entry/lpeLangPackEntrySelection.h"

#include <deigde/editableentity/igdeEditableEntity.h>
#include <dragengine/common/collection/decTOrderedSet.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>

class lpeLangPackListener;



/**
 * Editable language pack.
 */
class lpeLangPack : public igdeEditableEntity{
public:
	typedef deTObjectReference<lpeLangPack> Ref;
	
	
private:
	decString pIdentifier;
	decUnicodeString pName;
	decUnicodeString pDescription;
	decUnicodeString pMissingText;
	
	lpeLangPackEntry::List pEntries;
	lpeLangPackEntrySelection pEntrySelection;
	
	decTObjectOrderedSet<lpeLangPackListener> pListeners;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new skin. */
	lpeLangPack(igdeEnvironment *environment);
	
	/** Cleans up the skin. */
protected:
	~lpeLangPack() override;
public:
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Identifier. */
	inline const decString &GetIdentifier() const{ return pIdentifier; }
	
	/** Sets identifier. */
	void SetIdentifier(const char *identifier);
	
	/** Retrieves the name in the native language. */
	inline const decUnicodeString &GetName() const{ return pName; }
	/** Sets the name in the native language. */
	void SetName(const decUnicodeString &name);
	/** Retrieves the description in the native language. */
	inline const decUnicodeString &GetDescription() const{ return pDescription; }
	/** Sets the description in the native language. */
	void SetDescription(const decUnicodeString &description);
	/** Retrieves the missing text in the native language. */
	inline const decUnicodeString &GetMissingText() const{ return pMissingText; }
	/** Sets the description in the native language. */
	void SetMissingText(const decUnicodeString &missingText);
	/*@}*/
	
	/** \name Entries */
	/*@{*/
	/** Retrieves the entry list read-only. */
	inline const lpeLangPackEntry::List &GetEntries() const{ return pEntries; }
	/** Retrieves the entry selection. */
	inline lpeLangPackEntrySelection &GetEntrySelection(){ return pEntrySelection; }
	inline const lpeLangPackEntrySelection &GetEntrySelection() const{ return pEntrySelection; }
	/** Adds a new entry. */
	void AddEntry(lpeLangPackEntry *entry);
	/** Removes a entry. */
	void RemoveEntry(lpeLangPackEntry *entry);
	/** Removes all entries. */
	void RemoveAllEntries();
	/*@}*/
	
	/** \name Notifiers */
	/*@{*/
	/** Adds a listener. */
	void AddListener(lpeLangPackListener *listener);
	/** Removes a listener. */
	void RemoveListener(lpeLangPackListener *listener);
	
	/** Notifies all listeners that the changed or saved state changed. */
	void NotifyStateChanged() override;
	/** Notifies all listeners that the undo system changed. */
	void NotifyUndoChanged() override;
	
	/** Notifies all that a language pack parameter changed. */
	void NotifyLangPackChanged();
	
	/** Notifies all that entries have been added or removed. */
	void NotifyEntryStructureChanged();
	/** Notifies all that an entry changed. */
	void NotifyEntryChanged(lpeLangPackEntry *entry);
	/** Notifies all that an entry name changed. */
	void NotifyEntryNameChanged(lpeLangPackEntry *entry);
	/** Notifies all that one or more entries changed selection. */
	void NotifyEntrySelectionChanged();
	/** Active entry changed. */
	void NotifyActiveEntryChanged();
	/*@}*/
};

#endif
