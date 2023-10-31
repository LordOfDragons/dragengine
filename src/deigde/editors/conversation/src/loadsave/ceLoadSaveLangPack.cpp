/* 
 * Drag[en]gine IGDE Conversation Editor
 *
 * Copyright (C) 2023, Roland Plüss (roland@rptd.ch)
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

#include "ceLoadSaveLangPack.h"
#include "../langpack/ceLangPack.h"
#include "../langpack/ceLangPackEntry.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/resources/localization/deLanguagePack.h>
#include <dragengine/resources/localization/deLanguagePackEntry.h>
#include <dragengine/resources/localization/deLanguagePackBuilder.h>
#include <dragengine/resources/localization/deLanguagePackManager.h>
#include <dragengine/systems/modules/langpack/deBaseLanguagePackModule.h>
#include <dragengine/systems/modules/deLoadableModule.h>



// Class ceLoadSaveLangPack
/////////////////////////////

// Constructor, destructor
////////////////////////////

ceLoadSaveLangPack::ceLoadSaveLangPack( deBaseLanguagePackModule &module ) :
pModule( module )
{
	const deLoadableModule &loadableModule = module.GetLoadableModule();
	const decStringList &patternList = loadableModule.GetPatternList();
	const int patternCount = patternList.GetCount();
	int i;
	
	pName = loadableModule.GetName();
	
	for( i=0; i<patternCount; i++ ){
		if( i > 0 ){
			pPattern.AppendCharacter( ',' );
		}
		pPattern.AppendCharacter( '*' );
		pPattern.Append( patternList.GetAt( i ) );
	}
}

ceLoadSaveLangPack::~ceLoadSaveLangPack(){
}



// Management
///////////////

class cLoadLangPackLoader : public deLanguagePackBuilder{
public:
	cLoadLangPackLoader(){ }
	void BuildLanguagePack( deLanguagePack& ){ }
};

void ceLoadSaveLangPack::LoadLangPack( ceLangPack &langpack, decBaseFileReader &file ){
	cLoadLangPackLoader builder;
	const deLanguagePack::Ref engLangPack( deLanguagePack::Ref::New(
		pModule.GetGameEngine()->GetLanguagePackManager()->CreateLanguagePack( "", builder ) ) );
	pModule.LoadLanguagePack( file, engLangPack );
	
	langpack.SetIdentifier( engLangPack->GetIdentifier() );
	langpack.SetName( engLangPack->GetName() );
	langpack.SetDescription( engLangPack->GetDescription() );
	langpack.SetMissingText( engLangPack->GetMissingText() );
	
	const int entryCount = engLangPack->GetEntryCount();
	int i;
	
	for( i=0; i<entryCount; i++ ){
		const deLanguagePackEntry &engEntry = engLangPack->GetEntryAt( i );
		
		const ceLangPackEntry::Ref entry( ceLangPackEntry::Ref::New( new ceLangPackEntry ) );
		entry->SetName( engEntry.GetName() );
		entry->SetText( engEntry.GetText() );
		langpack.AddEntry( entry );
	}
}

class cSaveLangPackLoader : public deLanguagePackBuilder{
	const ceLangPack &pLangPack;
	
public:
	cSaveLangPackLoader( const ceLangPack &langpack ) : pLangPack( langpack ){}
	
	virtual void BuildLanguagePack( deLanguagePack &langPack ) override{
		const int entryCount = pLangPack.GetEntryCount();
		int i;
		
		langPack.SetIdentifier( pLangPack.GetIdentifier() );
		langPack.SetName( pLangPack.GetName() );
		langPack.SetDescription( pLangPack.GetDescription() );
		langPack.SetMissingText( pLangPack.GetMissingText() );
		
		langPack.SetEntryCount( entryCount );
		for( i=0; i<entryCount; i++ ){
			const ceLangPackEntry &entry = *pLangPack.GetEntryAt( i );
			deLanguagePackEntry &engEntry = langPack.GetEntryAt( i );
			
			engEntry.SetName( entry.GetName() );
			engEntry.SetText( entry.GetText() );
		}
	}
};

void ceLoadSaveLangPack::SaveLangPack( const ceLangPack &langpack, decBaseFileWriter &file ){
	cSaveLangPackLoader builder( langpack );
	pModule.SaveLanguagePack( file, deLanguagePack::Ref::New(
		pModule.GetGameEngine()->GetLanguagePackManager()->CreateLanguagePack( "", builder ) ) );
}
