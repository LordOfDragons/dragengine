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
#include <stdlib.h>

#include "lpeViewLangPack.h"
#include "lpeViewLangPackListener.h"
#include "../langpack/entry/lpeLangPackEntry.h"

#include <deigde/environment/igdeEnvironment.h>

#include <dragengine/common/exceptions.h>



// Class lpeViewLangPackListener
//////////////////////////////////

// Constructor, destructor
////////////////////////////

lpeViewLangPackListener::lpeViewLangPackListener(lpeViewLangPack &panel) :
pPanel(panel){
}

lpeViewLangPackListener::~lpeViewLangPackListener(){
}



// Management
///////////////

void lpeViewLangPackListener::LangPackChanged(lpeLangPack *langpack){
	if(pPanel.GetLangPack() != langpack){
		return;
	}
	
	pPanel.UpdateEntries();
}



void lpeViewLangPackListener::EntryStructureChanged(lpeLangPack *langpack){
	if(pPanel.GetLangPack() != langpack){
		return;
	}
	
	pPanel.UpdateEntries();
}

void lpeViewLangPackListener::EntryChanged(lpeLangPack *langpack, lpeLangPackEntry *entry){
	if(pPanel.GetLangPack() != langpack){
		return;
	}
	
	if(entry->GetActive()){
		pPanel.UpdateActiveEntry();
		
	}else{
		pPanel.UpdateEntry(entry);
	}
}

void lpeViewLangPackListener::EntryNameChanged(lpeLangPack *langpack, lpeLangPackEntry *entry){
	if(pPanel.GetLangPack() != langpack){
		return;
	}
	
	if(pPanel.GetReferenceLangPack()){
		pPanel.UpdateEntries();
		
	}else{
		if(entry->GetActive()){
			pPanel.UpdateActiveEntry();
		}
		pPanel.SortEntries();
	}
}

void lpeViewLangPackListener::EntrySelectionChanged(lpeLangPack *langpack){
	if(pPanel.GetLangPack() != langpack){
		return;
	}
	
	pPanel.UpdateEntrySelection();
}

void lpeViewLangPackListener::ActiveEntryChanged(lpeLangPack *langpack){
	if(pPanel.GetLangPack() != langpack){
		return;
	}
	
	pPanel.SelectActiveEntry();
}
