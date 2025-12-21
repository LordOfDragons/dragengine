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

#ifndef _LPELANGPACKENTRY_H_
#define _LPELANGPACKENTRY_H_

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decTOrderedSet.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>

class lpeLangPack;



/**
 * Language pack entry.
 */
class lpeLangPackEntry : public deObject{
public:
	typedef deTObjectReference<lpeLangPackEntry> Ref;
	typedef decTObjectOrderedSet<lpeLangPackEntry> List;
	
	
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
	lpeLangPackEntry(const lpeLangPackEntry &entry);
	
protected:
	/** Cleans up the entry. */
	~lpeLangPackEntry() override;
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Retrieves the parent language pack or nullptr if there is none. */
	inline lpeLangPack *GetLangPack() const{ return pLangPack; }
	
	/** Sets the parent language pack or nullptr if there is none. */
	void SetLangPack(lpeLangPack *langpack);
	
	
	
	/** Retrieves the name. */
	inline const decString &GetName() const{ return pName; }
	
	/** Sets the name. */
	void SetName(const char *name);
	
	/** Retrieves the textu. */
	inline const decUnicodeString &GetText() const{ return pText; }
	
	/** Sets the text. */
	void SetText(const decUnicodeString &text);
	
	
	
	/** Determines if the entry is the active one. */
	inline bool GetActive() const{ return pActive; }
	
	/** Sets if the entry is the active one. */
	void SetActive(bool active);
	
	/** Determines if the entry is selected. */
	inline bool GetSelected() const{ return pSelected; }
	
	/** Sets if the entry is selected. */
	void SetSelected(bool selected);
	
	
	
	/** Notifies the listeners that the entry changed. */
	void NotifyChanged();
	
	/** Notifies the listeners that the entry name changed. */
	void NotifyNameChanged();
	/*@}*/
};

#endif
