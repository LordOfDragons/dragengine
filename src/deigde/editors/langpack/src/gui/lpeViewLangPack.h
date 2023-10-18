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

#ifndef _LPEVIEWLANGPACK_H_
#define _LPEVIEWLANGPACK_H_

#include "../langpack/lpeLangPack.h"
#include "../langpack/lpeLangPackListener.h"

#include <deigde/gui/igdeIconListBoxReference.h>
#include <deigde/gui/igdeTextAreaReference.h>
#include <deigde/gui/igdeTextFieldReference.h>
#include <deigde/gui/layout/igdeContainerBorder.h>

class lpeLangPackEntry;
class lpeWindowMain;



/**
 * Language pack view.
 */
class lpeViewLangPack : public igdeContainerBorder{
private:
	lpeWindowMain &pWindowMain;
	lpeLangPackListener::Ref pListener;
	
	lpeLangPack::Ref pLangPack;
	lpeLangPack::Ref pRefLangPack;
	
	igdeTextFieldReference pEditFilter;
	igdeIconListBoxReference pListEntries;
	igdeTextFieldReference pEditEntryName;
	igdeTextAreaReference pEditEntryText;
	igdeTextFieldReference pEditRefText;
	
	
	
public:
	bool preventUpdate;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create view. */
	lpeViewLangPack( lpeWindowMain &windowMain );
	
protected:
	/** Cleanup view. */
	virtual ~lpeViewLangPack() override;
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Language pack or nullptr. */
	inline const lpeLangPack::Ref &GetLangPack() const{ return pLangPack; }
	
	/** Set language pack or nullptr. */
	void SetLangPack( lpeLangPack *langpack );
	
	/** Reference language pack or nullptr. */
	inline const lpeLangPack::Ref &GetReferenceLangPack() const{ return pRefLangPack; }
	
	/** Set reference language pack or nullptr. */
	void SetReferenceLangPack( lpeLangPack *langpack );
	
	/** Retrieves the active entry or nullptr if there is none. */
	lpeLangPackEntry *GetActiveEntry() const;
	
	
	
	/** Update list with entries. */
	void UpdateEntries();
	
	/** Sorts the entry list. */
	void SortEntries();
	
	/** Select active entry. */
	void SelectActiveEntry();
	
	/** Select entry with name. */
	void SelectEntryNamed( const char *name );
	
	/** Select entry. */
	void SelectEntry( lpeLangPackEntry *entry );
	
	/** Select next missing. */
	void SelectNextMissingEntry();
	
	/** Update active entry. */
	void UpdateActiveEntry();
	
	/** Update entry selection. */
	void UpdateEntrySelection();
	
	/** Update a specific entry. */
	void UpdateEntry( lpeLangPackEntry *entry );
	/*@}*/
};

#endif
