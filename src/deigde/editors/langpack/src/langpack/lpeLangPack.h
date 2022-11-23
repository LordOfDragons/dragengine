/* 
 * Drag[en]gine IGDE Language Pack Editor
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef _LPELANGPACK_H_
#define _LPELANGPACK_H_

#include "entry/lpeLangPackEntrySelection.h"

#include <deigde/editableentity/igdeEditableEntity.h>
#include <dragengine/common/collection/decObjectSet.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>

class lpeLangPackListener;



/**
 * \brief Editable language pack.
 */
class lpeLangPack : public igdeEditableEntity{
private:
	decString pIdentifier;
	decUnicodeString pName;
	decUnicodeString pDescription;
	decUnicodeString pMissingText;
	
	lpeLangPackEntryList pEntryList;
	lpeLangPackEntrySelection pEntrySelection;
	
	decObjectSet pListeners;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates a new skin. */
	lpeLangPack( igdeEnvironment *environment );
	/** \brief Cleans up the skin. */
	virtual ~lpeLangPack();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Identifier. */
	inline const decString &GetIdentifier() const{ return pIdentifier; }
	
	/** Sets identifier. */
	void SetIdentifier( const char *identifier );
	
	/** Retrieves the name in the native language. */
	inline const decUnicodeString &GetName() const{ return pName; }
	/** Sets the name in the native language. */
	void SetName( const decUnicodeString &name );
	/** Retrieves the description in the native language. */
	inline const decUnicodeString &GetDescription() const{ return pDescription; }
	/** Sets the description in the native language. */
	void SetDescription( const decUnicodeString &description );
	/** Retrieves the missing text in the native language. */
	inline const decUnicodeString &GetMissingText() const{ return pMissingText; }
	/** Sets the description in the native language. */
	void SetMissingText( const decUnicodeString &missingText );
	/*@}*/
	
	/** \name Entries */
	/*@{*/
	/** \brief Retrieves the entry list read-only. */
	inline const lpeLangPackEntryList &GetEntryList() const{ return pEntryList; }
	/** \brief Retrieves the entry selection. */
	inline lpeLangPackEntrySelection &GetEntrySelection(){ return pEntrySelection; }
	inline const lpeLangPackEntrySelection &GetEntrySelection() const{ return pEntrySelection; }
	/** \brief Adds a new entry. */
	void AddEntry( lpeLangPackEntry *entry );
	/** \brief Removes a entry. */
	void RemoveEntry( lpeLangPackEntry *entry );
	/** \brief Removes all entries. */
	void RemoveAllEntries();
	/*@}*/
	
	/** \name Notifiers */
	/*@{*/
	/** \brief Adds a listener. */
	void AddListener( lpeLangPackListener *listener );
	/** \brief Removes a listener. */
	void RemoveListener( lpeLangPackListener *listener );
	
	/** \brief Notifies all listeners that the changed or saved state changed. */
	virtual void NotifyStateChanged();
	/** \brief Notifies all listeners that the undo system changed. */
	virtual void NotifyUndoChanged();
	
	/** \brief Notifies all that a language pack parameter changed. */
	void NotifyLangPackChanged();
	
	/** \brief Notifies all that entries have been added or removed. */
	void NotifyEntryStructureChanged();
	/** \brief Notifies all that an entry changed. */
	void NotifyEntryChanged( lpeLangPackEntry *entry );
	/** \brief Notifies all that an entry name changed. */
	void NotifyEntryNameChanged( lpeLangPackEntry *entry );
	/** \brief Notifies all that one or more entries changed selection. */
	void NotifyEntrySelectionChanged();
	/** \brief Active entry changed. */
	void NotifyActiveEntryChanged();
	/*@}*/
};

#endif
