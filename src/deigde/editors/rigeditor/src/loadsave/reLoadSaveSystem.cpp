/* 
 * Drag[en]gine IGDE Rig Editor
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

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "reLoadSaveSystem.h"
#include "reLSRig.h"
#include "../rig/reRig.h"
#include "../rig/constraint/reRigConstraint.h"
#include "../gui/reWindowMain.h"

#include <deigde/engine/igdeEngineController.h>
#include <deigde/gui/filedialog/igdeFilePattern.h>
#include <deigde/gui/filedialog/igdeFilePatternList.h>
#include <deigde/gamedefinition/igdeGameDefinition.h>
#include <deigde/environment/igdeEnvironment.h>

#include <dragengine/deEngine.h>
#include <dragengine/systems/deModuleSystem.h>
#include <dragengine/systems/modules/deLoadableModule.h>
#include <dragengine/systems/modules/rig/deBaseRigModule.h>
#include <dragengine/filesystem/dePatternList.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/file/decBaseFileReaderReference.h>
#include <dragengine/common/file/decDiskFileReader.h>
#include <dragengine/common/file/decBaseFileWriterReference.h>
#include <dragengine/common/file/decDiskFileWriter.h>
#include <dragengine/common/exceptions.h>



// Class reLoadSaveSystem
///////////////////////////

// Constructor, destructor
////////////////////////////

reLoadSaveSystem::reLoadSaveSystem( reWindowMain &windowMain ) :
pWindowMain( windowMain )
{
	pLSRigs = NULL;
	pLSRigCount = 0;
	pLSRigSize = 0;
}

reLoadSaveSystem::~reLoadSaveSystem(){
	pCleanUp();
}
	


// Management
///////////////

reLSRig *reLoadSaveSystem::GetLSRigAt( int index ) const{
	if( index < 0 || index >= pLSRigCount ) DETHROW( deeInvalidParam );
	
	return pLSRigs[ index ];
}

int reLoadSaveSystem::IndexOfLSRig( reLSRig *lsRig ) const{
	if( ! lsRig ) DETHROW( deeInvalidParam );
	int i;
	
	for( i=0; i<pLSRigCount; i++ ){
		if( lsRig == pLSRigs[ i ] ){
			return i;
		}
	}
	
	return -1;
}

bool reLoadSaveSystem::HasLSRig( reLSRig *lsRig ) const{
	if( ! lsRig ) DETHROW( deeInvalidParam );
	int i;
	
	for( i=0; i<pLSRigCount; i++ ){
		if( lsRig == pLSRigs[ i ] ){
			return true;
		}
	}
	
	return false;
}

int reLoadSaveSystem::IndexOfLSRigMatching( const char *filename ){
	const decString testFilename( filename );
	int i;
	
	for( i=0; i<pLSRigCount; i++ ){
		if( testFilename.MatchesPattern( pLSRigs[ i ]->GetPattern() ) ){
			return i;
		}
	}
	
	return -1;
}

void reLoadSaveSystem::AddLSRig( reLSRig *lsRig ){
	if( HasLSRig( lsRig ) ) DETHROW( deeInvalidParam );
	
	if( pLSRigCount == pLSRigSize ){
		int newSize = pLSRigSize * 3 / 2 + 1;
		reLSRig **newArray = new reLSRig*[ newSize ];
		if( ! newArray ) DETHROW( deeOutOfMemory );
		if( pLSRigs ){
			memcpy( newArray, pLSRigs, sizeof( reLSRig* ) * pLSRigSize );
			delete [] pLSRigs;
		}
		pLSRigs = newArray;
		pLSRigSize = newSize;
	}
	
	pLSRigs[ pLSRigCount ] = lsRig;
	pLSRigCount++;
}

void reLoadSaveSystem::RemoveLSRig( reLSRig *lsRig ){
	int i, index = IndexOfLSRig( lsRig );
	if( index == -1 ) DETHROW( deeInvalidParam );
	
	for( i=index+1; i<pLSRigCount; i++ ){
		pLSRigs[ i - 1 ] = pLSRigs[ i ];
	}
	pLSRigCount--;
	
	delete lsRig;
}

void reLoadSaveSystem::RemoveAllLSRigs(){
	while( pLSRigCount > 0 ){
		pLSRigCount--;
		delete pLSRigs[ pLSRigCount ];
	}
}

void reLoadSaveSystem::UpdateLSRigs(){
	deEngine *engine = pWindowMain.GetEngineController().GetEngine();
	deModuleSystem *modSys = engine->GetModuleSystem();
	int m, moduleCount = modSys->GetModuleCount();
	deLoadableModule *loadableModule;
	reLSRig *lsRig = NULL;
	
	// remove all load save rigs
	RemoveAllLSRigs();
	
	try{
		// add a new load save rig for each rig module found in the engine that is also
		// running and usable therefore
		for( m=0; m<moduleCount; m++ ){
			loadableModule = modSys->GetModuleAt( m );
			
			if( loadableModule->GetType() != deModuleSystem::emtRig ) continue;
			if( ! loadableModule->IsLoaded() ) continue;
			
			lsRig = new reLSRig( ( deBaseRigModule* )loadableModule->GetModule() );
			if( ! lsRig ) DETHROW( deeOutOfMemory );
			
			AddLSRig( lsRig );
			lsRig = NULL;
		}
		
	}catch( const deException & ){
		if( lsRig ) delete lsRig;
		throw;
	}
}



reRig *reLoadSaveSystem::LoadRig( const char *filename ){
	const int lsIndex = IndexOfLSRigMatching( filename );
	if( lsIndex == -1 ){
		DETHROW( deeInvalidParam );
	}
	
	decBaseFileReaderReference fileReader;
	fileReader.TakeOver( pWindowMain.GetEnvironment().GetFileSystemGame()
		->OpenFileForReading( decPath::CreatePathUnix( filename ) ) );
	
	deObjectReference refRig;
	refRig.TakeOver( new reRig( &pWindowMain.GetEnvironment() ) );
	reRig * const rig = ( reRig* )( deObject* )refRig;
	
	pLSRigs[ lsIndex ]->LoadRig( rig, fileReader );
	
	rig->SetFilePath( filename );
	rig->SetChanged( false );
	rig->SetSaved( true );
	
	refRig->AddReference(); // required to hand over reference to caller
	return rig;
}

void reLoadSaveSystem::SaveRig( reRig *rig, const char *filename ){
	const int lsIndex = IndexOfLSRigMatching( filename );
	if( lsIndex == -1 ){
		DETHROW( deeInvalidParam );
	}
	
	decBaseFileWriterReference fileWriter;
	fileWriter.TakeOver( pWindowMain.GetEnvironment().GetFileSystemGame()
		->OpenFileForWriting( decPath::CreatePathUnix( filename ) ) );
	pLSRigs[ lsIndex ]->SaveRig( rig, fileWriter );
}




// Private Functions
//////////////////////	

void reLoadSaveSystem::pCleanUp(){
	RemoveAllLSRigs();
	if( pLSRigs ) delete [] pLSRigs;
}
