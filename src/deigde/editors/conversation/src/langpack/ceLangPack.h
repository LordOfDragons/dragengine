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
	ceLangPack(const char *path);
	
protected:
	/** Cleans up language pack. */
	~ceLangPack() override;
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Path. */
	inline const decString &GetPath() const{ return pPath; }
	
	/** Identifier. */
	inline const decString &GetIdentifier() const{ return pIdentifier; }
	void SetIdentifier(const char *identifier);
	
	/** Name. */
	inline const decUnicodeString &GetName() const{ return pName; }
	void SetName(const decUnicodeString &name);
	
	/** Description. */
	inline const decUnicodeString &GetDescription() const{ return pDescription; }
	void SetDescription(const decUnicodeString &description);
	
	/** Missing text. */
	inline const decUnicodeString &GetMissingText() const{ return pMissingText; }
	void SetMissingText(const decUnicodeString &missingText);
	
	/** Language pack changed and needs to be saved. */
	inline bool GetChanged() const{ return pChanged; }
	void SetChanged(bool changed);
	/*@}*/
	
	
	
	/** \name Entries */
	/*@{*/
	/** Count of entries. */
	int GetEntryCount() const;
	
	/** Entry at index. */
	ceLangPackEntry *GetEntryAt(int index) const;
	
	/** Named entry or nullptr if absent. */
	ceLangPackEntry *GetEntryNamed(const char *name) const;
	
	/** Add entry. */
	void AddEntry(ceLangPackEntry *entry);
	
	/** Remove entry. */
	void RemoveEntry(ceLangPackEntry *entry);
	
	/** Remove all entries. */
	void RemoveAllEntries();
	
	/** Get sorted list of entry names. */
	void GetEntryNames(decStringList &list) const;
	/*@}*/
};

#endif
