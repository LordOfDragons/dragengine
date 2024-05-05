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
	decString pIdentifier;
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
	/** \brief Unique identifier. */
	inline const decString &GetIdentifier() const{ return pIdentifier; }
	
	/** \brief Set unique identifier. */
	void SetIdentifier( const decString &identifier );
	
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
