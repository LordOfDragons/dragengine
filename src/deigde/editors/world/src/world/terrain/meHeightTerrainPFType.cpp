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

#include "meHeightTerrainPFLayer.h"
#include "meHeightTerrainPFType.h"
#include "meHeightTerrainSector.h"
#include "meHeightTerrain.h"
#include "../meWorld.h"

#include <deigde/environment/igdeEnvironment.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/model/deModel.h>
#include <dragengine/resources/model/deModelManager.h>
#include <dragengine/resources/skin/deSkin.h>
#include <dragengine/resources/skin/deSkinManager.h>
#include <dragengine/resources/propfield/dePropField.h>
#include <dragengine/resources/propfield/dePropFieldType.h>
#include <dragengine/resources/propfield/dePropFieldInstance.h>



// Definitions
////////////////

#define LOGSOURCE "World Editor"



// Class meHeightTerrainPFType
////////////////////////////////

// Constructor, destructor
////////////////////////////

meHeightTerrainPFType::meHeightTerrainPFType( deEngine *engine ) :
pPFLayer( NULL ),
pEngine( engine ),
pModel( NULL ),
pSkin( NULL ),
pRotationPerForce( 5.0f ),
pRestitution( 0.5f ),
pCoverageDensity( 1 ), //10; // 10 props per 1m^2
pRandomScaleMin( 0.5f ),
pRandomScaleMax( 1.5f ),
pRandomRotationMin( 0.0f, -30.0f, 0.0f ),
pRandomRotationMax( 0.0f, 30.0f, 0.0f )
{
	if( ! engine ){
		DETHROW( deeInvalidParam );
	}
}

meHeightTerrainPFType::~meHeightTerrainPFType(){
	if( pSkin ){
		pSkin->FreeReference();
	}
	if( pModel ){
		pModel->FreeReference();
	}
}



// Management
///////////////

void meHeightTerrainPFType::SetPFLayer( meHeightTerrainPFLayer *pflayer ){
	pPFLayer = pflayer;
}

void meHeightTerrainPFType::NotifyTypeChanged(){
	if( ! pPFLayer ){
		return;
	}
	
	meHeightTerrainSector * const htsector = pPFLayer->GetHTSector();
	if( htsector == NULL ){
		return;
	}
	
	if( htsector->GetHeightTerrain() ){
		htsector->GetHeightTerrain()->RebuildVegetationPropFieldTypes();
		htsector->GetHeightTerrain()->SetChanged( true );
	}
}



void meHeightTerrainPFType::SetPathModel( const char *path ){
	if( ! path ){
		DETHROW( deeInvalidParam );
	}
	
	if( pPathModel.Equals( path ) ){
		return;
	}
	
	pPathModel = path;
	
	if( pModel ){
		pModel->FreeReference();
		pModel = NULL;
	}
	
	if( ! pPathModel.IsEmpty() ){
		try{
			pModel = pEngine->GetModelManager()->LoadModel( pPathModel, "/" );
			
		}catch( const deException &e ){
			if( pPFLayer && pPFLayer->GetHTSector() && pPFLayer->GetHTSector()->GetHeightTerrain() ){
				pPFLayer->GetHTSector()->GetHeightTerrain()->GetWorld().GetLogger()->LogException( LOGSOURCE, e );
			}
		}
	}
	
	NotifyTypeChanged();
}

void meHeightTerrainPFType::SetPathSkin( const char *path ){
	if( ! path ){
		DETHROW( deeInvalidParam );
	}
	
	if( pPathSkin.Equals( path ) ){
		return;
	}
	
	pPathSkin = path;
	
	if( pSkin ){
		pSkin->FreeReference();
		pSkin = NULL;
	}
	
	if( ! pPathSkin.IsEmpty() ){
		try{
			pSkin = pEngine->GetSkinManager()->LoadSkin( pPathSkin.GetString(), "/" );
			
		}catch( const deException & ){
			if( pPFLayer && pPFLayer->GetHTSector() && pPFLayer->GetHTSector()->GetHeightTerrain() ){
				pSkin = pPFLayer->GetHTSector()->GetHeightTerrain()->GetWorld().
					GetEnvironment()->GetStockSkin( igdeEnvironment::essError );
				pSkin->AddReference();
			}
		}
	}
	
	NotifyTypeChanged();
}

void meHeightTerrainPFType::SetRotationPerForce( float rotation ){
	rotation = decMath::max( rotation, 0.0f );
	
	if( fabsf( rotation - pRotationPerForce ) < FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pRotationPerForce = rotation;
	
	NotifyTypeChanged();
}

void meHeightTerrainPFType::SetRestitution( float restitution ){
	restitution = decMath::max( restitution, 0.0f );
	
	if( fabsf( restitution - pRestitution ) < FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pRestitution = restitution;
	
	NotifyTypeChanged();
}



void meHeightTerrainPFType::SetCoverageDensity( int density ){
	if( density == pCoverageDensity ){
		return;
	}
	
	pCoverageDensity = density;
	
	NotifyTypeChanged();
}

void meHeightTerrainPFType::SetMinimumRandomScaling( float scaling ){
	if( fabsf( scaling - pRandomScaleMin ) < FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pRandomScaleMin = scaling;
	
	NotifyTypeChanged();
}

void meHeightTerrainPFType::SetMaximumRandomScaling( float scaling ){
	if( fabsf( scaling - pRandomScaleMax ) < FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pRandomScaleMax = scaling;
	
	NotifyTypeChanged();
}

void meHeightTerrainPFType::SetMinimumRandomRotation( const decVector &rotation ){
	if( rotation.IsEqualTo( pRandomRotationMin ) ){
		return;
	}
	
	pRandomRotationMin = rotation;
	
	NotifyTypeChanged();
}

void meHeightTerrainPFType::SetMaximumRandomRotation( const decVector &rotation ){
	if( rotation.IsEqualTo( pRandomRotationMax ) ){
		return;
	}
	
	pRandomRotationMax = rotation;
	
	NotifyTypeChanged();
}
