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

#include "deoglParticleEmitterType.h"
#include "deoglParticleEmitterInstance.h"
#include "deoglParticleEmitter.h"
#include "deoglParticleEmitterInstanceType.h"
#include "deoglRParticleEmitterInstance.h"
#include "../deGraphicOpenGl.h"
#include "../skin/deoglSkin.h"
#include "../skin/deoglSkinTexture.h"
#include "../skin/dynamic/deoglDynamicSkin.h"
#include "../sky/deoglSky.h"
#include "../sky/deoglRSky.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/errortracing/deErrorTrace.h>
#include <dragengine/errortracing/deErrorTracePoint.h>
#include <dragengine/errortracing/deErrorTraceValue.h>
#include <dragengine/resources/particle/deParticleEmitter.h>
#include <dragengine/resources/particle/deParticleEmitterInstance.h>
#include <dragengine/resources/particle/deParticleEmitterInstanceType.h>
#include <dragengine/resources/particle/deParticleEmitterOffsets.h>
#include <dragengine/resources/particle/deParticleEmitterType.h>



// Class deoglParticleEmitterInstance
///////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglParticleEmitterInstance::deoglParticleEmitterInstance(
deGraphicOpenGl &ogl, const deParticleEmitterInstance &instance ) :
pOgl( ogl ),
pInstance( instance ),
pRInstance( NULL ),

pEmitter( NULL ),

pTypes( NULL ),
pTypeCount( 0 ),

pDirtyEmitter( true ),
pDirtyExtends( true ),
pDirtyOctreeNode( true ),
pDirtyParticles( true ),
pDirtyInstance( true ),
pDirtyTypes( true )
{
	try{
		pRInstance = new deoglRParticleEmitterInstance( ogl.GetRenderThread() );
		EmitterChanged();
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

deoglParticleEmitterInstance::~deoglParticleEmitterInstance(){
	pCleanUp();
}



// Management
///////////////

void deoglParticleEmitterInstance::SyncToRender(){
	if( pDirtyInstance ){
		pRInstance->SetBurstTime( pInstance.GetBurstTime() );
		pRInstance->SetPosition( pInstance.GetPosition() );
		pRInstance->SetReferencePosition( pInstance.GetReferencePosition() );
		pRInstance->SetLayerMask( pInstance.GetLayerMask() );
		pDirtyInstance = false;
	}
	
	// emitter
	if( pEmitter ){
		pEmitter->SyncToRender();
	}
	
	if( pDirtyEmitter ){
		if( pEmitter ){
			pRInstance->SetEmitter( pEmitter->GetREmitter() );
			
		}else{
			pRInstance->SetEmitter( NULL );
		}
		
		pDirtyEmitter = false;
	}
	
	// types
	if( pDirtyTypes ){
		pRInstance->RemoveAllTypes();
		int i;
		for( i=0; i<pTypeCount; i++ ){
			pRInstance->AddType( pTypes[ i ]->GetRType() );
		}
		pDirtyTypes = false;
	}
	
	int i;
	for( i=0; i<pTypeCount; i++ ){
		pTypes[ i ]->SyncToRender();
	}
	
	// extends and octree
	if( pDirtyExtends ){
		pRInstance->UpdateExtends( pInstance ); // depends on types to be correct
		pDirtyExtends = false;
	}
	
	if( pDirtyOctreeNode ){
		pRInstance->UpdateOctreeNode();
		pDirtyOctreeNode = false;
	}
	
	// particles
	if( pDirtyParticles ){
		pRInstance->UpdateParticles( pInstance );
		pDirtyParticles = false;
	}
}



deoglParticleEmitterInstanceType &deoglParticleEmitterInstance::GetTypeAt( int index ){
	if( index < 0 || index >= pTypeCount ){
		DETHROW( deeInvalidParam );
	}
	return *pTypes[ index ];
}



// Notifications
//////////////////

void deoglParticleEmitterInstance::EmitterChanged(){
	if( pInstance.GetEmitter() ){
		pEmitter = ( deoglParticleEmitter* )pInstance.GetEmitter()->GetPeerGraphic();
		
	}else{
		pEmitter = NULL;
	}
	
	pUpdateTypes();
	
	pDirtyInstance = true;
	pDirtyEmitter = true;
	pDirtyExtends = true;
	pDirtyParticles = true;
	pDirtyOctreeNode = true;
}

void deoglParticleEmitterInstance::PositionChanged(){
	pDirtyInstance = true;
	pDirtyOctreeNode = true;
	pDirtyExtends = true;
	pDirtyParticles = true;
}

void deoglParticleEmitterInstance::OrientationChanged(){
}

void deoglParticleEmitterInstance::EnableCastingChanged(){
}

void deoglParticleEmitterInstance::ReferencePositionChanged(){
	pDirtyInstance = true;
}

void deoglParticleEmitterInstance::CollisionFilterChanged(){
}

void deoglParticleEmitterInstance::WarmUpTimeChanged(){
}

void deoglParticleEmitterInstance::BurstTimeChanged(){
	pDirtyInstance = true;
}

void deoglParticleEmitterInstance::LayerMaskChanged(){
	pDirtyInstance = true;
}

void deoglParticleEmitterInstance::ControllerChanged( int ){
}

void deoglParticleEmitterInstance::TypeChanged( int type ){
	pTypes[ type ]->TypeChanged();
	
	pDirtyOctreeNode = true;
	pDirtyExtends = true;
	pDirtyParticles = true;
}

void deoglParticleEmitterInstance::TypeParticlesChanged( int ){
	pDirtyOctreeNode = true;
	pDirtyExtends = true;
	pDirtyParticles = true;
}


void deoglParticleEmitterInstance::ResetBurst(){
}

void deoglParticleEmitterInstance::KillAllParticles(){
}



// Private functions
//////////////////////

void deoglParticleEmitterInstance::pCleanUp(){
	if( pTypes ){
		int i;
		for( i=0; i<pTypeCount; i++ ){
			delete pTypes[ i ];
		}
		delete [] pTypes;
	}
	
	// types holds a reference to pRInstance. do not remove it earlier
	if( pRInstance ){
		pRInstance->FreeReference();
		pRInstance = NULL;
	}
}

void deoglParticleEmitterInstance::pUpdateTypes(){
	const int typeCount = pInstance.GetTypeCount();
	
	if( pTypes ){
		int i;
		for( i=0; i<pTypeCount; i++ ){
			delete pTypes[ i ];
		}
		delete [] pTypes;
		pTypes = NULL;
	}
	pTypeCount = 0;
	
	if( typeCount > 0 ){
		pTypes = new deoglParticleEmitterInstanceType*[ typeCount ];
		
		for( pTypeCount=0; pTypeCount<typeCount; pTypeCount++ ){
			pTypes[ pTypeCount ] = new deoglParticleEmitterInstanceType( *this, pTypeCount );
		}
	}
	
	pDirtyTypes = true;
	pDirtyParticles = true;
	pDirtyOctreeNode = true;
	pDirtyExtends = true;
}
