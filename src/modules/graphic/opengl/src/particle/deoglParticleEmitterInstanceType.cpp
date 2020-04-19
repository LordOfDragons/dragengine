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
#include <stdlib.h>

#include "deoglParticleEmitter.h"
#include "deoglParticleEmitterInstance.h"
#include "deoglParticleEmitterInstanceType.h"
#include "deoglParticleEmitterType.h"
#include "deoglRParticleEmitterInstanceType.h"
#include "../deGraphicOpenGl.h"
#include "../skin/deoglRSkin.h"
#include "../skin/deoglSkin.h"
#include "../skin/dynamic/deoglDynamicSkin.h"
#include "../skin/dynamic/deoglRDynamicSkin.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/particle/deParticleEmitter.h>
#include <dragengine/resources/particle/deParticleEmitterInstance.h>
#include <dragengine/resources/particle/deParticleEmitterType.h>
#include <dragengine/resources/particle/deParticleEmitterInstanceType.h>
#include <dragengine/resources/skin/deSkin.h>
#include <dragengine/resources/skin/dynamic/deDynamicSkin.h>



// Class deoglParticleEmitterInstanceType
///////////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglParticleEmitterInstanceType::deoglParticleEmitterInstanceType(
deoglParticleEmitterInstance &instance, int index ) :
pEmitterInstance( instance ),
pIndex( index ),
pRType( NULL ),
pDirtyType( true ),
pDirtyParamBlocks( true )
{
	try{
		pRType = new deoglRParticleEmitterInstanceType( *instance.GetRInstance(), index );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

deoglParticleEmitterInstanceType::~deoglParticleEmitterInstanceType(){
	pCleanUp();
}



// Management
///////////////

void deoglParticleEmitterInstanceType::SyncToRender(){
	if( pDirtyType ){
		const deParticleEmitterInstanceType &itype = pEmitterInstance.GetInstance().GetTypeAt( pIndex );
		if( itype.GetDynamicSkin() ){
			pRType->SetDynamicSkin( ( ( deoglDynamicSkin* )itype.GetDynamicSkin()->GetPeerGraphic() )->GetRDynamicSkin() );
			
		}else{
			pRType->SetDynamicSkin( NULL );
		}
		
		deoglParticleEmitter * const emitter = pEmitterInstance.GetEmitter();
		if( emitter ){
			const deParticleEmitterType &etype = emitter->GetParticleEmitter().GetTypeAt( pIndex );
			
			if( etype.GetSkin() ){
				pRType->SetUseSkin( ( ( deoglSkin* )etype.GetSkin()->GetPeerGraphic() )->GetRSkin() );
				
			}else{
				pRType->SetUseSkin( NULL );
			}
			
		}else{
			pRType->SetUseSkin( NULL );
		}
		
		pDirtyType = false;
	}
	
	if( pDirtyParamBlocks ){
		pRType->InvalidateParamBlocks();
		pDirtyParamBlocks = false;
	}
}



void deoglParticleEmitterInstanceType::InvalidateParamBlocks(){
	pDirtyParamBlocks = true;
}



void deoglParticleEmitterInstanceType::TypeChanged(){
	pDirtyType = true;
}



// Private Functions
//////////////////////

void deoglParticleEmitterInstanceType::pCleanUp(){
	if( pRType ){
		pRType->FreeReference();
	}
}
