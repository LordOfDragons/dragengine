/* 
 * Drag[en]gine OpenXR
 *
 * Copyright (C) 2022, Roland Plüss (roland@rptd.ch)
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

#include <stdlib.h>
#include <string.h>

#include "deoxrLoader.h"
#include "deVROpenXR.h"
#include "deoxrGlobalFunctions.h"
#include "deoxrBasics.h"

#include <openxr/loader_interfaces.h>

#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/file/decDiskFileReader.h>
#include <dragengine/filesystem/deVFSDiskDirectory.h>
#include <dragengine/systems/modules/deBaseModule.h>

#ifdef OS_W32
#include <dragengine/app/deOSWindows.h>
#endif



// class deoxrLoader
/////////////////////

deoxrLoader::deoxrLoader( deVROpenXR &oxr ) :
pOxr( oxr ),
#ifdef OS_BEOS
pLibHandle( 0 )
#else
pLibHandle( NULL )
#endif
{
	xrGetInstanceProcAddr = nullptr;
	
	try{
		// find runtime configuration file
		pFindRuntimeConfigFile();
		if( pRuntimeConfigFile.IsEmpty() ){
			DETHROW_INFO( deeInvalidAction, "runtime config file not found" );
		}
		
		// read runtime config file to get library file to load
		pReadConfig();
		
		// load oxr library
		pLoadOpenXR();
		oxr.LogInfo( "OpenXR library loaded" );
		
		// negotiate
		pNegotiate();
		oxr.LogInfo( "OpenXR library negotiated" );
		if( ! xrGetInstanceProcAddr ){
			DETHROW_INFO( deeInvalidAction, "runtime provided no xrGetInstanceProcAddr" );
		}
		
		// load functions
		pLoadFunctions();
		oxr.LogInfo( "OpenXR functions loaded" );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

deoxrLoader::~deoxrLoader(){
	pCleanUp();
}



// Management
///////////////




// Private Functions
//////////////////////

void deoxrLoader::pCleanUp(){
	if( pLibHandle ){
		#ifdef OS_BEOS
		unload_add_on( pLibHandle );
		#endif
		
		#ifdef HAS_LIB_DL
		dlclose( pLibHandle );
		#endif
		
		#ifdef OS_W32
		FreeLibrary( pLibHandle );
		#endif
	}
}

void deoxrLoader::pLoadOpenXR(){
	#ifdef OS_BEOS
	pLibHandle = load_add_on( "oxr" );
	
	if( pLibHandle < 0 ){
		DETHROW_INFO( deeInvalidAction, "Load OpenXR image failed" );
	}
	#endif
	
	#ifdef HAS_LIB_DL
	pLibHandle = dlopen( pRuntimeLibraryPath, RTLD_NOW );
	if( ! pLibHandle ){
		pOxr.LogErrorFormat( "dlerror: %s.", dlerror() );
		DETHROW_INFO( deeInvalidAction, "Load OpenXR library failed" );
	}
	#endif
	
	#ifdef OS_W32
	wchar_t widePath[ MAX_PATH ];
	deOSWindows::Utf8ToWide( pRuntimeLibraryPath, widePath, MAX_PATH );
	pLibHandle = LoadLibrary( widePath );
	
	if( ! pLibHandle ){
		int err = GetLastError();
		wchar_t messageBuffer[ 251 ];
		FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, err, MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ), // Default language
			messageBuffer, 250, NULL );
		
		pOxr.LogErrorFormat( "LoadLibrary(err=%i): %s.",
			err, deOSWindows::WideToUtf8( messageBuffer ).GetString() );
		
		DETHROW_INFO( deeInvalidAction, "Load OpenXR DLL failed" );
	}
	#endif
}

void deoxrLoader::pLoadFunctions(){
	#define GLOBAL_LEVEL_OPENXR_FUNCTION( name ) \
		if( xrGetInstanceProcAddr( XR_NULL_HANDLE, #name, ( PFN_xrVoidFunction* )&name ) != XR_SUCCESS ){ \
			DETHROW_INFO( deeInvalidAction, "Function " #name " not found" ); \
		}
	#define GLOBAL_LEVEL_OPENXR_FUNCTION_WARN( name ) \
		if( xrGetInstanceProcAddr( XR_NULL_HANDLE, #name, ( PFN_xrVoidFunction* )&name ) != XR_SUCCESS ){ \
			pOxr.LogInfoFormat( "Function " #name " not found! Broken VR Runtime!" ); \
		}
	
	#include "deoxrFunctionNames.h"
}

void deoxrLoader::pFindRuntimeConfigFile(){
	#ifdef OS_UNIX
	const decPath home( decPath::CreatePathUnix( pGetHomeDirectory() ) );
	decStringList directories;
	
	const char *envPath = getenv( "XDG_CONFIG_HOME" );
	if( envPath ){
		directories += decString( envPath ).Split( ':' );
	}
	
	envPath = getenv( "XDG_CONFIG_DIRS" );
	if( envPath ){
		directories += decString( envPath ).Split( ':' );
	}
	
	directories += "~/.config";
	directories += "/etc";
	
	const int count = directories.GetCount();
	const deVFSDiskDirectory::Ref container( deVFSDiskDirectory::Ref::New( new deVFSDiskDirectory(
		decPath::CreatePathUnix( "/" ), decPath::CreatePathUnix( "/" ), true ) ) );
	const decPath childPath( decPath::CreatePathUnix( "openxr/1/active_runtime.json" ) );
	int i;
	
	for( i=0; i<count; i++ ){
		decPath path( decPath::CreatePathUnix( directories.GetAt( i ) ) );
		if( path.GetComponentCount() > 0 && path.GetComponentAt( 0 ) == "~" ){
			path.RemoveComponentFrom( 0 );
			path = home + path;
		}
		
		path += childPath;
		if( container->CanReadFile( path ) ){
			pRuntimeConfigFile = path.GetPathUnix();
			return;
		}
	}
	
	#elif defined OS_W32
	// HKEY_LOCAL_MACHINE\SOFTWARE\Khronos\OpenXR\1
	// string value "ActiveRuntime" is path to json file
	#endif
}

void deoxrLoader::pReadConfig(){
	const decDiskFileReader::Ref reader( decDiskFileReader::Ref::New(
		new decDiskFileReader( pRuntimeConfigFile ) ) );
	const int contentLength = reader->GetLength();
	decString content;
	content.Set( ' ', contentLength );
	reader->Read( ( void* )content.GetString(), contentLength );
	
	int index = content.FindString( "\"library_path\"" );
	if( index == -1 ){
		DETHROW_INFO( deeInvalidFileFormat, "invalid config file format" );
	}
	
	index = content.Find( ':', index + 1 );
	if( index == -1 ){
		DETHROW_INFO( deeInvalidFileFormat, "invalid config file format" );
	}
	
	index = content.Find( '"', index + 1 );
	if( index == -1 ){
		DETHROW_INFO( deeInvalidFileFormat, "invalid config file format" );
	}
	
	int index2 = content.Find( '"', index + 1 );
	if( index2 == -1 ){
		DETHROW_INFO( deeInvalidFileFormat, "invalid config file format" );
	}
	
	pRuntimeLibraryPath = content.GetMiddle( index + 1, index2 );
}

void deoxrLoader::pNegotiate(){
	// get function pointer
	PFN_xrNegotiateLoaderRuntimeInterface fNegotiate = nullptr;
	
	#ifdef OS_BEOS
	if( get_image_symbol( pLibHandle, "xrNegotiateLoaderRuntimeInterface",
	B_SYMBOL_TYPE_TEXT, ( void** )&fNegotiate ) != B_OK ){
		fNegotiate = nullptr;
	}
	#endif
	
	#ifdef HAS_LIB_DL
	fNegotiate = ( PFN_xrNegotiateLoaderRuntimeInterface )dlsym( pLibHandle, "xrNegotiateLoaderRuntimeInterface" );
	#endif
	
	#ifdef OS_W32
	fNegotiate = ( PFN_xrNegotiateLoaderRuntimeInterface )GetProcAddress( pLibHandle, "xrNegotiateLoaderRuntimeInterface" );
	#endif
	
	if( ! fNegotiate ){
		DETHROW_INFO( deeInvalidAction, "Function xrNegotiateLoaderRuntimeInterface not found" );
	}
	
	// negotiate
	// https://www.khronos.org/registry/OpenXR/specs/1.0/loader.html#loader-runtime-interface-negotiation
	XrNegotiateLoaderInfo loaderInfo;
	memset( &loaderInfo, 0, sizeof( loaderInfo ) );
	loaderInfo.structType = XR_LOADER_INTERFACE_STRUCT_LOADER_INFO;
	loaderInfo.structVersion = XR_LOADER_INFO_STRUCT_VERSION;
	loaderInfo.structSize = sizeof( loaderInfo );
	loaderInfo.minInterfaceVersion = 1;
	loaderInfo.maxInterfaceVersion = XR_CURRENT_LOADER_API_LAYER_VERSION;
	loaderInfo.minApiVersion = XR_MAKE_VERSION( 1, 0, 0 );
	loaderInfo.maxApiVersion = XR_MAKE_VERSION( 1, 0, 0 );
	
	XrNegotiateRuntimeRequest runtimeRequest;
	memset( &runtimeRequest, 0, sizeof( runtimeRequest ) );
	runtimeRequest.structType = XR_LOADER_INTERFACE_STRUCT_RUNTIME_REQUEST;
	runtimeRequest.structVersion = XR_RUNTIME_INFO_STRUCT_VERSION;
	runtimeRequest.structSize = sizeof( runtimeRequest );
	
	OXR_CHECK( pOxr, fNegotiate( &loaderInfo, &runtimeRequest ) );
	xrGetInstanceProcAddr = runtimeRequest.getInstanceProcAddr;
}

#ifdef OS_UNIX
decString deoxrLoader::pGetHomeDirectory() const{
	const char *envPath = getenv( "HOME" );
	if( envPath ){
		return decString( envPath );
	}
	
	envPath = getenv( "USER" );
	if( envPath ){
		return decString( "/home/" ) + envPath;
	}
	
	envPath = getenv( "LOGUSER" );
	if( envPath ){
		return decString( "/home/" ) + envPath;
	}
	
	DETHROW_INFO( deeInvalidParam, "home directory not found" );
}
#endif