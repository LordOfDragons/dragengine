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
 * Language pack entry.
 */
class lpeLangPackEntry : public deObject{
public:
	typedef deTObjectReference<lpeLangPackEntry> Ref;
	
	
	
private:
	lpeLangPack *pLangPack;
	
	decString pName;
	decUnicodeString pText;
	
	bool pSelected;
	bool pActive;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new entry. */
	lpeLangPackEntry();
	
	/** Creates a new entry as a copy of another entry. */
	lpeLangPackEntry( const lpeLangPackEntry &entry );
	
protected:
	/** Cleans up the entry. */
	virtual ~lpeLangPackEntry() override;
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Retrieves the parent language pack or NULL if there is none. */
	inline lpeLangPack *GetLangPack() const{ return pLangPack; }
	
	/** Sets the parent language pack or NULL if there is none. */
	void SetLangPack( lpeLangPack *langpack );
	
	
	
	/** Retrieves the name. */
	inline const decString &GetName() const{ return pName; }
	
	/** Sets the name. */
	void SetName( const char *name );
	
	/** Retrieves the textu. */
	inline const decUnicodeString &GetText() const{ return pText; }
	
	/** Sets the text. */
	void SetText( const decUnicodeString &text );
	
	
	
	/** Determines if the entry is the active one. */
	inline bool GetActive() const{ return pActive; }
	
	/** Sets if the entry is the active one. */
	void SetActive( bool active );
	
	/** Determines if the entry is selected. */
	inline bool GetSelected() const{ return pSelected; }
	
	/** Sets if the entry is selected. */
	void SetSelected( bool selected );
	
	
	
	/** Notifies the listeners that the entry changed. */
	void NotifyChanged();
	
	/** Notifies the listeners that the entry name changed. */
	void NotifyNameChanged();
	/*@}*/
};

#endif
