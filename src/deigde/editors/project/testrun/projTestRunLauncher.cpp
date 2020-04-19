/* 
 * DEIGDE Project
 *
 * Copyright (C) 2018, Plüss Roland ( roland@rptd.ch )
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is projributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <stdlib.h>

#include "projTestRunProcess.h"
#include "projTestRunLauncher.h"

#ifdef OS_W32
#include <dragengine/app/deOSWindows.h>
#endif

#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/file/decBaseFileWriterReference.h>
#include <dragengine/filesystem/deVFSContainerReference.h>
#include <dragengine/filesystem/deVFSDiskDirectory.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/logger/deLogger.h>



// Definitions
////////////////

#define FILE_LAUNCHER_CONFIG_SYSTEM		"/config/system/delauncherconsole.xml"
#define FILE_LAUNCHER_CONFIG_USER		"/config/user/delauncherconsole.xml"

#define LOGSOURCE "Test-Run Launcher"



// Class projTestRunLauncher
//////////////////////////////

// Constructors and Destructors
/////////////////////////////////

projTestRunLauncher::projTestRunLauncher( projTestRunProcess &process ) :
pProcess( process ){
}

projTestRunLauncher::~projTestRunLauncher(){
}



// Management
///////////////

void projTestRunLauncher::LocatePath(){
	const char *value;
	decPath path;
	
	// the system wide configuration directory is hard coded at compile time.
	// can be changed at runtime using an environment parameter.
	pPathConfigSystem = LAUNCHER_CONFIG_PATH;
#ifdef OS_W32
	const decString pathEngineBase( deOSWindows().GetPathEngineBase() );
	
	//pPathConfigSystem = deOSWindows::GetRegistryValue( "SOFTWARE\\Drag[en]gine",
	//	"PathLauncherConfig", pPathConfigSystem );
	pPathConfigSystem = pathEngineBase + "\\Launchers\\Config";
#endif
	
	value = getenv( "DELAUNCHER_SYS_CONFIG" );
	if( value ){
		pPathConfigSystem = value;
	}
#ifdef OS_W32
	pPathConfigSystem = deOSWindows::ParseNativePath( pPathConfigSystem );
#endif
	
#ifdef OS_W32
	pPathConfigUser = "@RoamingAppData\\DELaunchers\\Config";
	
#elif defined( OS_BEOS )
	pPathConfigUser = "/boot/home/config/settings/delauncher";
	
#else
	// the user configuration directory is located under the user home directory.
	// can be changed at runtime using an environment parameter.
	value = getenv( "HOME" );
	if( value ){
		path.SetFromNative( value );
		
	}else{
		value = getenv( "USER" );
		
		if( value ){
			path.SetFromNative( "/home" );
			path.AddComponent( value );
			
		}else{
			value = getenv( "LOGUSER" );
			
			if( value ){
				path.SetFromNative( "/home" );
				path.AddComponent( value );
			}
		}
	}
	
	if( path.GetComponentCount() > 0 ){
		path.AddComponent( ".config" );
		path.AddComponent( "delauncher" );
		pPathConfigUser = path.GetPathNative();
	}
#endif
	
	value = getenv( "DELAUNCHER_USER_CONFIG" );
	if( value ){
		pPathConfigUser = value;
	}
#ifdef OS_W32
	pPathConfigUser = deOSWindows::ParseNativePath( pPathConfigUser );
#endif
	
	// the shares directory is hard coded at compile time. can be changed at runtime
	// using an environment parameter.
	pPathShares = LAUNCHER_SHARE_PATH;
#ifdef OS_W32
	//pPathShares = deOSWindows::GetRegistryValue( "SOFTWARE\\Drag[en]gine",
	//	"PathLauncherShares", pPathShares );
	pPathShares = pathEngineBase + "\\Launchers\\Share";
#endif
	
	value = getenv( "DELAUNCHER_SHARES" );
	if( value ){
		pPathShares = value;
	}
#ifdef OS_W32
	pPathShares = deOSWindows::ParseNativePath( pPathShares );
#endif
	
	// the logs directory is located right under the user configuration directory.
	// can be changed at runtime using an environment parameter. for test running logs
	// will be written to the project so this path is not used
	
	pProcess.GetLogger()->LogInfoFormat( LOGSOURCE, "System config path = '%s'",
		pPathConfigSystem.GetString() );
	pProcess.GetLogger()->LogInfoFormat( LOGSOURCE, "User config path = '%s'",
		pPathConfigUser.GetString() );
	pProcess.GetLogger()->LogInfoFormat( LOGSOURCE, "Shares path = '%s'",
		pPathShares.GetString() );
}

void projTestRunLauncher::CreateVFS(){
	deVFSContainerReference container;
	decPath pathRootDir, pathDiskDir;
	
	pVFS.TakeOver( new deVirtualFileSystem );
	
	// add configuration containers
	if( ! pPathConfigSystem.IsEmpty() ){
		pathRootDir.SetFromUnix( "/config/system" );
		pathDiskDir.SetFromNative( pPathConfigSystem.GetString() );
		container.TakeOver( new deVFSDiskDirectory( pathRootDir, pathDiskDir ) );
		( ( deVFSDiskDirectory* )container.operator->() )->SetReadOnly( true );
		pVFS->AddContainer( container );
	}
	
	if( ! pPathConfigUser.IsEmpty() ){
		pathRootDir.SetFromUnix( "/config/user" );
		pathDiskDir.SetFromNative( pPathConfigUser.GetString() );
		container.TakeOver( new deVFSDiskDirectory( pathRootDir, pathDiskDir ) );
		pVFS->AddContainer( container );
	}
	
	// add data directory which is the engine share directory
	if( ! pPathShares.IsEmpty() ){
		pathRootDir.SetFromUnix( "/data" );
		pathDiskDir.SetFromNative( pPathShares.GetString() );
		container.TakeOver( new deVFSDiskDirectory( pathRootDir, pathDiskDir ) );
		pVFS->AddContainer( container );
	}
}
