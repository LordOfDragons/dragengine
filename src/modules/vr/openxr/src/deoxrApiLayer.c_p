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

#include "deoxrApiLayer.h"
#include "deVROpenXR.h"
#include "deoxrBasics.h"

#include <openxr/loader_interfaces.h>

#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/file/decDiskFileReader.h>
#include <dragengine/common/utils/decTimer.h>
#include <dragengine/filesystem/deVFSDiskDirectory.h>
#include <dragengine/systems/modules/deBaseModule.h>
#include <dragengine/threading/deThread.h>

#ifdef OS_W32
#include <dragengine/app/deOSWindows.h>
#endif



// class deoxrApiLayer
/////////////////////

deoxrApiLayer::deoxrApiLayer( deVROpenXR &oxr, const char *configFile ) :
pOxr( oxr ),
pConfigFile( configFile ),
#ifdef OS_BEOS
pLibHandle( 0 )
#else
pLibHandle( NULL )
#endif
{
	try{
		pReadConfig();
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

deoxrApiLayer::~deoxrApiLayer(){
	pCleanUp();
}



// Management
///////////////

bool deoxrApiLayer::IsLoaded() const{
	return pLibHandle;
}

void deoxrApiLayer::LoadLibrary(){
	if( pLibHandle ){
		return;
	}
	
	try{
		pLoadLibrary();
		pOxr.LogInfoFormat( "API layer library loaded: %s", pName.GetString() );
		
		pNegotiate();
		pOxr.LogInfoFormat( "API library negotiated: %s", pName.GetString() );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

void deoxrApiLayer::UnloadLibrary(){
	if( ! pLibHandle ){
		return;
	}
	
	pOxr.LogInfoFormat( "Unload API layer library: %s", pLibraryPath.GetString() );
	#ifdef OS_BEOS
	unload_add_on( pLibHandle );
	#endif
	
	#ifdef HAS_LIB_DL
	dlclose( pLibHandle );
	#endif
	
	#ifdef OS_W32
	FreeLibrary( pLibHandle );
	#endif
	
	pLibHandle = nullptr;
}



// Private Functions
//////////////////////

void deoxrApiLayer::pCleanUp(){
	UnloadLibrary();
}

void deoxrApiLayer::pLoadLibrary(){
	pOxr.LogInfoFormat( "Load API layer library: %s", pLibraryPath.GetString() );
	#ifdef OS_BEOS
	pLibHandle = load_add_on( pLibraryPath );
	
	if( pLibHandle < 0 ){
		DETHROW_INFO( deeInvalidAction, "Load image failed" );
	}
	#endif
	
	#ifdef HAS_LIB_DL
	pLibHandle = dlopen( pLibraryPath, RTLD_NOW );
	if( ! pLibHandle ){
		pOxr.LogErrorFormat( "dlerror: %s.", dlerror() );
		DETHROW_INFO( deeInvalidAction, "Load library failed" );
	}
	#endif
	
	#ifdef OS_W32
	wchar_t widePath[ MAX_PATH ];
	deOSWindows::Utf8ToWide( pLibraryPath, widePath, MAX_PATH );
	pLibHandle = ::LoadLibrary( widePath );
	
	if( ! pLibHandle ){
		int err = GetLastError();
		wchar_t messageBuffer[ 251 ];
		FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, err, MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ), // Default language
			messageBuffer, 250, NULL );
		
		pOxr.LogErrorFormat( "LoadLibrary(err=%i): %s.",
			err, deOSWindows::WideToUtf8( messageBuffer ).GetString() );
		
		DETHROW_INFO( deeInvalidAction, "Load DLL failed" );
	}
	#endif
}

void deoxrApiLayer::pReadConfig(){
	const decDiskFileReader::Ref reader( decDiskFileReader::Ref::New( new decDiskFileReader( pConfigFile ) ) );
	const int contentLength = reader->GetLength();
	decString content;
	content.Set( ' ', contentLength );
	reader->Read( ( void* )content.GetString(), contentLength );
	
	// name
	int index = content.FindString( "\"name\"" );
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
	
	pName = content.GetMiddle( index + 1, index2 );
	
	// library
	index = content.FindString( "\"library_path\"" );
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
	
	index2 = content.Find( '"', index + 1 );
	if( index2 == -1 ){
		DETHROW_INFO( deeInvalidFileFormat, "invalid config file format" );
	}
	
	// library file can be absolute or relative
	decPath configDir( decPath::CreatePathNative( pConfigFile ) );
	configDir.RemoveLastComponent();
	
	pLibraryPath = decPath::CreatePathNative( content.GetMiddle( index + 1, index2 ) ).
		AbsolutePath( configDir ).GetPathNative();
}

void deoxrApiLayer::pNegotiate(){
	// get function pointer
	PFN_xrNegotiateLoaderApiLayerInterface fNegotiate = nullptr;
	
	#ifdef OS_BEOS
	if( get_image_symbol( pLibHandle, "xrNegotiateLoaderApiLayerInterface",
	B_SYMBOL_TYPE_TEXT, ( void** )&fNegotiate ) != B_OK ){
		fNegotiate = nullptr;
	}
	#endif
	
	#ifdef HAS_LIB_DL
	fNegotiate = ( PFN_xrNegotiateLoaderApiLayerInterface )dlsym( pLibHandle, "xrNegotiateLoaderApiLayerInterface" );
	#endif
	
	#ifdef OS_W32
	fNegotiate = ( PFN_xrNegotiateLoaderApiLayerInterface )GetProcAddress( pLibHandle, "xrNegotiateLoaderApiLayerInterface" );
	#endif
	
	if( ! fNegotiate ){
		DETHROW_INFO( deeInvalidAction, "Function xrNegotiateLoaderApiLayerInterface not found" );
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
	loaderInfo.maxApiVersion = XR_MAKE_VERSION( 1, 0x3ff, 0xfff );  // Maximum allowed version for this major version.
	
	XrNegotiateApiLayerRequest layerRequest;
	memset( &layerRequest, 0, sizeof( layerRequest ) );
	layerRequest.structType = XR_LOADER_INTERFACE_STRUCT_API_LAYER_REQUEST;
	layerRequest.structVersion = XR_API_LAYER_INFO_STRUCT_VERSION;
	layerRequest.structSize = sizeof( layerRequest );
	
	OXR_CHECK( fNegotiate( &loaderInfo, pName, &layerRequest ) );
	if( ! layerRequest.getInstanceProcAddr ){
		DETHROW_INFO( deeNullPointer, "negotiate.getInstanceProcAddr" );
	}
}
