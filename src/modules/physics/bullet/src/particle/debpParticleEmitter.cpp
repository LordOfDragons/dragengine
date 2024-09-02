/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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
