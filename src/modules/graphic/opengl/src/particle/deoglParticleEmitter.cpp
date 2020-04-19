/* 
 * Drag[en]gine OpenGL Graphic Module
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
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "deoglParticleEmitter.h"
#include "deoglParticleEmitterType.h"
#include "deoglRParticleEmitter.h"
#include "../deGraphicOpenGl.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/curve/decCurveBezierEvaluator.h>
#include <dragengine/errortracing/deErrorTrace.h>
#include <dragengine/errortracing/deErrorTracePoint.h>
#include <dragengine/errortracing/deErrorTraceValue.h>
#include <dragengine/resources/particle/deParticleEmitter.h>
#include <dragengine/resources/particle/deParticleEmitterOffsets.h>
#include <dragengine/resources/particle/deParticleEmitterParameter.h>
#include <dragengine/resources/particle/deParticleEmitterType.h>



// Class deoglParticleEmitter
///////////////////////////////

// Constructor, destructor
////////////////////////////

deoglParticleEmitter::deoglParticleEmitter( deGraphicOpenGl &ogl, const deParticleEmitter &emitter ) :
pOgl( ogl ),
pParticleEmitter( emitter ),
pREmitter( NULL ),
pTypes( NULL ),
pTypeCount( 0 ),
pDirtyTypes( true )
{
	try{
		pREmitter = new deoglRParticleEmitter( ogl.GetRenderThread() );
		TypeCountChanged();
		
	}catch( const deException & ){
		pCleanUp();
	}
}

deoglParticleEmitter::~deoglParticleEmitter(){
	pCleanUp();
}



// Management
///////////////

void deoglParticleEmitter::SyncToRender(){
	int i;
	
	if( pDirtyTypes ){
		pREmitter->RemoveAllTypes();
		for( i=0; i<pTypeCount; i++ ){
			pREmitter->AddType( pTypes[ i ]->GetRType() );
		}
		pDirtyTypes = false;
	}
	
	for( i=0; i<pTypeCount; i++ ){
		pTypes[ i ]->SyncToRender();
	}
}



deoglParticleEmitterType &deoglParticleEmitter::GetTypeAt( int index ) const{
	if( index < 0 || index >= pTypeCount ){
		DETHROW( deeInvalidParam );
	}
	return *pTypes[ index ];
}



// Notifications
//////////////////

void deoglParticleEmitter::ControllerCountChanged(){
}

void deoglParticleEmitter::ControllerChanged( int controller ){
}

void deoglParticleEmitter::TypeCountChanged(){
	const int typeCount = pParticleEmitter.GetTypeCount();
	
	if( pTypes ){
		while( pTypeCount > 0 ){
			pTypeCount--;
			delete pTypes[ pTypeCount ];
		}
		delete [] pTypes;
		pTypes = NULL;
	}
	pTypeCount = 0;
	
	if( typeCount > 0 ){
		pTypes = new deoglParticleEmitterType*[ typeCount ];
		for( pTypeCount=0; pTypeCount<typeCount; pTypeCount++ ){
			pTypes[ pTypeCount ] = new deoglParticleEmitterType( *this, pParticleEmitter.GetTypeAt( pTypeCount ) );
		}
	}
	
	pDirtyTypes = true;
}

void deoglParticleEmitter::TypeChanged( int type ){
	pTypes[ type ]->TypeChanged();
}



// Private Functions
//////////////////////

void deoglParticleEmitter::pCleanUp(){
	if( pTypes ){
		while( pTypeCount > 0 ){
			pTypeCount--;
			delete pTypes[ pTypeCount ];
		}
		delete [] pTypes;
	}
	
	// types holds a reference to pREmitter. do not remove it earlier
	if( pREmitter ){
		pREmitter->FreeReference();
		pREmitter = NULL;
	}
}
