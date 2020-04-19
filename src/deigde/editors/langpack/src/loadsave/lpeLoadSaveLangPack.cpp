/* 
 * Drag[en]gine IGDE Language Pack Editor
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lpeLoadSaveLangPack.h"
#include "../langpack/lpeLangPack.h"
#include "../langpack/lpeLangPackBuilder.h"
#include "../langpack/entry/lpeLangPackEntry.h"
#include "../gui/lpeWindowMain.h"

#include <deigde/engine/igdeEngineController.h>
#include <deigde/gamedefinition/igdeGameDefinition.h>

#include <dragengine/resources/localization/deLanguagePack.h>
#include <dragengine/resources/localization/deLanguagePackEntry.h>
#include <dragengine/resources/localization/deLanguagePackBuilder.h>
#include <dragengine/resources/localization/deLanguagePackManager.h>
#include <dragengine/systems/modules/langpack/deBaseLanguagePackModule.h>
#include <dragengine/systems/modules/deLoadableModule.h>
#include <dragengine/deEngine.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/exceptions.h>



// Class lpeLoadSaveLangPack
//////////////////////////////

// Constructor, destructor
////////////////////////////

lpeLoadSaveLangPack::lpeLoadSaveLangPack( deBaseLanguagePackModule *module ){
	if( ! module ){
		DETHROW( deeInvalidParam );
	}
	
	const deLoadableModule &loadableModule = module->GetLoadableModule();
	const decStringList &patternList = loadableModule.GetPatternList();
	const int patternCount = patternList.GetCount();
	int i;
	
	pModule = module;
	
	pName = loadableModule.GetName();
	
	for( i=0; i<patternCount; i++ ){
		if( i > 0 ){
			pPattern.AppendCharacter( ',' );
		}
		pPattern.AppendCharacter( '*' );
		pPattern.Append( patternList.GetAt( i ) );
	}
}

lpeLoadSaveLangPack::~lpeLoadSaveLangPack(){
}



// Management
///////////////

void lpeLoadSaveLangPack::SetName( const char *name ){
	if( ! name ){
		DETHROW( deeInvalidParam );
	}
	
	pName = name;
}

void lpeLoadSaveLangPack::SetPattern( const char *pattern ){
	if( ! pattern ){
		DETHROW( deeInvalidParam );
	}
	
	pPattern = pattern;
}



class cDirectLangPackLoader : public deLanguagePackBuilder{
public:
	cDirectLangPackLoader(){
	}
	
	void BuildLanguagePack( deLanguagePack &langPack ){
	}
};

void lpeLoadSaveLangPack::LoadLangPack( lpeLangPack *langpack, decBaseFileReader *file ){
	if( ! langpack || ! file ){
		DETHROW( deeInvalidParam );
	}
	
	deEngine *engine = pModule->GetGameEngine();
	lpeLangPackEntry *entry = NULL;
	deLanguagePack *engLangPack = NULL;
	int i, entryCount;
	
	cDirectLangPackLoader builder;
	
	try{
		engLangPack = engine->GetLanguagePackManager()->CreateLanguagePack( "", builder );
		pModule->LoadLanguagePack( *file, *engLangPack );
		
		langpack->SetName( engLangPack->GetName() );
		langpack->SetDescription( engLangPack->GetDescription() );
		langpack->SetMissingText( engLangPack->GetMissingText() );
		
		entryCount = engLangPack->GetEntryCount();
		
		for( i=0; i<entryCount; i++ ){
			const deLanguagePackEntry &engEntry = engLangPack->GetEntryAt( i );
			
			entry = new lpeLangPackEntry;
			entry->SetName( engEntry.GetName() );
			entry->SetText( engEntry.GetText() );
			
			langpack->AddEntry( entry );
			entry->FreeReference();
			entry = NULL;
		}
		
		engLangPack->FreeReference();
		
	}catch( const deException &e ){
		if( entry ){
			entry->FreeReference();
		}
		if( engLangPack ){
			engLangPack->FreeReference();
		}
		throw;
	}
}

void lpeLoadSaveLangPack::SaveLangPack( lpeLangPack *langpack, decBaseFileWriter *file ){
	if( ! langpack || ! file ){
		DETHROW( deeInvalidParam );
	}
	
	lpeLangPackBuilder builder( langpack );
	deLanguagePack *temporaryLangPack = NULL;
	
	try{
		temporaryLangPack = langpack->GetEngine()->GetLanguagePackManager()->CreateLanguagePack( "", builder );
		pModule->SaveLanguagePack( *file, *temporaryLangPack );
		temporaryLangPack->FreeReference();
		
	}catch( const deException & ){
		if( temporaryLangPack ){
			temporaryLangPack->FreeReference();
		}
		throw;
	}
}
