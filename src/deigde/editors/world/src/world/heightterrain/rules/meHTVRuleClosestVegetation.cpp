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

#include "meHTVRuleClosestVegetation.h"
#include "meHTVRLink.h"
#include "meHTVRSlot.h"
#include "../meHTVInstance.h"
#include "../meHTVEvaluationEnvironment.h"
#include "../../terrain/meHeightTerrain.h"
#include "../../terrain/meHeightTerrainSector.h"
#include "../../terrain/meHeightTerrainPropField.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/propfield/dePropField.h>
#include <dragengine/resources/world/deWorld.h>



// Class meHTVRuleClosestVegetation
/////////////////////////////////////

// Constructor, destructor
////////////////////////////

meHTVRuleClosestVegetation::meHTVRuleClosestVegetation() : meHTVRule( ertClosestVegetation, 2 ){
	pSearchRadius = 1.0f;
	
	GetSlotAt( eosDirection ).SetIsInput( false );
	GetSlotAt( eosDistance ).SetIsInput( false );
	
	Reset();
}

meHTVRuleClosestVegetation::meHTVRuleClosestVegetation( const meHTVRuleClosestVegetation &rule ) :
meHTVRule( rule ),
pVegetationType( rule.pVegetationType ),
pSearchRadius( rule.pSearchRadius ),
pDistance( rule.pDistance ),
pDirection( rule.pDirection ),
pDirty( false ){
}

meHTVRuleClosestVegetation::~meHTVRuleClosestVegetation(){
}



// Management
///////////////

void meHTVRuleClosestVegetation::SetVegetationType( const char *vegetationType ){
	if( ! vegetationType ) DETHROW( deeInvalidParam );
	
	pVegetationType = vegetationType;
	
	Reset();
}

void meHTVRuleClosestVegetation::SetSearchRadius( float searchRadius ){
	pSearchRadius = searchRadius;
	
	Reset();
}

void meHTVRuleClosestVegetation::UpdateResult( meHTVEvaluationEnvironment &evalEnv ){
	if( ! pDirty ){
		return;
	}
	
	float srSquared = pSearchRadius * pSearchRadius;
	float bestDistSquared = srSquared;
	
	// for the time being we will restrict ourselves on looking in the current
	// prop field. this is not the best solution but works well enough
#if 0
	meHeightTerrainSector *htsector = evalEnv.GetHTSector();
	
	if( htsector && htsector->GetHeightTerrain() && false ){
		const decDVector &ipos = evalEnv.GetPosition();
		meHeightTerrainPropField *htpf;
		int i, pfiCount = 0;
		int p, pfCount = 0;
		decVector testIPos;
		float distSquared;
		decVector posDiff;
		float pfDropDist;
		
		pfDropDist = htsector->GetHeightTerrain()->GetSectorSize() / ( float )htsector->GetPropFieldCellCount();
		pfCount = htsector->GetPropFieldCount();
		
		for( p=0; p<pfCount; p++ ){
			htpf = htsector->GetPropFieldAt( p );
			if( htpf->GetEnginePropField() ){
				testIPos = ( ipos - htpf->GetEnginePropField()->GetPosition() ).ToVector();
				
				if( testIPos.x >= -pfDropDist && testIPos.x <= pfDropDist && testIPos.z >= -pfDropDist && testIPos.z <= pfDropDist ){
					pfiCount = htpf->GetVInstanceCount();
					
					for( i=0; i<pfiCount; i++ ){
						posDiff = htpf->GetVInstanceAt( i ).GetPosition() - testIPos;
						distSquared = posDiff * posDiff;
						
						if( distSquared < bestDistSquared ){
							bestDistSquared = distSquared;
						}
					}
				}
			}
		}
	}
#endif
	
	pDistance = sqrtf( bestDistSquared );
	
	// no more dirty
	pDirty = false;
}



void meHTVRuleClosestVegetation::Reset(){
	if( pSearchRadius < 0.001f ){
		pDistance = 1.0f;
		pDirection.Set( 0.0f, 0.0f, 1.0f );
		pDirty = false;
		
	}else{
		pDistance = pSearchRadius;
		pDirection.Set( 0.0f, 0.0f, 1.0f );
		pDirty = true;
	}
}

float meHTVRuleClosestVegetation::GetOutputSlotValueAt( int slot, meHTVEvaluationEnvironment &evalEnv ){
	if( slot < 0 || slot > 1 ) DETHROW( deeInvalidParam );
	
	UpdateResult( evalEnv );
	
	if( slot == eosDistance ){
		return pDistance;
		
	}else{ // slot == eosDirection // invalid usage. in this case we return the y coordinate
		return pDirection.y;
	}
}

decVector meHTVRuleClosestVegetation::GetOutputSlotVectorAt( int slot, meHTVEvaluationEnvironment &evalEnv ){
	if( slot < 0 || slot > 1 ) DETHROW( deeInvalidParam );
	
	UpdateResult( evalEnv );
	
	if( slot == eosDistance ){
		return decVector( pDistance, pDistance, pDistance );
		
	}else{ // slot == eosDirection
		return pDirection;
	}
}

meHTVRule *meHTVRuleClosestVegetation::Copy() const{
	return new meHTVRuleClosestVegetation( *this );
}
