/* 
 * Drag[en]gine Shared Vulkan
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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

#include "devkLoader.h"
#include "deSharedVulkan.h"
#include "devkGlobalFunctions.h"
#include "devkBasics.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/systems/modules/deBaseModule.h>

#ifdef OS_W32
#include <dragengine/app/deOSWindows.h>
#endif



// class devkLoader
/////////////////////

devkLoader::devkLoader( deSharedVulkan &vulkan ) :
pVulkan( vulkan ),
#ifdef OS_BEOS
pLibHandle( 0 )
#else
pLibHandle( NULL )
#endif
{
	try{
		// load vulkan library
		pLoadVulkan();
		vulkan.GetModule().LogInfo( "Vulkan library loaded" );
		
		// get function pointer query function
		#ifdef OS_BEOS
		if( get_image_symbol( pLibHandle, "vkGetInstanceProcAddr", B_SYMBOL_TYPE_TEXT, ( void** )&vkGetInstanceProcAddr ) != B_OK ){
			vkGetInstanceProcAddr = NULL;
		}
		if( get_image_symbol( pLibHandle, "vkGetDeviceProcAddr", B_SYMBOL_TYPE_TEXT, ( void** )&vkGetDeviceProcAddr ) != B_OK ){
			vkGetDeviceProcAddr = NULL;
		}
		#endif
		
		#ifdef HAS_LIB_DL
		vkGetInstanceProcAddr = ( PFN_vkGetInstanceProcAddr )dlsym( pLibHandle, "vkGetInstanceProcAddr" );
		vkGetDeviceProcAddr = ( PFN_vkGetDeviceProcAddr )dlsym( pLibHandle, "vkGetDeviceProcAddr" );
		#endif
		
		#ifdef OS_W32
		vkGetInstanceProcAddr = ( PFN_vkGetInstanceProcAddr )GetProcAddress( pLibHandle, "vkGetInstanceProcAddr" );
		vkGetDeviceProcAddr = ( PFN_vkGetDeviceProcAddr )GetProcAddress( pLibHandle, "vkGetDeviceProcAddr" );
		#endif
		
		if( ! vkGetInstanceProcAddr ){
			DETHROW_INFO( deeInvalidAction, "Function vkGetInstanceProcAddr not found" );
		}
		if( ! vkGetDeviceProcAddr ){
			DETHROW_INFO( deeInvalidAction, "Function vkGetDeviceProcAddr not found" );
		}
		
		// load functions
		pLoadFunctions();
		vulkan.GetModule().LogInfo( "Vulkan functions loaded" );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

devkLoader::~devkLoader(){
	pCleanUp();
}



// Management
///////////////




// Private Functions
//////////////////////

void devkLoader::pCleanUp(){
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

void devkLoader::pLoadVulkan(){
	#ifdef OS_BEOS
	pLibHandle = load_add_on( "vulkan" );
	
	if( pLibHandle < 0 ){
		DETHROW_INFO( deeInvalidAction, "Load Vulkan image failed" );
	}
	#endif
	
	#ifdef HAS_LIB_DL
	pLibHandle = dlopen( "libvulkan.so.1", RTLD_NOW );
	if( ! pLibHandle ){
		pVulkan.GetModule().LogErrorFormat( "dlerror: %s.", dlerror() );
		DETHROW_INFO( deeInvalidAction, "Load Vulkan library failed" );
	}
	#endif
	
	#ifdef OS_W32
	wchar_t widePath[ MAX_PATH ];
	deOSWindows::Utf8ToWide( "vulkan-1.dll", widePath, MAX_PATH );
	pLibHandle = LoadLibrary( widePath );
	
	if( ! pLibHandle ){
		int err = GetLastError();
		LPVOID lpMsgBuf;
		FormatMessage( 
			FORMAT_MESSAGE_ALLOCATE_BUFFER | 
			FORMAT_MESSAGE_FROM_SYSTEM | 
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			err,
			MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ), // Default language
			( LPTSTR ) &lpMsgBuf,
			0,
			NULL 
		);
		
		// Display the string.
		pVulkan.GetModule().LogErrorFormat( "LoadLibrary(err=%i): %s.", err, ( char* )lpMsgBuf );
		
		// Free the buffer.
		LocalFree( lpMsgBuf );
		
		DETHROW_INFO( deeInvalidAction, "Load Vulkan DLL failed" );
	}
	#endif
}

void devkLoader::pLoadFunctions(){
	#define GLOBAL_LEVEL_VULKAN_FUNCTION( name ) \
		name = ( PFN_##name )vkGetInstanceProcAddr( NULL, #name ); \
		if( ! name ){ \
			DETHROW_INFO( deeInvalidAction, "Function " #name " not found" ); \
		}
	
	#include "devkFunctionNames.h"
}
