/* 
 * Drag[en]gine Bullet Physics Module
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
#include <string.h>

#include "debpParticleEmitter.h"
#include "debpParticleEmitterType.h"
#include "../debpCommon.h"
#include "../dePhysicsBullet.h"

#include <dragengine/resources/particle/deParticleEmitterController.h>
#include <dragengine/common/curve/decCurveBezierEvaluator.h>
#include <dragengine/common/exceptions.h>



#ifndef random
#define random rand
#endif

// Class debpParticleEmitter
//////////////////////////////

// Constructor, destructor
////////////////////////////

debpParticleEmitter::debpParticleEmitter( dePhysicsBullet *bullet, deParticleEmitter *emitter ){
	if( ! bullet || ! emitter ){
		DETHROW( deeInvalidParam );
	}
	
	pBullet = bullet;
	pEmitter = emitter;
	
	pTypes = NULL;
	pTypeCount = 0;
	
	pDirtyParameters = true;
	pDirtyGraRequests = true;
	
	pUpdateTypes();
}

debpParticleEmitter::~debpParticleEmitter(){
	pCleanUp();
}



// Management
///////////////

debpParticleEmitterType &debpParticleEmitter::GetTypeAt( int index ){
	if( index < 0 || index >= pTypeCount ){
		DETHROW( deeInvalidParam );
	}
	
	return pTypes[ index ];
}

const debpParticleEmitterType &debpParticleEmitter::GetTypeAt( int index ) const{
	if( index < 0 || index >= pTypeCount ){
		DETHROW( deeInvalidParam );
	}
	
	return pTypes[ index ];
}



void debpParticleEmitter::UpdateParameters(){
	if( pDirtyParameters ){
		int t;
		
		for( t=0; t<pTypeCount; t++ ){
			pTypes[ t ].UpdateParameters();
		}
		
		pDirtyParameters = false;
	}
}



// Notifications
//////////////////

void debpParticleEmitter::TypeCountChanged(){
	pDirtyParameters = true;
	pUpdateTypes();
}

void debpParticleEmitter::TypeChanged( int type ){
	pDirtyParameters = true;
	
	if( type >= 0 && type < pTypeCount ){
		pTypes[ type ].UpdateType();
	}
	// this would require a link back to all instances using it
	//if( parameter == deParticleEmitter::epInterval ){
	//	pUpdateCastIntervals();
	//}
}

void debpParticleEmitter::GraphicModuleRequestsChanged(){
	pDirtyGraRequests = true;
}



// Private Functions
//////////////////////

void debpParticleEmitter::pCleanUp(){
	if( pTypes ){
		delete [] pTypes;
	}
}

void debpParticleEmitter::pUpdateTypes(){
	const int typeCount = pEmitter->GetTypeCount();
	
	if( pTypes ){
		delete [] pTypes;
		pTypes = NULL;
	}
	pTypeCount = 0;
	
	if( typeCount > 0 ){
		pTypes = new debpParticleEmitterType[ typeCount ];
		
		for( pTypeCount=0; pTypeCount<typeCount; pTypeCount++ ){
			pTypes[ pTypeCount ].SetEmitter( pEmitter );
			pTypes[ pTypeCount ].SetType( pTypeCount );
			pTypes[ pTypeCount ].UpdateType();
		}
	}
	
	pDirtyParameters = true;
}
