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
#include <string.h>

#include "lpeLangPackEntry.h"
#include "lpeLangPackEntryList.h"
#include "lpeLangPackEntrySelection.h"

#include <dragengine/common/exceptions.h>



// Class lpeLangPackEntrySelection
////////////////////////////////////

// Constructor, destructor
////////////////////////////

lpeLangPackEntrySelection::lpeLangPackEntrySelection(){
	pActive = NULL;
}

lpeLangPackEntrySelection::~lpeLangPackEntrySelection(){
	Reset();
}



// Management
///////////////

void lpeLangPackEntrySelection::Add(lpeLangPackEntry *entry){
	DEASSERT_NOTNULL(entry)
	
	entry->SetSelected(true);
	
	if(!pSelection.Has(entry)){
		pSelection.Add(entry);
	}
}

void lpeLangPackEntrySelection::Remove(lpeLangPackEntry *entry){
	DEASSERT_NOTNULL(entry)
	
	entry->SetSelected(false);
	
	if(pSelection.Has(entry)){
		pSelection.Remove(entry);
	}
}

void lpeLangPackEntrySelection::RemoveAll(){
	const int count = pSelection.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		pSelection.GetAt(i)->SetSelected(false);
	}
	
	pSelection.RemoveAll();
}



bool lpeLangPackEntrySelection::HasActive() const{
	return pActive != NULL;
}

void lpeLangPackEntrySelection::SetActive(lpeLangPackEntry *entry){
	if(entry == pActive){
		return;
	}
	
	if(entry){
		DEASSERT_TRUE(pSelection.Has(entry))
		DEASSERT_TRUE(entry->GetSelected())
	}
	
	if(pActive){
		pActive->SetActive(false);
	}
	
	pActive = entry;
	
	if(entry){
		entry->SetActive(true);
	}
}

void lpeLangPackEntrySelection::ActivateNext(){
	const int count = pSelection.GetCount();
	lpeLangPackEntry *next = NULL;
	int i;
	
	for(i=0; i<count; i++){
		if(pActive != pSelection.GetAt(i)){
			next = pSelection.GetAt(i);
			break;
		}
	}
	
	SetActive(next);
}

void lpeLangPackEntrySelection::Reset(){
	SetActive(nullptr);
	RemoveAll();
}
