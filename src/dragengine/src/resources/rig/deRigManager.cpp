/* 
 * Drag[en]gine Game Engine
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
#include "deRig.h"
#include "deRigBuilder.h"
#include "deRigManager.h"
#include "../../deEngine.h"
#include "../../common/exceptions.h"
#include "../../common/file/decPath.h"
#include "../../common/file/decBaseFileReader.h"
#include "../../common/file/decBaseFileWriter.h"
#include "../../filesystem/deVirtualFileSystem.h"
#include "../../systems/deModuleSystem.h"
#include "../../systems/dePhysicsSystem.h"
#include "../../systems/modules/deLoadableModule.h"
#include "../../systems/modules/rig/deBaseRigModule.h"



// Class deRigManager
///////////////////////

// Constructor, destructor
////////////////////////////

deRigManager::deRigManager( deEngine *engine ) : deFileResourceManager( engine, ertRig ){
	SetLoggingName( "rig" );
}

deRigManager::~deRigManager(){
	ReleaseLeakingResources();
}

// management
int deRigManager::GetRigCount() const{
	return pRigs.GetCount();
}

deRig *deRigManager::GetRootRig() const{
	return ( deRig* )pRigs.GetRoot();
}

deRig *deRigManager::GetRigWith( const char *filename ) const{
	return GetRigWith( GetEngine()->GetVirtualFileSystem(), filename );
}

deRig *deRigManager::GetRigWith( deVirtualFileSystem *vfs, const char *filename ) const{
	deRig * const rig = ( deRig* )pRigs.GetWithFilename( vfs, filename );
	return rig && ! rig->GetOutdated() ? rig : NULL;
}

deRig *deRigManager::CreateRig( const char *filename, deRigBuilder &builder ){
	return CreateRig( GetEngine()->GetVirtualFileSystem(), filename, builder );
}

deRig *deRigManager::CreateRig( deVirtualFileSystem *vfs, const char *filename, deRigBuilder &builder ){
	if( ! vfs || ! filename ){
		DETHROW( deeInvalidParam );
	}
	deRig *rig = NULL, *findRig;
	
	try{
		// check if the rigation with this filename already exists. this check
		// is only done if the filename is not empty in which case an unnamed
		// rig is created
		if( filename[ 0 ] != '\0' ){
			findRig = ( deRig* )pRigs.GetWithFilename( vfs, filename );
			if( findRig && ! findRig->GetOutdated() ){
				DETHROW( deeInvalidParam );
			}
		}
		
		// create rigation using the builder
		rig = new deRig( this, vfs, filename, decDateTime::GetSystemTime() );
		builder.BuildRig( rig );
		
		// check rig
		if( ! rig->Verify() ){
			DETHROW( deeInvalidParam );
		}
		
		// prepare rig
		rig->Prepare();
		
		// load system peers
		GetPhysicsSystem()->LoadRig( rig );
		
		// add rigation
		pRigs.Add( rig );
		
	}catch( const deException &e ){
		if( rig ){
			rig->FreeReference();
		}
		LogErrorFormat( "Creating rig '%s' failed", filename );
		LogException( e );
		throw;
	}
	
	return rig;
}

deRig *deRigManager::LoadRig( const char *filename, const char *basePath ){
	return LoadRig( GetEngine()->GetVirtualFileSystem(), filename, basePath );
}

deRig *deRigManager::LoadRig( deVirtualFileSystem *vfs, const char *filename, const char *basePath ){
	if( ! vfs || ! filename || filename[ 0 ] == '\0' || ! basePath ){
		DETHROW( deeInvalidParam );
	}
	
	decBaseFileReader *fileReader=NULL;
	deBaseRigModule *module;
	deRig *rig=NULL, *findRig;
	decPath path;
	
	try{
		// locate file
		if( ! FindFileForReading( path, *vfs, filename, basePath ) ){
			DETHROW_INFO( deeFileNotFound, filename );
		}
		const TIME_SYSTEM modificationTime = vfs->GetFileModificationTime( path );
		
		// check if the rig with this filename already exists
		findRig = ( deRig* )pRigs.GetWithFilename( vfs, path.GetPathUnix() );
		
		if( findRig && findRig->GetModificationTime() != modificationTime ){
			LogInfoFormat( "Rig '%s' (base path '%s') changed on VFS: Outdating and Reloading",
				filename, basePath ? basePath : "" );
			findRig->MarkOutdated();
			findRig = NULL;
		}
		
		if( findRig ){
			findRig->AddReference();
			rig = findRig;
			
		}else{
			// find the module able to handle this rig file
			module = ( deBaseRigModule* )GetModuleSystem()->GetModuleAbleToLoad(
				deModuleSystem::emtRig, path.GetPathUnix() );
			// load the file with it
			fileReader = OpenFileForReading( *vfs, path.GetPathUnix() );
			rig = new deRig( this, vfs, path.GetPathUnix(), modificationTime );
			rig->SetAsynchron( false );
			module->LoadRig( *fileReader, *rig );
			fileReader->FreeReference(); fileReader = NULL;
			// check rig
			if( ! rig->Verify() ) DETHROW( deeInvalidParam );
			// prepare rig
			rig->Prepare();
			// load system peers
			GetPhysicsSystem()->LoadRig( rig );
			// add rig
			pRigs.Add( rig );
//			LogInfoFormat( "Loading '%s' succeeded", path->GetPath() );
		}
		
	}catch( const deException & ){
		LogErrorFormat( "Loading rig '%s' (base path '%s') failed", filename, basePath ? basePath : "" );
		if( fileReader ){
			fileReader->FreeReference();
		}
		if( rig ){
			rig->FreeReference();
		}
		throw;
	}
	
	if( rig ) rig->SetAsynchron( false );
	return rig;
}

void deRigManager::AddLoadedRig( deRig *rig ){
	if( ! rig ){
		DETHROW( deeInvalidParam );
	}
	
	pRigs.Add( rig );
}



void deRigManager::ReleaseLeakingResources(){
	const int count = GetRigCount();
	
	if( count > 0 ){
		deRig *rig = ( deRig* )pRigs.GetRoot();
		
		LogWarnFormat( "%i leaking rigs", count );
		
		while( rig ){
			LogWarnFormat( "- %s", rig->GetFilename().GetString() );
			rig = ( deRig* )rig->GetLLManagerNext();
		}
		
		pRigs.RemoveAll(); // wo do not delete them to avoid crashes. better leak than crash
	}
}



// systems support
////////////////////

void deRigManager::SystemPhysicsLoad(){
	deRig *rig = ( deRig* )pRigs.GetRoot();
	dePhysicsSystem &phySys = *GetPhysicsSystem();
	
	while( rig ){
		if( ! rig->GetPeerPhysics() ){
			phySys.LoadRig( rig );
		}
		
		rig = ( deRig* )rig->GetLLManagerNext();
	}
}

void deRigManager::SystemPhysicsUnload(){
	deRig *rig = ( deRig* )pRigs.GetRoot();
	
	while( rig ){
		rig->SetPeerPhysics( NULL );
		rig = ( deRig* )rig->GetLLManagerNext();
	}
}

void deRigManager::RemoveResource( deResource *resource ){
	pRigs.RemoveIfPresent( resource );
}
