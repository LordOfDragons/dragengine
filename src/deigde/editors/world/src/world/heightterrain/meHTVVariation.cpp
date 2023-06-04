/* 
 * Drag[en]gine IGDE World Editor
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

#include "meHTVVariation.h"
#include "meHTVegetationLayer.h"
#include "../meWorld.h"
#include "../terrain/meHeightTerrain.h"
#include "../terrain/meHeightTerrainSector.h"

#include <deigde/environment/igdeEnvironment.h>

#include <dragengine/deEngine.h>
#include <dragengine/resources/model/deModel.h>
#include <dragengine/resources/model/deModelManager.h>
#include <dragengine/resources/skin/deSkin.h>
#include <dragengine/resources/skin/deSkinManager.h>
#include <dragengine/common/exceptions.h>



// Class meHTVegetationLayer
////////////////////////////////////

// Constructor, destructor
////////////////////////////

meHTVVariation::meHTVVariation( deEngine *engine ){
	if( ! engine ) DETHROW( deeInvalidParam );
	
	pEngine = engine;
	pVLayer = NULL;
	
	pModel = NULL;
	pSkin = NULL;
	pRotationPerForce = 5.0f;
	pRestitution = 0.5f;
	
	pRandomScaleMin = 0.5f;
	pRandomScaleMax = 1.5f;
	pRandomRotationMin = -30.0f;
	pRandomRotationMax = 30.0f;
}

meHTVVariation::~meHTVVariation(){
	if( pSkin ) pSkin->FreeReference();
	if( pModel ) pModel->FreeReference();
}



// Management
///////////////

void meHTVVariation::SetVLayer( meHTVegetationLayer *vlayer ){
	pVLayer = vlayer;
}



void meHTVVariation::SetPathModel( const char *path ){
	if( ! path ) DETHROW( deeInvalidParam );
	
	if( ! pPathModel.Equals( path ) ){
		pPathModel = path;
		
		if( pModel ){
			pModel->FreeReference();
			pModel = NULL;
		}
		
		if( ! pPathModel.IsEmpty() ){
			try{
				pModel = pEngine->GetModelManager()->LoadModel( pPathModel.GetString(), "/" );
				
			}catch( const deException & ){
			}
		}
		
		NotifyChanged();
	}
}

void meHTVVariation::SetPathSkin( const char *path ){
	if( ! path ) DETHROW( deeInvalidParam );
	
	if( ! pPathSkin.Equals( path ) ){
		pPathSkin = path;
		
		if( pSkin ){
			pSkin->FreeReference();
			pSkin = NULL;
		}
		
		if( ! pPathSkin.IsEmpty() ){
			try{
				pSkin = pEngine->GetSkinManager()->LoadSkin( pPathSkin.GetString(), "/" );
				
			}catch( const deException & ){
				if( pVLayer && pVLayer->GetHeightTerrain() ){
					pSkin = pVLayer->GetHeightTerrain()->GetWorld().GetEnvironment()->
						GetStockSkin( igdeEnvironment::essError );
					pSkin->AddReference();
				}
			}
		}
		
		NotifyChanged();
	}
}

void meHTVVariation::SetRotationPerForce( float rotation ){
	if( rotation < 0.0f ) rotation = 0.0f;
	
	if( fabsf( rotation - pRotationPerForce ) > 1e-5f ){
		pRotationPerForce = rotation;
		
		NotifyChanged();
	}
}

void meHTVVariation::SetRestitution( float restitution ){
	if( restitution < 0.0f ) restitution = 0.0f;
	
	if( fabsf( restitution - pRestitution ) > 1e-5f ){
		pRestitution = restitution;
		
		NotifyChanged();
	}
}



void meHTVVariation::SetMinimumRandomScaling( float scaling ){
	if( fabsf( scaling - pRandomScaleMin ) > 1e-5f ){
		pRandomScaleMin = scaling;
		
		NotifyChanged();
	}
}

void meHTVVariation::SetMaximumRandomScaling( float scaling ){
	if( fabsf( scaling - pRandomScaleMax ) > 1e-5f ){
		pRandomScaleMax = scaling;
		
		NotifyChanged();
	}
}

void meHTVVariation::SetMinimumRandomRotation( float rotation ){
	if( fabsf( rotation - pRandomRotationMin ) > 1e-5f ){
		pRandomRotationMin = rotation;
		
		NotifyChanged();
	}
}

void meHTVVariation::SetMaximumRandomRotation( float rotation ){
	if( fabsf( rotation - pRandomRotationMax ) > 1e-5f ){
		pRandomRotationMax = rotation;
		
		NotifyChanged();
	}
}



void meHTVVariation::NotifyChanged(){
	if( pVLayer ){
		meHeightTerrain *hterrain = pVLayer->GetHeightTerrain();
		if( hterrain ){
			hterrain->GetWorld().NotifyHTVLVariationChanged( pVLayer, this );
			hterrain->SetChanged( true );
			hterrain->InvalidateAllPropFields();
		}
	}
}
