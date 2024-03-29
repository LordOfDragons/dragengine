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

#include "deoglFence.h"
#include "../delayedoperation/deoglDelayedOperations.h"
#include "../renderthread/deoglRenderThread.h"

#include <dragengine/common/exceptions.h>


// Class deoglFence
/////////////////////

// Constructor, destructor
////////////////////////////

deoglFence::deoglFence( deoglRenderThread &renderThread ) :
pRenderThread( renderThread ),
pFence( nullptr ){
}

deoglFence::~deoglFence(){
	pRenderThread.GetDelayedOperations().DeleteOpenGLSync( pFence );
}



// Management
///////////////

void deoglFence::Arm(){
	Reset();
	pFence = pglFenceSync( GL_SYNC_GPU_COMMANDS_COMPLETE, 0 );
	
	// ensure the fence is processed by the GPU. AMD goes to work quickly but nVidia delays
	// processing the queue causing the signal to not be processed causing glClientWaitSync()
	// to wait forever. we could use GL_SYNC_FLUSH_COMMANDS_BIT for glClientWaitSync() but
	// according to docs it is safer to do an explicit glFlush() due to driver peculiarities
	glFlush();
}

bool deoglFence::HasFired(){
	if( ! pFence ){
		return true;
	}
	
	oglClearError();
	
	switch( pglClientWaitSync( pFence, 0, 0 ) ){ // 0s timeout
	case GL_ALREADY_SIGNALED:
	case GL_CONDITION_SATISFIED:
		pFence = nullptr;
		return true;
		
	case GL_TIMEOUT_EXPIRED:
		return false;
		
	case GL_WAIT_FAILED:
		pFence = nullptr;
		dbgCheckOglError( pRenderThread, __FILE__, __LINE__ );
		return true;
		
	default:
		pFence = nullptr;
		DETHROW_INFO( deeInvalidAction, "Unknown return value while waiting for fence" );
	}
}

void deoglFence::Wait(){
	if( ! pFence ){
		return;
	}
	
	oglClearError();
	
	switch( pglClientWaitSync( pFence, 0, 3000000000 ) ){ // 3s timeout
	case GL_ALREADY_SIGNALED:
	case GL_CONDITION_SATISFIED:
		pFence = nullptr;
		return;
		
	case GL_TIMEOUT_EXPIRED:
		pFence = nullptr;
		DETHROW_INFO( deeInvalidAction, "Timeout while waiting for fence" );
		
	case GL_WAIT_FAILED:
		pFence = nullptr;
		dbgCheckOglError( pRenderThread, __FILE__, __LINE__ );
		return;
		
	default:
		pFence = nullptr;
		DETHROW_INFO( deeInvalidAction, "Unknown return value while waiting for fence" );
	}
}

void deoglFence::Reset(){
	if( pFence ){
		pglDeleteSync( pFence );
		pFence = nullptr;
	}
}
