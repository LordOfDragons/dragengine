/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2023, Roland Plüss (roland@rptd.ch)
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

#include "deoglCapCheckPersistCopyData.h"
#include "../deoglCapabilities.h"
#include "../../extensions/deoglExtensions.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../renderthread/deoglRTLogger.h"

#include <dragengine/common/exceptions.h>



// Class deoglCapCheckPersistCopyData
///////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglCapCheckPersistCopyData::deoglCapCheckPersistCopyData(
deoglCapabilities &capabilities ) :
pCapabilities( capabilities ),
pWorking( false ){
}



// Management
///////////////

void deoglCapCheckPersistCopyData::Check(){
	// Summary:
	//    Persistent mapped buffers allow to keep an SSBO persistently mapped in the CPU
	//    address space while still allowing glCopy*BufferSubData to be called on it.
	//    Some broken drivers incorrectly throw an error instead.
	//    
	// Background:
	//    In common OpenGL buffers are not allowed to be mapped while their data is modified
	//    for example using glCopy*BufferSubData. Persistent mapped buffers levitate this
	//    limitation if the following is true:
	//    - the buffer is created using immutable storage
	//    - the buffer has been created using the GL_MAP_PERSISTENT_BIT bit set
	//    - the buffer is mapped with the GL_MAP_PERSISTENT_BIT bit set
	//    In this situation calling glCopy*BufferSubData is valid and the user is responsible
	//    to synchronize memory access himself.
	//    
	//    nVidia driver on windows 10 is though broken. Even though the conditions are
	//    fulfilled the driver raises an GL_INVALID_VALUE error on calling glCopy*BufferSubData
	//    on the mapped buffer. This is incorrect and does not happen on the linux version of
	//    the driver nor on AMD drivers at all.
	//    
	//    
	// Links:
	//    - https://www.khronos.org/opengl/wiki/Buffer_Object#Persistent_mapping
	//    - https://www.khronos.org/opengl/wiki/Buffer_Object#Immutable_Storage
	//    
	// Test Procedure:
	//    Two buffers are created, a regular bugger and an immutable buffer. The immutable
	//    buffer is persistently mapped. Then glCopy*BufferSubData is called to copy data
	//    from the regular buffer to the persistently mapped one.
	//    
	//    If the driver is working correctly this sequence of calls works. If the driver is
	//    broken glCopy*BufferSubData will raise an error
	
	deoglRenderThread &renderThread = pCapabilities.GetRenderThread();
	const bool hasBufSto = renderThread.GetExtensions().GetHasExtension( deoglExtensions::ext_ARB_buffer_storage );
	if( ! hasBufSto ){
		return;
	}
	
	const GLfloat data[ 4 ] = { 1.0f, 2.0f, 3.0f, 4.0f };
	void * persistentMapped = nullptr;
	GLuint ssboGpu = 0, ssboCpu = 0;
	const int size = sizeof( data );
	
	try{
		// create gpu buffer and copy test data into it
		OGL_CHECK( renderThread, pglGenBuffers( 1, &ssboGpu ) );
		OGL_CHECK( renderThread, pglBindBuffer( GL_SHADER_STORAGE_BUFFER, ssboGpu ) );
		OGL_CHECK( renderThread, pglBufferData( GL_SHADER_STORAGE_BUFFER, size, data, GL_DYNAMIC_DRAW ) );
		OGL_CHECK( renderThread, pglBindBuffer( GL_SHADER_STORAGE_BUFFER, 0 ) );
		
		// create cpu buffer as immutable storage and persistently map it
		OGL_CHECK( renderThread, pglGenBuffers( 1, &ssboCpu ) );
		OGL_CHECK( renderThread, pglBindBuffer( GL_SHADER_STORAGE_BUFFER, ssboCpu ) );
		OGL_CHECK( renderThread, pglBufferStorage( GL_SHADER_STORAGE_BUFFER, size, nullptr,
			GL_MAP_READ_BIT | GL_MAP_PERSISTENT_BIT | GL_CLIENT_STORAGE_BIT ) );
		OGL_CHECK( renderThread, persistentMapped = pglMapBufferRange( GL_SHADER_STORAGE_BUFFER,
			0, size, GL_MAP_READ_BIT | GL_MAP_PERSISTENT_BIT ) );
		
		// copy data from gpu buffer to cpu buffer
		OGL_CHECK( renderThread, pglBindBuffer( GL_COPY_READ_BUFFER, ssboGpu ) );
		OGL_CHECK( renderThread, pglBindBuffer( GL_COPY_WRITE_BUFFER, ssboCpu ) );
		
		OGL_CHECK( renderThread, pglCopyBufferSubData( GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, size ) );
		
		OGL_CHECK( renderThread, pglBindBuffer( GL_COPY_WRITE_BUFFER, 0 ) );
		OGL_CHECK( renderThread, pglBindBuffer( GL_COPY_READ_BUFFER, 0 ) );
		
		// clean up
		pglBindBuffer( GL_SHADER_STORAGE_BUFFER, ssboCpu );
		pglUnmapBuffer( GL_SHADER_STORAGE_BUFFER );
		pglBindBuffer( GL_SHADER_STORAGE_BUFFER, 0 );
		
		pglDeleteBuffers( 1, &ssboCpu );
		pglDeleteBuffers( 1, &ssboGpu );
		
		pWorking = true;
		
	}catch( const deException & ){
		pWorking = false;
		
		if( persistentMapped ){
			pglBindBuffer( GL_SHADER_STORAGE_BUFFER, ssboCpu );
			pglUnmapBuffer( GL_SHADER_STORAGE_BUFFER );
			pglBindBuffer( GL_SHADER_STORAGE_BUFFER, 0 );
		}
		if( ssboCpu ){
			pglDeleteBuffers( 1, &ssboCpu );
		}
		if( ssboGpu ){
			pglDeleteBuffers( 1, &ssboGpu );
		}
	}
	
	if( pWorking ){
		renderThread.GetLogger().LogInfo( "Capabilities: Persistent Mapped CopySubData: Working" );
		
	}else{
		renderThread.GetLogger().LogWarn( "Capabilities: Persistent Mapped CopySubData: Driver Bug!" );
		return;
	}
	
	// to the same test also for the direct state access version
	const bool hasDsa = renderThread.GetExtensions().GetHasExtension( deoglExtensions::ext_ARB_direct_state_access );
	
	if( hasDsa ){
		persistentMapped = nullptr;
		ssboGpu = 0;
		ssboCpu = 0;
		
		try{
			// create gpu buffer and copy test data into it
			OGL_CHECK( renderThread, pglCreateBuffers( 1, &ssboGpu ) );
			OGL_CHECK( renderThread, pglNamedBufferData( ssboGpu, size, data, GL_DYNAMIC_DRAW ) );
			
			// create cpu buffer as immutable storage and persistently map it
			OGL_CHECK( renderThread, pglCreateBuffers( 1, &ssboCpu ) );
			OGL_CHECK( renderThread, pglNamedBufferStorage( ssboCpu, size, nullptr,
				GL_MAP_READ_BIT | GL_MAP_PERSISTENT_BIT | GL_CLIENT_STORAGE_BIT ) );
			OGL_CHECK( renderThread, persistentMapped = pglMapNamedBufferRange( ssboCpu,
				0, size, GL_MAP_READ_BIT | GL_MAP_PERSISTENT_BIT ) );
			
			// copy data from gpu buffer to cpu buffer
			OGL_CHECK( renderThread, pglCopyNamedBufferSubData( ssboGpu, ssboCpu, 0, 0, size ) );
			
			// clean up
			pglUnmapNamedBuffer( ssboCpu );
			pglDeleteBuffers( 1, &ssboCpu );
			pglDeleteBuffers( 1, &ssboGpu );
			
		}catch( const deException & ){
			pWorking = false;
			
			if( persistentMapped ){
				pglUnmapNamedBuffer( ssboCpu );
			}
			if( ssboCpu ){
				pglDeleteBuffers( 1, &ssboCpu );
			}
			if( ssboGpu ){
				pglDeleteBuffers( 1, &ssboGpu );
			}
		}
		
		if( pWorking ){
			renderThread.GetLogger().LogInfo( "Capabilities: Persistent Mapped NamedCopySubData: Working" );
			
		}else{
			renderThread.GetLogger().LogWarn( "Capabilities: Persistent Mapped NamedCopySubData: Driver Bug!" );
		}
	}
}
