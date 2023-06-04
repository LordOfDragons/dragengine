/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2022, Roland Plüss (roland@rptd.ch)
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

#include "deoglSkinTexturePipelinesList.h"
#include "deoglSkinTexturePipelines.h"
#include "deoglSTPipelinesBillboard.h"
#include "deoglSTPipelinesComponent.h"
#include "deoglSTPipelinesDecal.h"
#include "deoglSTPipelinesOutline.h"
#include "heightmap/deoglSTPipelinesHeightMap1.h"
#include "heightmap/deoglSTPipelinesHeightMap2.h"
#include "particle/deoglSTPipelinesParticle.h"
#include "particle/deoglSTPipelinesParticleBeam.h"
#include "particle/deoglSTPipelinesParticleRibbon.h"
#include "propfield/deoglSTPipelinesPropField.h"
#include "propfield/deoglSTPipelinesPropFieldImposter.h"
#include "../../shaders/deoglShaderLoadingTimeout.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/utils/decTimer.h>



// Class deoglSkinTexturePipelinesList
////////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglSkinTexturePipelinesList::deoglSkinTexturePipelinesList( const deoglSkinTexture &texture ) :
pTexture( texture )
{
	pPipelines[ eptComponent ].TakeOver( new deoglSTPipelinesComponent( texture ) );
	pPipelines[ eptBillboard ].TakeOver( new deoglSTPipelinesBillboard( texture ) );
	pPipelines[ eptDecal ].TakeOver( new deoglSTPipelinesDecal( texture ) );
	pPipelines[ eptPropField ].TakeOver( new deoglSTPipelinesPropField( texture ) );
	pPipelines[ eptPropFieldImposter ].TakeOver( new deoglSTPipelinesPropFieldImposter( texture ) );
	pPipelines[ eptHeightMap1 ].TakeOver( new deoglSTPipelinesHeightMap1( texture ) );
	pPipelines[ eptHeightMap2 ].TakeOver( new deoglSTPipelinesHeightMap2( texture ) );
	pPipelines[ eptParticle ].TakeOver( new deoglSTPipelinesParticle( texture ) );
	pPipelines[ eptParticleRibbon ].TakeOver( new deoglSTPipelinesParticleRibbon( texture ) );
	pPipelines[ eptParticleBeam ].TakeOver( new deoglSTPipelinesParticleBeam( texture ) );
	pPipelines[ eptOutline ].TakeOver( new deoglSTPipelinesOutline( texture ) );
}

deoglSkinTexturePipelinesList::~deoglSkinTexturePipelinesList(){
}



// Management
///////////////

deoglSkinTexturePipelines &deoglSkinTexturePipelinesList::GetAt( ePipelineTypes type ) const{
	return pPipelines[ type ];
}

void deoglSkinTexturePipelinesList::Prepare( deoglShaderLoadingTimeout &timeout ){
	int i;
	for( i=0; i<PipelineTypesCount; i++ ){
		pPipelines[ i ]->Prepare( timeout );
		if( timeout.TimedOut() ){
			return;
		}
	}
}
