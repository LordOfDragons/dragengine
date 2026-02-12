/*
 * MIT License
 *
 * Copyright (C) 2026, DragonDreams GmbH (info@dragondreams.ch)
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

#ifndef _IGDELANGUAGEPACK_H_
#define _IGDELANGUAGEPACK_H_

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decTDictionary.h>
#include <dragengine/common/collection/decTOrderedSet.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>

class deLogger;
class decBaseFileReader;


/**
 * \brief Language pack.
 */
class DE_DLL_EXPORT igdeLanguagePack : public deObject{
public:
	/** \brief Reference. */
	using Ref = deTObjectReference<igdeLanguagePack>;
	
	/** \brief Dictionary of entries. */
	using EntryMap = decTStringDictionary<decUnicodeString>;
	
	/** \brief Language pack list. */
	class List: public decTObjectOrderedSet<igdeLanguagePack>{
	public:
		using decTObjectOrderedSet<igdeLanguagePack>::decTOrderedSet;
		
		/** \brief Find language pack for language. */
		igdeLanguagePack::Ref FindByLanguage(const decString &language) const{
			const Ref *found;
			return Find(found, [&](const igdeLanguagePack &lp){
				return lp.GetLanguage() == language;
			}) ? *found : Ref();
		}
	};
	
	
private:
	decString pLanguage;
	decUnicodeString pName;
	EntryMap pEntries;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create language pack. */
	igdeLanguagePack();
	
protected:
	/** \brief Clean up language pack. */
	~igdeLanguagePack() override;
	/*@}*/
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Language. */
	inline const decString &GetLanguage() const{ return pLanguage; }
	
	/** \brief Set language. */
	void SetLanguage(const decString &language);
	
	/** \brief Name. */
	inline const decUnicodeString &GetName() const{ return pName; }
	
	/** \brief Set name. */
	void SetName(const decUnicodeString &name);
	
	/** \brief Entries. */
	inline EntryMap &GetEntries(){ return pEntries; }
	inline const EntryMap &GetEntries() const{ return pEntries; }
	
	/** \brief Load from .delangpack file. */
	void LoadFromFile(deLogger &logger, decBaseFileReader &reader);
	/*@}*/
};

#endif
