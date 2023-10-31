/* 
 * Drag[en]gine IGDE Conversation Editor
 *
 * Copyright (C) 2023, Roland Plüss (roland@rptd.ch)
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

#ifndef _CELANGPACK_H_
#define _CELANGPACK_H_

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decObjectOrderedSet.h>
#include <dragengine/common/string/decStringList.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>

class ceLangPackEntry;


/**
 * Language pack.
 */
class ceLangPack : public deObject{
public:
	typedef deTObjectReference<ceLangPack> Ref;
	
	
	
private:
	decString pPath;
	
	decString pIdentifier;
	decUnicodeString pName;
	decUnicodeString pDescription;
	decUnicodeString pMissingText;
	decObjectOrderedSet pEntries;
	
	bool pChanged;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create language pack. */
	ceLangPack( const char *path );
	
protected:
	/** Cleans up language pack. */
	virtual ~ceLangPack() override;
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Path. */
	inline const decString &GetPath() const{ return pPath; }
	
	/** Identifier. */
	inline const decString &GetIdentifier() const{ return pIdentifier; }
	void SetIdentifier( const char *identifier );
	
	/** Name. */
	inline const decUnicodeString &GetName() const{ return pName; }
	void SetName( const decUnicodeString &name );
	
	/** Description. */
	inline const decUnicodeString &GetDescription() const{ return pDescription; }
	void SetDescription( const decUnicodeString &description );
	
	/** Missing text. */
	inline const decUnicodeString &GetMissingText() const{ return pMissingText; }
	void SetMissingText( const decUnicodeString &missingText );
	
	/** Language pack changed and needs to be saved. */
	inline bool GetChanged() const{ return pChanged; }
	void SetChanged( bool changed );
	/*@}*/
	
	
	
	/** \name Entries */
	/*@{*/
	/** Count of entries. */
	int GetEntryCount() const;
	
	/** Entry at index. */
	ceLangPackEntry *GetEntryAt( int index ) const;
	
	/** Named entry or nullptr if absent. */
	ceLangPackEntry *GetEntryNamed( const char *name ) const;
	
	/** Add entry. */
	void AddEntry( ceLangPackEntry *entry );
	
	/** Remove entry. */
	void RemoveEntry( ceLangPackEntry *entry );
	
	/** Remove all entries. */
	void RemoveAllEntries();
	
	/** Get sorted list of entry names. */
	void GetEntryNames( decStringList &list ) const;
	/*@}*/
};

#endif
