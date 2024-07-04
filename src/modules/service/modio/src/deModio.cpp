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

#include <string.h>

#include "deModio.h"
#include "deModioService.h"
#include "deModioModConfig.h"

#include <dragengine/deEngine.h>
#include <dragengine/app/deOS.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/filesystem/deVFSRedirect.h>
#include <dragengine/filesystem/deVFSDiskDirectory.h>
#include <dragengine/systems/deModuleSystem.h>
#include <dragengine/systems/modules/deLoadableModule.h>


// export definition
#ifdef __cplusplus
extern "C" {
#endif
MOD_ENTRY_POINT_ATTR deBaseModule *ModioCreateModule( deLoadableModule *loadableModule );
#ifdef __cplusplus
}
#endif


// entry function
// used to create a dragonscript scripting module.
// has to be named CreateModule returning deBaseModule.
// returns NULL on error.
/////////////////////////////////////////////////////////

deBaseModule *ModioCreateModule( deLoadableModule *loadableModule ){
	try{
		return new deModio( *loadableModule );
	}catch( ... ){
		return nullptr;
	}
}


// Class deModio
//////////////////

// Constructor, destructor
////////////////////////////

deModio::deModio( deLoadableModule& loadableModule ) :
deBaseServiceModule( loadableModule ),
pRequiresEventHandlingCount( 0 )
{
	pLoadConfig();
}

deModio::~deModio(){
}



// Management
///////////////

decStringSet deModio::GetSupportedServices(){
	decStringSet names;
	names.Add( deModioService::serviceName );
	return names;
}

void deModio::SetModConfigs( const decObjectList &configs ){
	pModConfigs = configs;
}

void deModio::SetDisabledMods(const decStringSet &mods){
	if( mods == pDisabledMods ){
		return;
	}
	
	pDisabledMods = mods;
	
	pUpdateVFS();
	pSaveConfig();
}

bool deModio::GetModDisabled( const decString &id ) const{
	return pDisabledMods.Has( id );
}

void deModio::SetModDisabled( const decString &id, bool disabled ){
	decStringSet mods( pDisabledMods );
	
	if( disabled ){
		mods.Add( id );
		
	}else{
		mods.Remove( id );
	}
	
	SetDisabledMods( mods );
}

void deModio::ActiveModsChanged(){
	pUpdateVFS();
	pSaveConfig();
}

deVFSContainer *deModio::GetModVFSContainer( const decString &id ) const{
	deObject *container = nullptr;
	return pModVFSContainers.GetAt( id, &container ) ? ( deVFSContainer* )container : nullptr;
}

deBaseServiceService* deModio::CreateService( deService *service,
const char *name, const deServiceObject::Ref &data ){
	DEASSERT_NOTNULL( service )
	
	if( strcmp( name, deModioService::serviceName ) == 0 ){
		LogInfo( "deModioService: Create" );
		return new deModioService( *this, service, data );
	}
	
	return nullptr;
}

void deModio::FrameUpdate( float elapsed ){
	if( pRequiresEventHandlingCount > 0 ){
		Modio::RunPendingHandlers();
	}
	
	if( deModioService::Global() ){
		deModioService::Global()->FrameUpdate( elapsed );
	}
}

void deModio::AddVFSContainers( deVirtualFileSystem &vfs, const char *stage ){
	DEASSERT_NOTNULL( stage )
	
	if( strcmp( stage, deModuleSystem::VFSStageMods ) == 0 ){
		LogInfo( "Add VFS containers for stage 'VFSStageMods'" );
		
		if( ! pVFSMods ){
			pVFSMods.TakeOver( new deVirtualFileSystem );
		}
		
		const decPath rootDir( decPath::CreatePathUnix( "/" ) );
		vfs.AddContainer( deVFSRedirect::Ref::New(
			new deVFSRedirect( rootDir, rootDir, pVFSMods, true ) ) );
		
		pUpdateVFS();
	}
}

void deModio::AddRequiresEventHandlingCount(){
	pRequiresEventHandlingCount++;
}

void deModio::RemoveRequiresEventHandlingCount(){
	if( pRequiresEventHandlingCount == 0 ){
		LogWarn("RemoveRequiresEventHandlingCount called with pRequiresEventHandlingCount == 0");
		return;
	}
	
	pRequiresEventHandlingCount--;
}



// Private Functions
//////////////////////

#define DEM_PATH_MODS "/config/mods"

void deModio::pLoadConfig(){
	LogInfo( "Load configuration");
	pModConfigs.RemoveAll();
	pDisabledMods.RemoveAll();
	
	try{
		const decBaseFileReader::Ref reader( GetVFS().OpenFileForReading(
			decPath::CreatePathUnix( DEM_PATH_MODS ) ) );
		
		const int version = reader->ReadByte();
		switch( version ){
		case 0:
			pLoadConfigV0( reader );
			break;
			
		default:
			DETHROW_INFO( deeInvalidParam, "Unsupported version" );
		}
		
	}catch( const deException &e ){
		LogException( e );
		LogError( "Load configuration failed. Delete configuration.");
		pDeleteConfig();
	}
}

void deModio::pLoadConfigV0( decBaseFileReader &reader ){
	int i, count;
	
	count = reader.ReadInt();
	for( i=0; i<count; i++ ){
		pModConfigs.Add( deModioModConfig::Ref::New( new deModioModConfig( reader ) ) );
	}
	
	count = reader.ReadInt();
	for( i=0; i<count; i++ ){
		pDisabledMods.Add( reader.ReadString8() );
	}
}

void deModio::pSaveConfig(){
	LogInfo( "Save configuration");
	int i, count;
	
	try{
		const decBaseFileWriter::Ref writer( GetVFS().OpenFileForWriting(
			decPath::CreatePathUnix( DEM_PATH_MODS ) ) );
		writer->WriteByte( 0 );
		
		count = pModConfigs.GetCount();
		writer->WriteInt( count );
		for( i=0; i<count; i++ ){
			( ( deModioModConfig* )pModConfigs.GetAt( i ) )->WriteToFile( writer );
		}
		
		count = pDisabledMods.GetCount();
		writer->WriteInt( count );
		for( i=0; i<count; i++ ){
			writer->WriteString8( pDisabledMods.GetAt( i ) );
		}
		
	}catch( const deException &e ){
		LogException( e );
		LogError( "Save configuration failed. Delete configuration.");
		pDeleteConfig();
	}
}

void deModio::pDeleteConfig(){
	const decPath path( decPath::CreatePathUnix( DEM_PATH_MODS ) );
	deVirtualFileSystem &vfs = GetVFS();
	
	if( vfs.ExistsFile( path ) ){
		try{
			vfs.DeleteFile( path );
			
		}catch( const deException &e ){
			LogException( e );
		}
	}
}

void deModio::pUpdateVFS(){
	const decPath rootPath( decPath::CreatePathUnix( "/" ) );
	const int count = pModConfigs.GetCount();
	deVFSDiskDirectory::Ref vfsContainer;
	bool requiresSaving = false;
	int i;
	
	LogInfoFormat( "Updating Mods VFS: %d modifications", count );
	pVFSMods->RemoveAllContainers();
	pModVFSContainers.RemoveAll();
	
	for( i=0; i<count; i++ ){
		const deModioModConfig &config = *( ( deModioModConfig* )pModConfigs.GetAt( i ) );
		if( pDisabledMods.Has( config.id ) ){
			LogInfoFormat( "- %s: Mod disabled. Skipping mod", config.id.GetString() );
			continue;
		}
		
		try{
			LogInfoFormat( "- %s: Add mod using path '%s'", config.id.GetString(), config.path.GetString() );
			vfsContainer.TakeOver( new deVFSDiskDirectory( rootPath,
				decPath::CreatePathNative( config.path ), true ) );
			pVFSMods->AddContainer( vfsContainer );
			pModVFSContainers.SetAt( config.id, vfsContainer );
			
		}catch( const deException &e ){
			LogException( e );
			LogWarnFormat( "Adding mod failed. Disable mod and skip it" );
			pDisabledMods.Add( config.id );
			requiresSaving = true;
		}
	}
	
	if( requiresSaving ){
		pSaveConfig();
	}
}
