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
#include "deoglRParticleEmitterType.h"
#include "../deGraphicOpenGl.h"
#include "../skin/deoglSkin.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/curve/decCurveBezierEvaluator.h>
#include <dragengine/resources/particle/deParticleEmitter.h>
#include <dragengine/resources/particle/deParticleEmitterType.h>
#include <dragengine/resources/particle/deParticleEmitterOffsets.h>
#include <dragengine/resources/particle/deParticleEmitterParameter.h>
#include <dragengine/resources/skin/deSkin.h>



// Class deoglParticleEmitterType
///////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglParticleEmitterType::deoglParticleEmitterType(
deoglParticleEmitter &emitter, const deParticleEmitterType &type ) :
pType( type ),
pRType( NULL ),
pDirtyType( true ){
	pRType = new deoglRParticleEmitterType( *emitter.GetREmitter() );
}

deoglParticleEmitterType::~deoglParticleEmitterType(){
	if( pRType ){
		pRType->FreeReference();
	}
}



// Management
///////////////

void deoglParticleEmitterType::SyncToRender(){
	if( pDirtyType ){
		if( pType.GetSkin() ){
			pRType->SetSkin( ( ( deoglSkin* )pType.GetSkin()->GetPeerGraphic() )->GetRSkin() );
			
		}else{
			pRType->SetSkin( NULL );
		}
		
		pRType->SetSimulationType( pType.GetSimulationType() );
		
		pRType->CheckEmitLight( pType );
		pRType->UpdateParameterSamples( pType );
		pRType->DropPipelines();
		
		pDirtyType = false;
	}
}



void deoglParticleEmitterType::TypeChanged(){
	pDirtyType = true;
}
