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
