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

#include <deigde/gui/igdeIconListBoxReference.h>
#include <deigde/gui/igdeTextAreaReference.h>
#include <deigde/gui/igdeTextFieldReference.h>
#include <deigde/gui/layout/igdeContainerBorder.h>

class lpeLangPackEntry;
class lpeViewLangPackListener;
class lpeWindowMain;
class lpeLangPack;



/**
 * \brief Language pack view.
 */
class lpeViewLangPack : public igdeContainerBorder{
private:
	lpeWindowMain &pWindowMain;
	lpeViewLangPackListener *pListener;
	
	lpeLangPack *pLangPack;
	
	igdeTextFieldReference pEditFilter;
	igdeIconListBoxReference pListEntries;
	igdeTextFieldReference pEditEntryName;
	igdeTextAreaReference pEditEntryText;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create view. */
	lpeViewLangPack( lpeWindowMain &windowMain );
	
protected:
	/** \brief Cleanup view. */
	virtual ~lpeViewLangPack();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Language pack or NULL. */
	inline lpeLangPack *GetLangPack() const{ return pLangPack; }
	
	/** \brief Set language pack or NULL. */
	void SetLangPack( lpeLangPack *langpack );
	
	/** \brief Retrieves the active entry or NULL if there is none. */
	lpeLangPackEntry *GetActiveEntry() const;
	
	
	
	/** \brief Update list with entries. */
	void UpdateEntries();
	
	/** \brief Sorts the entry list. */
	void SortEntries();
	
	/** \brief Select active entry. */
	void SelectActiveEntry();
	
	/** \brief Update active entry. */
	void UpdateActiveEntry();
	
	/** \brief Update entry selection. */
	void UpdateEntrySelection();
	
	/** \brief Update a specific entry. */
	void UpdateEntry( lpeLangPackEntry *entry );
	/*@}*/
};

#endif
