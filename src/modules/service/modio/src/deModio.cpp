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
#include "config/deModioModConfig.h"
#include "config/deModioUserConfig.h"
#include "parameters/deMPLogLevel.h"

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
pRequiresEventHandlingCount( 0 ),
pParamLogLevel( new deMPLogLevel( *this ) )
{
	pParameters.AddParameter( pParamLogLevel );
	
	pInitPath();
	pLoadConfig();
}

deModio::~deModio(){
	StoreFailureStateIfFailed();
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

deModioUserConfig *deModio::GetUserConfigIfPresent( const decString &id ) const{
	deObject *user = nullptr;
	return pUserConfigs.GetAt( id, &user ) ? ( deModioUserConfig* )user : nullptr;
}

deModioUserConfig &deModio::GetUserConfig( const decString &id ){
	deModioUserConfig * const user = GetUserConfigIfPresent( id );
	if( user ){
		return *user;
	}
	
	const deModioUserConfig::Ref newUser( deModioUserConfig::Ref::New(
		new deModioUserConfig( *this, id ) ) );
	pUserConfigs.SetAt( id, newUser );
	
	pSaveConfig();
	
	return newUser;
}

void deModio::SetCurUserId( const decString &id ){
	if( pCurUserId == id ){
		return;
	}
	
	pCurUserId = id;
	
	if( ! pUserConfigs.Has( id ) ){
		pUserConfigs.SetAt( id, deModioUserConfig::Ref::New( new deModioUserConfig( *this, id ) ) );
	}
	
	pSaveConfig();
}

void deModio::UserConfigChanged(){
	pSaveConfig();
}

void deModio::ActivateMods( const decString &userId ){
	LogInfoFormat( "Activate mods: userId='%s'", userId.GetString() );
	
	( void )GetUserConfig( userId ); // ensure user exists
	pCurUserId = userId;
	
	pUpdateVFS();
	
	pSaveConfig();
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
		pCheckFailureState();
		
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

void deModio::StoreFailureStateIfFailed(){
	const bool failure = GetGameEngine()->GetScriptFailed() || GetGameEngine()->GetSystemFailed();
	if( ! failure ){
		return;
	}
	
	LogInfo( "Store failure state: Failed" );
	try{
		decBaseFileWriter::Ref::New( GetVFS().OpenFileForWriting(
			pPathFailureState ) )->WriteByte( '1' );
		
	}catch( const deException &e ){
		LogException( e );
	}
}

void deModio::ClearFailureState(){
	LogInfo( "Clear failure state" );
	try{
		decBaseFileWriter::Ref::New( GetVFS().OpenFileForWriting(
			pPathFailureState ) )->WriteByte( '0' );
		
	}catch( const deException &e ){
		LogException( e );
	}
}



// Parameters
///////////////

int deModio::GetParameterCount() const{
	return pParameters.GetParameterCount();
}

void deModio::GetParameterInfo( int index, deModuleParameter &info ) const{
	info = pParameters.GetParameterAt( index );
}

int deModio::IndexOfParameterNamed( const char *name ) const{
	return pParameters.IndexOfParameterNamed( name );
}

decString deModio::GetParameterValue( const char *name ) const{
	return pParameters.GetParameterNamed( name ).GetParameterValue();
}

void deModio::SetParameterValue( const char *name, const char *value ){
	pParameters.GetParameterNamed( name ).SetParameterValue( value );
}



// Private Functions
//////////////////////

void deModio::pInitPath(){
	const decString &appid = GetGameEngine()->GetCacheAppID();
	decPath basePath( decPath::CreatePathUnix( "/config" ) );
	
	if( appid.IsEmpty() ){
		basePath.AddComponent( "global" );
		
	}else{
		basePath.AddComponent( "local" );
		basePath.AddComponent( appid );
	}
	
	pPathConfig = basePath;
	pPathConfig.AddComponent( "config" );
	
	pPathFailureState = basePath;
	pPathFailureState.AddComponent( "failureState" );
}

void deModio::pLoadConfig(){
	LogInfo( "Load configuration");
	pModConfigs.RemoveAll();
	pUserConfigs.RemoveAll();
	
	try{
		const decBaseFileReader::Ref reader( GetVFS().OpenFileForReading( pPathConfig ) );
		
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
		pModConfigs.RemoveAll();
		pUserConfigs.RemoveAll();
		pDeleteConfig();
	}
}

void deModio::pLoadConfigV0( decBaseFileReader &reader ){
	int i, count;
	
	count = reader.ReadInt();
	for( i=0; i<count; i++ ){
		pModConfigs.Add( deModioModConfig::Ref::New( new deModioModConfig( reader ) ) );
	}
	
	deModioUserConfig::Ref userConfig;
	count = reader.ReadInt();
	for( i=0; i<count; i++ ){
		userConfig.TakeOver( new deModioUserConfig( *this, reader ) );
		pUserConfigs.SetAt( userConfig->GetId(), userConfig );
	}
	
	pCurUserId = reader.ReadString8();
}

void deModio::pSaveConfig(){
	LogInfo( "Save configuration");
	int i, count;
	
	try{
		const decBaseFileWriter::Ref writer( GetVFS().OpenFileForWriting( pPathConfig ) );
		writer->WriteByte( 0 );
		
		count = pModConfigs.GetCount();
		writer->WriteInt( count );
		for( i=0; i<count; i++ ){
			( ( deModioModConfig* )pModConfigs.GetAt( i ) )->WriteToFile( writer );
		}
		
		const decObjectList users( pUserConfigs.GetValues() );
		count = users.GetCount();
		writer->WriteInt( count );
		for( i=0; i<count; i++ ){
			( ( deModioUserConfig* )users.GetAt( i ) )->WriteToFile( writer );
		}
		
		writer->WriteString8( pCurUserId );
		
	}catch( const deException &e ){
		LogException( e );
		LogError( "Save configuration failed. Delete configuration.");
		pDeleteConfig();
	}
}

void deModio::pDeleteConfig(){
	const deVirtualFileSystem &vfs = GetVFS();
	if( ! vfs.ExistsFile( pPathConfig ) ){
		return;
	}
	
	try{
		vfs.DeleteFile( pPathConfig );
		
	}catch( const deException &e ){
		LogException( e );
	}
}

void deModio::pCheckFailureState(){
	bool failure = pReadFailureState();
	if( ! failure ){
		return;
	}
	
	LogWarn( "Failure state is set to 1. Disabling all modifications.");
	deModioUserConfig * const userConfig = GetUserConfigIfPresent( pCurUserId );
	if( ! userConfig ){
		ClearFailureState();
		return;
	}
	
	decStringSet &disabledMods = userConfig->GetDisabledMods();
	const int count = pModConfigs.GetCount();
	bool requiresSaving = false;
	int i;
	
	for( i=0; i<count; i++ ){
		const deModioModConfig &modConfig = *( ( deModioModConfig* )pModConfigs.GetAt( i ) );
		if( userConfig->GetModDisabled( modConfig.id ) ){
			continue;
		}
		
		LogWarnFormat( "-> Disable modification: %s", modConfig.id.GetString() );
		disabledMods.Add( modConfig.id );
		requiresSaving = true;
	}
	
	if( requiresSaving ){
		pSaveConfig();
	}
	ClearFailureState();
}

bool deModio::pReadFailureState(){
	const deVirtualFileSystem &vfs = GetVFS();
	if( ! vfs.ExistsFile( pPathFailureState ) ){
		return false;
	}
	
	try{
		return decBaseFileReader::Ref::New( vfs.OpenFileForReading(
			pPathFailureState ) )->ReadByte() == '1';
		
	}catch( const deException &e ){
		LogException( e );
		return false;
	}
}

void deModio::pUpdateVFS(){
	deModioUserConfig * const userConfig = GetUserConfigIfPresent( pCurUserId );
	const decPath rootPath( decPath::CreatePathUnix( "/" ) );
	const int count = pModConfigs.GetCount();
	deVFSDiskDirectory::Ref vfsContainer;
	bool requiresSaving = false;
	int i;
	
	LogInfoFormat( "Updating Mods VFS: user='%s' mods=%d", pCurUserId.GetString(), count );
	pVFSMods->RemoveAllContainers();
	
	if( ! userConfig ){
		LogInfoFormat( "-> User missing. Skipping" );
		return;
	}
	
	for( i=0; i<count; i++ ){
		const deModioModConfig &config = *( ( deModioModConfig* )pModConfigs.GetAt( i ) );
		if( userConfig->GetModDisabled( config.id ) ){
			LogInfoFormat( "- %s: Mod disabled. Skipping mod", config.id.GetString() );
			continue;
		}
		
		try{
			LogInfoFormat( "- %s: Add mod using path '%s'",
				config.id.GetString(), config.path.GetString() );
			vfsContainer.TakeOver( new deVFSDiskDirectory( rootPath,
				decPath::CreatePathNative( config.path ), true ) );
			pVFSMods->AddContainer( vfsContainer );
			
		}catch( const deException &e ){
			LogException( e );
			LogWarnFormat( "Adding mod failed. Disable mod and skip it" );
			userConfig->GetDisabledMods().Add( config.id );
			requiresSaving = true;
		}
	}
	
	if( requiresSaving ){
		pSaveConfig();
	}
}
