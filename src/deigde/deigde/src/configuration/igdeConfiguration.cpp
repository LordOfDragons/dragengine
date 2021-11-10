/* 
 * Drag[en]gine IGDE
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
#include <sys/types.h>
#include <unistd.h>

#ifdef OS_UNIX
#	include <pwd.h>
#endif

#ifdef OS_W32
#	define _WIN32_IE 0x0500
#	include <windows.h>
#	include <shlobj.h>
#endif

#include "igdeConfiguration.h"
#include "igdeConfigurationXML.h"
#include "../gui/igdeWindowMain.h"
#include "../../shared/src/codec/igdeCodecPropertyString.h"

#include <deigde/engine/igdeEngineController.h>

#include <dragengine/deEngine.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/filesystem/deVFSDiskDirectory.h>
#include <dragengine/common/file/decBaseFileReaderReference.h>
#include <dragengine/common/file/decBaseFileWriterReference.h>
#include <dragengine/common/file/decDiskFileReader.h>
#include <dragengine/common/file/decDiskFileWriter.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>
#include <dragengine/common/xmlparser/decXmlParser.h>
#include <dragengine/common/xmlparser/decXmlDocument.h>
#include <dragengine/common/xmlparser/decXmlCharacterData.h>
#include <dragengine/common/xmlparser/decXmlElementTag.h>
#include <dragengine/common/xmlparser/decXmlAttValue.h>
#include <dragengine/common/xmlparser/decXmlVisitor.h>
#include <dragengine/common/exceptions.h>
#ifdef OS_W32
#include <dragengine/app/deOSWindows.h>
#endif



// Definitions
////////////////

#define FILE_IGDE_CONFIG_SYSTEM		"/config/system/deigde.xml"
#define FILE_IGDE_CONFIG_USER		"/config/user/deigde.xml"

#define LOGSOURCE "IGDE"



// Class igdeConfiguration
////////////////////////////

// Constructor, destructor
////////////////////////////

//#define TEST_SPECIAL
#ifdef TEST_SPECIAL
#include <dragengine/common/string/decStringList.h>
#include <dragengine/common/shape/decShapeCapsule.h>
#include <dragengine/common/shape/decShapeCylinder.h>
#include <dragengine/common/shape/decShapeBox.h>
#include <dragengine/common/shape/decShapeList.h>
#include <dragengine/common/shape/decShapeSphere.h>
#endif

igdeConfiguration::igdeConfiguration( igdeWindowMain &windowMain ) :
pWindowMain( windowMain )
{
#ifdef TEST_SPECIAL
	// DEBUG
	igdeCodecPropertyString codec;
	decString string1, string2;
	
	decVector vector;
	decDVector dvector;
	codec.DecodeVector( "4.5 7 1.223e-2", vector );
	codec.DecodeDVector( "4.5 7 1.223e-2", dvector );
	printf( "decode vector=(%g,%g,%g) dvector=(%g,%g,%g)\n", vector.x, vector.y, vector.z, dvector.x, dvector.y, dvector.z );
	codec.EncodeVector( vector, string1 );
	codec.EncodeDVector( dvector, string2 );
	printf( "encode vector='%s' dvector='%s'\n", string1.GetString(), string2.GetString() );
	
	decVector2 vector2;
	codec.DecodeVector2( "4.2 76", vector2 );
	printf( "decode vector2=(%g,%g)\n", vector2.x, vector2.y );
	codec.EncodeVector2( vector2, string1 );
	printf( "encode vector2='%s'\n", string1.GetString() );
	
	decVector4 vector4;
	decDVector4 dvector4;
	codec.DecodeVector4( "8.4 4 11.2 0.4e-2", vector4 );
	codec.DecodeDVector4( "8.4 4 11.2 0.4e-2", dvector4 );
	printf( "decode vector4=(%g,%g,%g,%g) dvector4=(%g,%g,%g,%g)\n", vector4.x, vector4.y, vector4.z, vector4.w, dvector4.x, dvector4.y, dvector4.z, dvector4.w );
	codec.EncodeVector4( vector4, string1 );
	codec.EncodeDVector4( dvector4, string2 );
	printf( "encode vector4='%s' dvector4='%s'\n", string1.GetString(), string2.GetString() );
	
	decColor color3, color4;
	codec.DecodeColor3( "0.1 1 0.6", color3 );
	codec.DecodeColor4( "0.1 1 0.6 0.2", color4 );
	printf( "decode color3=(%g,%g,%g) color4=(%g,%g,%g,%g)\n", color3.r, color3.g, color3.b, color4.r, color4.g, color4.b, color4.a );
	codec.EncodeColor3( color3, string1 );
	codec.EncodeColor4( color4, string2 );
	printf( "encode color3='%s' color4='%s'\n", string1.GetString(), string2.GetString() );
	
	
	
	decShapeList shapeList1, shapeList2;
	shapeList1.AddShape( new decShapeSphere( 0.5f, decVector( 2.0f, 6.5f, -3.2f ) ) );
	codec.EncodeShapeList( shapeList1, string1 );
	printf( "encode sphere shape='%s'\n", string1.GetString() );
	codec.DecodeShapeList( string1.GetString(), shapeList2 );
	codec.EncodeShapeList( shapeList2, string2 );
	printf( "decode sphere shape='%s'\n", string2.GetString() );
	
	shapeList1.RemoveAllShapes();
	shapeList1.AddShape( new decShapeBox( decVector( 1.5f, 2.0f, 0.5f ), decVector( 2.0f, -1.0f, 0.2f ) ) );
	codec.EncodeShapeList( shapeList1, string1 );
	printf( "encode box shape='%s'\n", string1.GetString() );
	codec.DecodeShapeList( string1.GetString(), shapeList2 );
	codec.EncodeShapeList( shapeList2, string2 );
	printf( "decode box shape='%s'\n", string2.GetString() );
	
	shapeList1.RemoveAllShapes();
	shapeList1.AddShape( new decShapeBox( decVector( 1.5f, 2.0f, 0.5f ), decVector( 2.0f, -1.0f, 0.2f ),
		decMatrix::CreateRotation( 10.0f * DEG2RAD, 120.0f * DEG2RAD, -90.0f * DEG2RAD ).ToQuaternion() ) );
	codec.EncodeShapeList( shapeList1, string1 );
	printf( "encode box shape='%s'\n", string1.GetString() );
	codec.DecodeShapeList( string1.GetString(), shapeList2 );
	codec.EncodeShapeList( shapeList2, string2 );
	printf( "decode box shape='%s'\n", string2.GetString() );
	
	shapeList1.RemoveAllShapes();
	shapeList1.AddShape( new decShapeCylinder( 3.5f, 0.5f, 1.0f, decVector( 2.0f, -1.0f, 0.2f ) ) );
	codec.EncodeShapeList( shapeList1, string1 );
	printf( "encode cylinder shape='%s'\n", string1.GetString() );
	codec.DecodeShapeList( string1.GetString(), shapeList2 );
	codec.EncodeShapeList( shapeList2, string2 );
	printf( "decode cylinder shape='%s'\n", string2.GetString() );
	
	shapeList1.RemoveAllShapes();
	shapeList1.AddShape( new decShapeCylinder( 3.5f, 0.5f, 1.0f, decVector( 2.0f, -1.0f, 0.2f ),
		decMatrix::CreateRotation( 10.0f * DEG2RAD, 120.0f * DEG2RAD, -90.0f * DEG2RAD ).ToQuaternion() ) );
	codec.EncodeShapeList( shapeList1, string1 );
	printf( "encode cylinder shape='%s'\n", string1.GetString() );
	codec.DecodeShapeList( string1.GetString(), shapeList2 );
	codec.EncodeShapeList( shapeList2, string2 );
	printf( "decode cylinder shape='%s'\n", string2.GetString() );
	
	shapeList1.RemoveAllShapes();
	shapeList1.AddShape( new decShapeCapsule( 3.5f, 0.5f, 1.0f, decVector( 2.0f, -1.0f, 0.2f ) ) );
	codec.EncodeShapeList( shapeList1, string1 );
	printf( "encode capsule shape='%s'\n", string1.GetString() );
	codec.DecodeShapeList( string1.GetString(), shapeList2 );
	codec.EncodeShapeList( shapeList2, string2 );
	printf( "decode capsule shape='%s'\n", string2.GetString() );
	
	shapeList1.RemoveAllShapes();
	shapeList1.AddShape( new decShapeCapsule( 3.5f, 0.5f, 1.0f, decVector( 2.0f, -1.0f, 0.2f ),
		decMatrix::CreateRotation( 10.0f * DEG2RAD, 120.0f * DEG2RAD, -90.0f * DEG2RAD ).ToQuaternion() ) );
	codec.EncodeShapeList( shapeList1, string1 );
	printf( "encode capsule shape='%s'\n", string1.GetString() );
	codec.DecodeShapeList( string1.GetString(), shapeList2 );
	codec.EncodeShapeList( shapeList2, string2 );
	printf( "decode capsule shape='%s'\n", string2.GetString() );
	
	shapeList1.RemoveAllShapes();
	shapeList1.AddShape( new decShapeSphere( 0.5f, decVector( 2.0f, 6.5f, -3.2f ) ) );
	shapeList1.AddShape( new decShapeBox( decVector( 1.5f, 2.0f, 0.5f ), decVector( 2.0f, -1.0f, 0.2f ) ) );
	shapeList1.AddShape( new decShapeCylinder( 3.5f, 0.5f, 1.0f, decVector( 2.0f, -1.0f, 0.2f ) ) );
	shapeList1.AddShape( new decShapeCapsule( 3.5f, 0.5f, 1.0f, decVector( 2.0f, -1.0f, 0.2f ) ) );
	codec.EncodeShapeList( shapeList1, string1 );
	printf( "encode shape list='%s'\n", string1.GetString() );
	codec.DecodeShapeList( string1.GetString(), shapeList2 );
	codec.EncodeShapeList( shapeList2, string2 );
	printf( "decode shape list='%s'\n", string2.GetString() );
	// DEBUG
#endif
	
	pMaxRecentProjectEntries = 10;
	
	try{
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

igdeConfiguration::~igdeConfiguration(){
	pCleanUp();
}



// Management
///////////////

void igdeConfiguration::SetPathConfigSystem( const char *path ){
	pPathConfigSystem = path;
}

void igdeConfiguration::SetPathConfigUser( const char *path ){
	pPathConfigUser = path;
}

void igdeConfiguration::SetPathShares( const char *path ){
	pPathShares = path;
}

void igdeConfiguration::SetPathLib( const char *path ){
	pPathLib = path;
}

void igdeConfiguration::SetPathLogs( const char *path ){
	pPathLogs = path;
}

void igdeConfiguration::SetPathProjects( const char *path ){
	pPathProjects = path;
}

void igdeConfiguration::SetPathIGDEData( const char *path ){
	pPathIGDEData = path;
}

void igdeConfiguration::SetPathIGDETemplates( const char *path ){
	pPathIGDETemplates = path;
}

void igdeConfiguration::SetPathIGDEGameDefs( const char *path ){
	pPathIGDEGameDefs = path;
}

void igdeConfiguration::SetPathIGDEEditorData( const char *path ){
	pPathIGDEEditorData = path;
}



void igdeConfiguration::SetMaxRecentProjectEntries( int entries ){
	if( entries < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	pMaxRecentProjectEntries = entries;
	
	while( pRecentProjectList.GetCount() > pMaxRecentProjectEntries ){
		pRecentProjectList.RemoveFrom( pRecentProjectList.GetCount() - 1 );
	}
}



void igdeConfiguration::LocatePath(){
	const char *value;
	decPath pathHome;
	decPath path;
	
#ifdef OS_W32
	decString pathIgde = deOSWindows::GetRegistryValue( "SOFTWARE\\Drag[en]gine", "PathIgde", IGDE_PATH );
	value = getenv( "DEIGDE_PATH" );
	if( value ){
		pathIgde = value;
	}
	pathIgde = deOSWindows::ParseNativePath( pathIgde );
#endif
	
	// the system wide configuration directory is hard coded at compile time.
	// can be changed at runtime using an environment parameter.
	pPathConfigSystem = IGDE_CONFIG_PATH;
#ifdef OS_W32
	//pPathConfigSystem = deOSWindows::GetRegistryValue( "SOFTWARE\\Drag[en]gine",
	//	"PathIgdeConfig", pPathConfigSystem );
	pPathConfigSystem = pathIgde + "\\Config";
#endif
	
	value = getenv( "DEIGDE_SYS_CONFIG" );
	if( value ){
		pPathConfigSystem = value;
	}
#ifdef OS_W32
	pPathConfigSystem = deOSWindows::ParseNativePath( pPathConfigSystem );
#endif
	
	// retrieves the home directory
#ifdef OS_W32
	pathHome.SetFromNative( deOSWindows::ParseNativePath( "@RoamingAppData" ) );
	
#else
	value = getenv( "HOME" );
	if( value ){
		pathHome.SetFromNative( value );
		
	}else{
		value = getenv( "USER" );
		
		if( value ){
			pathHome.SetFromNative( "/home" );
			pathHome.AddComponent( value );
			
		}else{
			value = getenv( "LOGUSER" );
			
			if( value ){
				pathHome.SetFromNative( "/home" );
				pathHome.AddComponent( value );
			}
		}
	}
#endif
	
#ifdef OS_W32
	pPathConfigUser = "@RoamingAppData\\DEIGDE\\Config";
	pPathProjects = deOSWindows::ParseNativePath( "@Documents\\DEGameProjects" );
#else
	if( pathHome.GetComponentCount() > 0 ){ // ~/.config/deigde
		// the user configuration directory is located under the user home directory.
		// can be changed at runtime using an environment parameter.
		path.SetFrom( pathHome );
		path.AddComponent( ".config" );
		path.AddComponent( "deigde" );
		pPathConfigUser = path.GetPathNative();
		
		// the projects path is also under the user directory
		path.SetFrom( pathHome );
		path.AddComponent( "deprojects" );
		pPathProjects = path.GetPathNative();
	}
	
#endif
	
	value = getenv( "DEIGDE_USER_CONFIG" );
	if( value ){
		pPathConfigUser = value;
	}
#ifdef OS_W32
	pPathConfigUser = deOSWindows::ParseNativePath( pPathConfigUser );
#endif
	
	value = getenv( "DEIGDE_PROJECTS" );
	if( value ){
		pPathProjects = value;
	}
#ifdef OS_W32
	pPathProjects = deOSWindows::ParseNativePath( pPathProjects );
#endif
	
	// the shares directory is hard coded at compile time. can be changed at runtime
	// using an environment parameter.
	pPathShares = IGDE_SHARE_PATH;
#ifdef OS_W32
	pPathShares = pathIgde + "\\Share";
#endif
	
	value = getenv( "DEIGDE_SHARES" );
	if( value ){
		pPathShares = value;
	}
#ifdef OS_W32
	pPathShares = deOSWindows::ParseNativePath( pPathShares );
#endif
	
	// the igde shared data path is relative to the shares path
	path.SetFromNative( pPathShares );
	path.AddComponent( "data" );
	pPathIGDEData = path.GetPathNative();
	
	// the igde project templates path is relative to the shares path
	path.SetFromNative( pPathShares );
	path.AddComponent( "templates" );
	pPathIGDETemplates = path.GetPathNative();
	
	// the igde shared game definitions path is relative to the shares path
	path.SetFromNative( pPathShares );
	path.AddComponent( "gamedefs" );
	pPathIGDEGameDefs = path.GetPathNative();
	
	// the igde editors shared data path is relative to the shares path
	path.SetFromNative( pPathShares );
	path.AddComponent( "modules" );
	pPathIGDEEditorData = path.GetPathNative();
	
	// the library directory is hard coded at compile time. can be changed at runtime
	// using an environment parameter.
	pPathLib = IGDE_LIB_PATH;
#ifdef OS_W32
	pPathLib = pathIgde + "\\Data";
#endif
	
	value = getenv( "DEIGDE_LIB" );
	if( value ){
		pPathLib = value;
	}
#ifdef OS_W32
	pPathLib = deOSWindows::ParseNativePath( pPathLib );
#endif
	
	// the logs directory is located right under the user configuration directory.
	// can be changed at runtime using an environment parameter
	path.SetFromNative( pPathConfigUser );
	path.AddComponent( "logs" );
	pPathLogs = path.GetPathNative();
#ifdef OS_W32
	pPathLogs = "@LocalAppData\\DEIGDE\\Logs";
#endif
	
	value = getenv( "DEIGDE_LOGS" );
	if( value ){
		pPathLogs = value;
	}
	#ifdef OS_W32
	pPathLogs = deOSWindows::ParseNativePath( pPathLogs );
	#endif
}

void igdeConfiguration::InitVirtualFileSystem(){
	deVirtualFileSystem &vfs = *pWindowMain.GetVirtualFileSystem();
	deVFSDiskDirectory *diskDir = NULL;
	decPath pathRootDir, pathDiskDir;
	
	// add the found path to the virtual file system. this makes it easier
	// to find the files later on without having to deal with file system
	// specific quirks.
	try{
		// add the configuration containers. the containers are added in
		// separate locations as we want to read the config files one
		// by one and mapping both containers to the same path would
		// shadow the system config files.
		if( ! pPathConfigSystem.IsEmpty() ){
			pathRootDir.SetFromUnix( "/config/system" );
			pathDiskDir.SetFromNative( pPathConfigSystem.GetString() );
			diskDir = new deVFSDiskDirectory( pathRootDir, pathDiskDir );
			if( ! diskDir ) DETHROW( deeOutOfMemory );
			diskDir->SetReadOnly( true );
			vfs.AddContainer( diskDir );
			diskDir->FreeReference();
			diskDir = NULL;
		}
		
		if( ! pPathConfigUser.IsEmpty() ){
			pathRootDir.SetFromUnix( "/config/user" );
			pathDiskDir.SetFromNative( pPathConfigUser.GetString() );
			diskDir = new deVFSDiskDirectory( pathRootDir, pathDiskDir );
			if( ! diskDir ) DETHROW( deeOutOfMemory );
			diskDir->SetReadOnly( false );
			vfs.AddContainer( diskDir );
			diskDir->FreeReference();
			diskDir = NULL;
		}
		
		// add the data directory. currently there exists only one which
		// is the system shares directory. a user one could be layered
		// on top of it though if required later on. the shares container
		// is set to read-write as the launcher has to potentiall install
		// new games or uninstall them.
		if( ! pPathShares.IsEmpty() ){
			pathRootDir.SetFromUnix( "/data" );
			pathDiskDir.SetFromNative( pPathShares.GetString() );
			diskDir = new deVFSDiskDirectory( pathRootDir, pathDiskDir );
			if( ! diskDir ) DETHROW( deeOutOfMemory );
			diskDir->SetReadOnly( false );
			vfs.AddContainer( diskDir );
			diskDir->FreeReference();
			diskDir = NULL;
		}
		
		// add the logs directory. this is read-write
		if( ! pPathLogs.IsEmpty() ){
			pathRootDir.SetFromUnix( "/logs" );
			pathDiskDir.SetFromNative( pPathLogs.GetString() );
			diskDir = new deVFSDiskDirectory( pathRootDir, pathDiskDir );
			if( ! diskDir ) DETHROW( deeOutOfMemory );
			diskDir->SetReadOnly( false );
			vfs.AddContainer( diskDir );
			diskDir->FreeReference();
			diskDir = NULL;
		}
		
	}catch( const deException & ){
		if( diskDir ){
			diskDir->FreeReference();
		}
		throw;
	}
}

void igdeConfiguration::LogImportantValues(){
	deLogger &logger = *pWindowMain.GetLogger();
	
	logger.LogInfoFormat( LOGSOURCE, "System config path = '%s'", pPathConfigSystem.GetString() );
	logger.LogInfoFormat( LOGSOURCE, "User config path = '%s'", pPathConfigUser.GetString() );
	logger.LogInfoFormat( LOGSOURCE, "Shares path = '%s'", pPathShares.GetString() );
	logger.LogInfoFormat( LOGSOURCE, "Lib path = '%s'", pPathLib.GetString() );
	logger.LogInfoFormat( LOGSOURCE, "Logs path = '%s'", pPathLogs.GetString() );
}



void igdeConfiguration::LoadConfiguration(){
	igdeConfigurationXML configXML( pWindowMain.GetLogger(), LOGSOURCE );
	deVirtualFileSystem &vfs = *pWindowMain.GetVirtualFileSystem();
	deLogger &logger = *pWindowMain.GetLogger();
	decBaseFileReader *reader = NULL;
	decPath pathFile;
	
	// read the system wide config file if existing
	pathFile.SetFromUnix( FILE_IGDE_CONFIG_SYSTEM );
	
	if( vfs.ExistsFile( pathFile ) ){
		if( vfs.GetFileType( pathFile ) == deVFSContainer::eftRegularFile ){
			 logger.LogInfo( LOGSOURCE, "Reading system configuration file" );
			try{
				reader = vfs.OpenFileForReading( pathFile );
				configXML.ReadFromFile( *reader, *this );
				
				reader->FreeReference();
				
			}catch( const deException & ){
				if( reader ){
					reader->FreeReference();
				}
				throw;
			}
			
		}else{
			logger.LogError( LOGSOURCE, "System configuration file is not a regular file" );
			DETHROW( deeInvalidParam );
		}
		
	}else{
		logger.LogInfo( LOGSOURCE, "System configuration file not found, skipped" );
	}
	
	// read the user config file if existing
	pathFile.SetFromUnix( FILE_IGDE_CONFIG_USER );
	
	if( vfs.ExistsFile( pathFile ) ){
		if( vfs.GetFileType( pathFile ) == deVFSContainer::eftRegularFile ){
			logger.LogInfo( LOGSOURCE, "Reading user configuration file" );
			reader = NULL;
			
			try{
				reader = vfs.OpenFileForReading( pathFile );
				configXML.ReadFromFile( *reader, *this );
				
				reader->FreeReference();
				
			}catch( const deException & ){
				if( reader ){
					reader->FreeReference();
				}
				throw;
			}
			
		}else{
			logger.LogError( LOGSOURCE, "User configuration file is not a regular file" );
			DETHROW( deeInvalidParam );
		}
		
	}else{
		logger.LogInfo( LOGSOURCE, "User configuration file not found, will be created upon exiting" );
	}
}

void igdeConfiguration::SaveConfiguration(){
	igdeConfigurationXML configXML( pWindowMain.GetLogger(), LOGSOURCE );
	deVirtualFileSystem &vfs = *pWindowMain.GetVirtualFileSystem();
	deLogger &logger = *pWindowMain.GetLogger();
	decBaseFileWriter *writer = NULL;
	decPath pathFile;
	
	pathFile.SetFromUnix( FILE_IGDE_CONFIG_USER );
	if( vfs.CanWriteFile( pathFile ) ){
		logger.LogInfo( LOGSOURCE, "Writing user configuration file" );
		
		try{
			writer = vfs.OpenFileForWriting( pathFile );
			configXML.WriteToFile( *writer, *this );
			
			writer->FreeReference();
			
		}catch( const deException & ){
			if( writer ){
				writer->FreeReference();
			}
			logger.LogError( LOGSOURCE, "Failed to write user configuration file (file permission problem)" );
			// DIALOG BOX
			// "User configuration can not be written!\n"
			// "Make sure you have write permission for the file and parent directory.\n"
			// "User configuration has not been saved!\n"
		}
		
	}else{
		logger.LogError( LOGSOURCE, "Failed to write user configuration file (file writing problem)" );
		// DIALOG BOX
		// "User configuration can not be written!\n"
		// "Make sure you have write permission for the file and parent directory.\n"
		// "User configuration has not been saved!\n"
	}
}



// Private Functions
//////////////////////

void igdeConfiguration::pCleanUp(){
}
