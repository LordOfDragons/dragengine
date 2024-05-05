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

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "ceLoadSaveSystem.h"
#include "ceLoadSaveConversation.h"
#include "ceLoadSaveCTS.h"
#include "ceLoadSaveCTA.h"
#include "ceLoadSaveCTGS.h"
#include "ceLoadSaveLangPack.h"
#include "../conversation/ceConversation.h"
#include "../gui/ceWindowMain.h"

#include <deigde/engine/igdeEngineController.h>
#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gamedefinition/igdeGameDefinition.h>
#include <deigde/gui/filedialog/igdeFilePattern.h>
#include <deigde/gui/filedialog/igdeFilePatternList.h>
#include <deigde/module/igdeEditorModule.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileReaderReference.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/file/decBaseFileWriterReference.h>
#include <dragengine/filesystem/dePatternList.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/systems/deModuleSystem.h>
#include <dragengine/systems/modules/deLoadableModule.h>
#include <dragengine/systems/modules/langpack/deBaseLanguagePackModule.h>



// Class ceLoadSaveSystem
////////////////////////////

// Constructor, destructor
////////////////////////////

ceLoadSaveSystem::ceLoadSaveSystem( ceWindowMain &windowMain ) :
pWindowMain( windowMain ),

pLSConversation( NULL ),
pLSCTS( NULL ),
pLSCTA( NULL ),
	pLSCTGS( NULL )
{
	deLogger *const logger = windowMain.GetEnvironment().GetLogger();
	const decString &loggingName = windowMain.GetEditorModule().GetLoggingName();
	
	pLSConversation = new ceLoadSaveConversation( this, logger, loggingName );
	pLSCTS = new ceLoadSaveCTS( this, logger, loggingName );
	pLSCTA = new ceLoadSaveCTA( *this, logger, loggingName );
	pLSCTGS = new ceLoadSaveCTGS( *this, logger, loggingName );
	
	UpdateLSLangPacks();
	pBuildFilePattern();
}

ceLoadSaveSystem::~ceLoadSaveSystem(){
	if( pLSCTGS ){
		delete pLSCTGS;
	}
	if( pLSCTA ){
		delete pLSCTA;
	}
	if( pLSCTS ){
		delete pLSCTS;
	}
	if( pLSConversation ){
		delete pLSConversation;
	}
}



// Management
///////////////

ceConversation *ceLoadSaveSystem::LoadConversation( const char *filename ){
	decBaseFileReaderReference reader;
	reader.TakeOver( pWindowMain.GetEnvironment().GetFileSystemGame()
		->OpenFileForReading( decPath::CreatePathUnix( filename ) ) );
	
	ceConversation *conversation = nullptr;
	
	try{
		conversation = new ceConversation( &pWindowMain.GetEnvironment() );
		pLSConversation->LoadConversation( *conversation, reader, filename );
		
	}catch( const deException & ){
		if( conversation ){
			conversation->FreeReference();
		}
		throw;
	}
	
	return conversation;
}

void ceLoadSaveSystem::SaveConversation( ceConversation *conversation, const char *filename ){
	decBaseFileWriterReference writer;
	writer.TakeOver( pWindowMain.GetEnvironment().GetFileSystemGame()
		->OpenFileForWriting( decPath::CreatePathUnix( filename ) ) );
	pLSConversation->SaveConversation( *conversation, writer );
}



void ceLoadSaveSystem::LoadCTS( const char *filename, ceConversation &conversation ){
	decBaseFileReaderReference reader;
	reader.TakeOver( pWindowMain.GetEnvironment().GetFileSystemGame()
		->OpenFileForReading( decPath::CreatePathUnix( filename ) ) );
	pLSCTS->LoadCTS( conversation, reader );
}

void ceLoadSaveSystem::SaveCTS( const char *filename, ceConversation &conversation ){
	decBaseFileWriterReference writer;
	writer.TakeOver( pWindowMain.GetEnvironment().GetFileSystemGame()
		->OpenFileForWriting( decPath::CreatePathUnix( filename ) ) );
	pLSCTS->SaveCTS( conversation, writer );
}



void ceLoadSaveSystem::LoadCTA( const char *filename, ceConversationActor &actor ){
	decBaseFileReaderReference reader;
	reader.TakeOver( pWindowMain.GetEnvironment().GetFileSystemGame()
		->OpenFileForReading( decPath::CreatePathUnix( filename ) ) );
	pLSCTA->LoadCTA( actor, reader );
}

void ceLoadSaveSystem::SaveCTA( const char *filename, ceConversationActor &actor ){
	decBaseFileWriterReference writer;
	writer.TakeOver( pWindowMain.GetEnvironment().GetFileSystemGame()
		->OpenFileForWriting( decPath::CreatePathUnix( filename ) ) );
	pLSCTA->SaveCTA( actor, writer );
}



void ceLoadSaveSystem::LoadCTGS( const char *filename, ceConversation &conversation ){
	decBaseFileReaderReference reader;
	reader.TakeOver( pWindowMain.GetEnvironment().GetFileSystemGame()
		->OpenFileForReading( decPath::CreatePathUnix( filename ) ) );
	pLSCTGS->LoadCTGS( conversation, reader );
}

void ceLoadSaveSystem::SaveCTGS( const char *filename, const ceConversation &conversation ){
	decBaseFileWriterReference writer;
	writer.TakeOver( pWindowMain.GetEnvironment().GetFileSystemGame()
		->OpenFileForWriting( decPath::CreatePathUnix( filename ) ) );
	pLSCTGS->SaveCTGS( conversation, writer );
}



int ceLoadSaveSystem::GetLSLangPackCount() const{
	return pLSLangPacks.GetCount();
}

ceLoadSaveLangPack *ceLoadSaveSystem::GetLSLangPackAt( int index ) const{
	return ( ceLoadSaveLangPack* )pLSLangPacks.GetAt( index );
}

int ceLoadSaveSystem::IndexOfLSLangPack( ceLoadSaveLangPack *lsLangPack ) const{
	return pLSLangPacks.IndexOf( lsLangPack );
}

bool ceLoadSaveSystem::HasLSLangPack( ceLoadSaveLangPack *lsLangPack ) const{
	return pLSLangPacks.Has( lsLangPack );
}

int ceLoadSaveSystem::IndexOfLSLangPackMatching( const char *filename ){
	const decString testFilename( filename );
	const int count = pLSLangPacks.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( testFilename.MatchesPattern( ( ( ceLoadSaveLangPack* )pLSLangPacks.GetAt( i ) )->GetPattern() ) ){
			return i;
		}
	}
	
	return -1;
}

void ceLoadSaveSystem::AddLSLangPack( ceLoadSaveLangPack *lsLangPack ){
	DEASSERT_FALSE( pLSLangPacks.Has( lsLangPack ) )
	pLSLangPacks.Add( lsLangPack );
}

void ceLoadSaveSystem::RemoveLSLangPack( ceLoadSaveLangPack *lsLangPack ){
	pLSLangPacks.Remove( lsLangPack );
}

void ceLoadSaveSystem::RemoveAllLSLangPacks(){
	pLSLangPacks.RemoveAll();
}

void ceLoadSaveSystem::UpdateLSLangPacks(){
	const deEngine &engine = *pWindowMain.GetEngineController().GetEngine();
	const deModuleSystem &modsys = *engine.GetModuleSystem();
	const int count = modsys.GetModuleCount();
	int i;
	
	pLSLangPacks.RemoveAll();
	
	for( i=0; i<count; i++ ){
		const deLoadableModule &lmodule = *modsys.GetModuleAt( i );
		
		if( lmodule.GetType() == deModuleSystem::emtLanguagePack && lmodule.IsLoaded() ){
			pLSLangPacks.Add( ceLoadSaveLangPack::Ref::New( new ceLoadSaveLangPack(
				*( ( deBaseLanguagePackModule* )lmodule.GetModule() ) ) ) );
		}
	}
}

ceLangPack::Ref ceLoadSaveSystem::LoadLangPack( const char *filename ){
	const int lsIndex = IndexOfLSLangPackMatching( filename );
	DEASSERT_TRUE( lsIndex != -1 )
	
	const ceLangPack::Ref langpack( ceLangPack::Ref::New( new ceLangPack( filename ) ) );
	GetLSLangPackAt( lsIndex )->LoadLangPack( langpack, decBaseFileReader::Ref::New(
		pWindowMain.GetEnvironment().GetFileSystemGame()->OpenFileForReading(
			decPath::CreatePathUnix( filename ) ) ) );
	return langpack;
}

void ceLoadSaveSystem::SaveLangPack( ceLangPack &langpack ){
	const int lsIndex = IndexOfLSLangPackMatching( langpack.GetPath() );
	DEASSERT_TRUE( lsIndex != -1 )
	
	GetLSLangPackAt( lsIndex )->SaveLangPack( langpack, decBaseFileWriter::Ref::New(
		pWindowMain.GetEnvironment().GetFileSystemGame()->OpenFileForWriting(
			decPath::CreatePathUnix( langpack.GetPath() ) ) ) );
	
	langpack.SetChanged( false );
}



// Private Functions
//////////////////////

void ceLoadSaveSystem::pBuildFilePattern(){
	igdeFilePattern *filePattern = nullptr;
	decString pattern;
	int i;
	
	try{
		pattern.Format( "*%s", pLSConversation->GetPattern().GetString() );
		filePattern = new igdeFilePattern( pLSConversation->GetName(),
			pattern, pLSConversation->GetPattern() );
		pFPConversation.AddFilePattern( filePattern );
		filePattern = nullptr;
		
		pattern.Format( "*%s", pLSCTS->GetPattern().GetString() );
		filePattern = new igdeFilePattern( pLSCTS->GetName(), pattern, pLSCTS->GetPattern() );
		pFPCTS.AddFilePattern( filePattern );
		filePattern = nullptr;
		
		pattern.Format( "*%s", pLSCTA->GetPattern().GetString() );
		filePattern = new igdeFilePattern( pLSCTA->GetName(), pattern, pLSCTA->GetPattern() );
		pFPCTA.AddFilePattern( filePattern );
		filePattern = nullptr;
		
		pattern.Format( "*%s", pLSCTGS->GetPattern().GetString() );
		filePattern = new igdeFilePattern( pLSCTGS->GetName(), pattern, pLSCTGS->GetPattern() );
		pFPCTGS.AddFilePattern( filePattern );
		filePattern = nullptr;
		
		// language pack
		const int lslpCount = pLSLangPacks.GetCount();
		
		pFPListLangPack.RemoveAllFilePatterns();
		
		for( i=0; i<lslpCount; i++ ){
			const ceLoadSaveLangPack &lslp = *GetLSLangPackAt( i );
			pattern.Format( "*%s", lslp.GetPattern().GetString() );
			filePattern = new igdeFilePattern( lslp.GetName(), pattern, lslp.GetPattern() );
			pFPListLangPack.AddFilePattern( filePattern );
			filePattern = nullptr;
		}
		
	}catch( const deException & ){
		if( filePattern ){
			delete filePattern;
		}
		throw;
	}
}
