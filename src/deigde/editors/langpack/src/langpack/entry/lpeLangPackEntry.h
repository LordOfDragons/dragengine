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

#ifndef _LPELANGPACKENTRY_H_
#define _LPELANGPACKENTRY_H_

#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>
#include <dragengine/deObject.h>

class lpeLangPack;



/**
 * \brief Language pack entry.
 */
class lpeLangPackEntry : public deObject{
private:
	lpeLangPack *pLangPack;
	
	decString pName;
	decUnicodeString pText;
	
	bool pSelected;
	bool pActive;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates a new entry. */
	lpeLangPackEntry();
	/** \brief Creates a new entry as a copy of another entry. */
	lpeLangPackEntry( const lpeLangPackEntry &entry );
	/** \brief Cleans up the entry. */
	virtual ~lpeLangPackEntry();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Retrieves the parent language pack or NULL if there is none. */
	inline lpeLangPack *GetLangPack() const{ return pLangPack; }
	/** \brief Sets the parent language pack or NULL if there is none. */
	void SetLangPack( lpeLangPack *langpack );
	
	/** \brief Retrieves the name. */
	inline const decString &GetName() const{ return pName; }
	/** \brief Sets the name. */
	void SetName( const char *name );
	/** \brief Retrieves the textu. */
	inline const decUnicodeString &GetText() const{ return pText; }
	/** \brief Sets the text. */
	void SetText( const decUnicodeString &text );
	
	/** \brief Determines if the entry is the active one. */
	inline bool GetActive() const{ return pActive; }
	/** \brief Sets if the entry is the active one. */
	void SetActive( bool active );
	/** \brief Determines if the entry is selected. */
	inline bool GetSelected() const{ return pSelected; }
	/** \brief Sets if the entry is selected. */
	void SetSelected( bool selected );
	
	/** \brief Notifies the listeners that the entry changed. */
	void NotifyChanged();
	/** \brief Notifies the listeners that the entry name changed. */
	void NotifyNameChanged();
	/*@}*/
};

#endif
