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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "lpeULangPackEntryRemove.h"
#include "../../langpack/lpeLangPack.h"
#include "../../langpack/entry/lpeLangPackEntry.h"

#include <dragengine/common/exceptions.h>



// Class lpeULangPackEntryRemove
//////////////////////////////////

// Constructor, destructor
////////////////////////////

lpeULangPackEntryRemove::lpeULangPackEntryRemove(lpeLangPack *langpack,
	const lpeLangPackEntry::List &list, lpeLangPack *refLangpack) :
pLangPack(langpack),
pList(list)
{
	const int count = list.GetCount();
	DEASSERT_NOTNULL(langpack)
	DEASSERT_TRUE(count > 0)
	
	if(refLangpack){
		const lpeLangPackEntry::List &refEntries = refLangpack->GetEntries();
		list.Visit([&](const lpeLangPackEntry &e){
			const lpeLangPackEntry::Ref *refEntry = nullptr;
			if(refEntries.Find([&](const lpeLangPackEntry &e2){
				return e2.GetName() == e.GetName();
			}, refEntry)){
				pListRef.Add(*refEntry);
			}
		});
	}
	
	SetShortInfo(count == 1 ? "Remove entry" : "Remove entries");
}

lpeULangPackEntryRemove::~lpeULangPackEntryRemove(){
}



// Management
///////////////

void lpeULangPackEntryRemove::Undo(){
	lpeLangPackEntrySelection &lpes = pLangPack->GetEntrySelection();
	
	lpes.Reset();
	
	pList.Visit([&](lpeLangPackEntry *e){
		pLangPack->AddEntry(e);
		lpes.Add(e);
	});
	
	lpes.ActivateNext();
	
	pLangPack->NotifyEntrySelectionChanged();
	pLangPack->NotifyActiveEntryChanged();
}

void lpeULangPackEntryRemove::Redo(){
	lpeLangPackEntrySelection &lpes = pLangPack->GetEntrySelection();
	
	pList.Visit([&](lpeLangPackEntry *e){
		lpes.Remove(e);
		pLangPack->RemoveEntry(e);
	});
	
	if(pListRef.IsNotEmpty()){
		pListRef.Visit([&](lpeLangPackEntry *e){
			lpes.Add(e);
		});
		lpes.SetActive(pListRef.First());
	}
	
	pLangPack->NotifyEntrySelectionChanged();
	pLangPack->NotifyActiveEntryChanged();
}
