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

#include "lpeLangPack.h"
#include "lpeLangPackBuilder.h"
#include "entry/lpeLangPackEntry.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/localization/deLanguagePack.h>
#include <dragengine/resources/localization/deLanguagePackEntry.h>



// Class lpeLangPackBuilder
//////////////////////////////

// Constructor, destructor
////////////////////////////

lpeLangPackBuilder::lpeLangPackBuilder(lpeLangPack *langpack){
	if(!langpack){
		DETHROW(deeInvalidParam);
	}
	
	pLangPack = langpack;
}

lpeLangPackBuilder::~lpeLangPackBuilder(){
}



// Management
///////////////

void lpeLangPackBuilder::BuildLanguagePack(deLanguagePack &langPack){
	const lpeLangPackEntryList &entryList = pLangPack->GetEntryList();
	const int entryCount = entryList.GetCount();
	int i;
	
	langPack.SetEntryCount(entryCount);
	
	langPack.SetIdentifier(pLangPack->GetIdentifier());
	langPack.SetName(pLangPack->GetName());
	langPack.SetDescription(pLangPack->GetDescription());
	langPack.SetMissingText(pLangPack->GetMissingText());
	
	for(i=0; i<entryCount; i++){
		const lpeLangPackEntry &entry = *entryList.GetAt(i);
		deLanguagePackEntry &engEntry = langPack.GetEntryAt(i);
		
		engEntry.SetName(entry.GetName());
		engEntry.SetText(entry.GetText());
	}
}
