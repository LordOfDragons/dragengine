/* 
 * Drag[en]gine Game Engine
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

#ifndef _DELANGUAGEPACK_H_
#define _DELANGUAGEPACK_H_

#include "../deFileResource.h"
#include "../../common/collection/decPointerDictionary.h"
#include "../../common/string/unicode/decUnicodeString.h"

class deLanguagePackEntry;
class deLanguagePackManager;


/**
 * \brief Language pack for translating text.
 *
 * Language pack contain a list of entries assigning translations to names.
 */
class DE_DLL_EXPORT deLanguagePack : public deFileResource{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deLanguagePack> Ref;
	
	
	
private:
	decUnicodeString pName;
	decUnicodeString pDescription;
	decUnicodeString pMissingText;
	
	deLanguagePackEntry *pEntries;
	int pEntryCount;
	
	decPointerDictionary pLookupTable;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create language pack. */
	deLanguagePack( deLanguagePackManager *manager, deVirtualFileSystem *vfs,
		const char *filename, TIME_SYSTEM modificationTime );
	
protected:
	/**
	 * \brief Clean up translator section.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~deLanguagePack();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Name in native language. */
	inline const decUnicodeString &GetName() const{ return pName; }
	
	/** \brief Set name in native language. */
	void SetName( const decUnicodeString &name );
	
	/** \brief Description in native language. */
	inline const decUnicodeString &GetDescription() const{ return pDescription; }
	
	/** \brief Set description in native language. */
	void SetDescription( const decUnicodeString &description );
	
	/** \brief Missing translation text in native language. */
	inline const decUnicodeString &GetMissingText() const{ return pMissingText; }
	
	/** \brief Set missing translation text in native language. */
	void SetMissingText( const decUnicodeString &missingText );
	
	
	
	/** \brief Number of entries. */
	inline int GetEntryCount() const{ return pEntryCount; }
	
	/**
	 * \brief Set number of entries.
	 * 
	 * All entries are set to empty.
	 */
	void SetEntryCount( int count );
	
	/** \brief Entry at index. */
	deLanguagePackEntry &GetEntryAt( int index );
	const deLanguagePackEntry &GetEntryAt( int index ) const;
	
	/** \brief Index of name entry or -1 if absent. */
	int IndexOfEntryNamed( const char *name ) const;
	
	/** \brief Translation for entry name or missing text if absent. */
	const decUnicodeString &Translate( const char *name ) const;
	
	/** \brief Translation for entry name or default value if absent. */
	const decUnicodeString &Translate( const char *name, const decUnicodeString &defaultValue ) const;
	
	/**
	 * \brief Translation for entry name.
	 * 
	 * If translation entry is present text is set pointing to the translated text and
	 * true is returned. Otherwise false is returned and text is unchanged.
	 */
	bool Translate( const char *name, const decUnicodeString **text ) const;
	
	
	
	/** \brief Verify language pack contains valid data. */
	bool Verify() const;
	
	/** \brief Build look up table. */
	void BuildLookupTable();
	/*@}*/
};

#endif
