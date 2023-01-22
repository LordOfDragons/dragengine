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

#include <stdio.h>
#include <string.h>

#include "devkShaderModuleManager.h"
#include "devkShaderModule.h"
#include "../devkDevice.h"
#include "../deSharedVulkan.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileReaderReference.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/filesystem/dePathList.h>
#include <dragengine/filesystem/deVFSDiskDirectory.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/filesystem/deCollectFileSearchVisitor.h>
#include <dragengine/systems/modules/deBaseModule.h>



// Class devkShaderModuleManager
//////////////////////////////////

// Constructor, destructor
////////////////////////////

devkShaderModuleManager::devkShaderModuleManager( devkDevice &device ) :
pDevice( device ){
}

devkShaderModuleManager::~devkShaderModuleManager(){
}



// Management
///////////////

int devkShaderModuleManager::GetCount() const{
	return pSources.GetCount();
}

devkShaderModule *devkShaderModuleManager::GetAt( int index ) const{
	return ( devkShaderModule* )pSources.GetAt( index );
}

devkShaderModule *devkShaderModuleManager::GetWithPath( const char *path ) const{
	const int count = pSources.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		devkShaderModule * const module = ( devkShaderModule* )pSources.GetAt( i );
		if( module->GetPath() == path ){
			return module;
		}
	}
	
	return nullptr;
}

bool devkShaderModuleManager::HasWithPath( const char *path ) const{
	const int count = pSources.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		if( ( ( devkShaderModule* )pSources.GetAt( i ) )->GetPath() == path ){
			return true;
		}
	}
	
	return false;
}

void devkShaderModuleManager::Add( devkShaderModule *module ){
	if( ! module ){
		DETHROW_INFO( deeNullPointer, "module" );
	}
	if( HasWithPath( module->GetPath() ) ){
		DETHROW_INFO( deeInvalidParam, "module with path is present" );
	}
	
	pSources.Add( module );
}

void devkShaderModuleManager::Clear(){
	pSources.RemoveAll();
}

void devkShaderModuleManager::Load( const char *directory ){
	deBaseModule &baseModule = pDevice.GetInstance().GetVulkan().GetModule();
	deVirtualFileSystem &vfs = baseModule.GetVFS();
	const int oldSourceCount = pSources.GetCount();
	decBaseFileReaderReference reader;
	devkShaderModule::Ref module;
	
	const decPath basePath( decPath::CreatePathUnix( directory ) );
	const int basePathLen = ( int )strlen( basePath.GetPathUnix() ) + 1;
	
	deCollectFileSearchVisitor collect( "*.spv", true );
	vfs.SearchFiles( basePath, collect );
	
	const dePathList &pathList = collect.GetFiles();
	const int count = pathList.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const decPath &path = pathList.GetAt( i );
		const decString filename( path.GetPathUnix().GetMiddle( basePathLen ) );
		
		try{
			reader.TakeOver( vfs.OpenFileForReading( path ) );
			module.TakeOver( new devkShaderModule( pDevice, filename, reader ) );
			Add( module );
			
		}catch( const deException & ){
			baseModule.LogInfoFormat( "Vulkan: Loading shader module failed: %s", filename.GetString() );
			throw;
		}
	}
	
	baseModule.LogInfoFormat( "Vulkan: Loaded %d shader modules.", pSources.GetCount() - oldSourceCount );
}
