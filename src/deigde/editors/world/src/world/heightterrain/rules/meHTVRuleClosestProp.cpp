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

#include "meHTVRuleClosestProp.h"
#include "meHTVRLink.h"
#include "meHTVRSlot.h"
#include "../../meWorld.h"
#include "../../object/meObject.h"
#include "../meHTVEvaluationEnvironment.h"

#include <dragengine/common/exceptions.h>



// Class meHTVRuleClosestProp
///////////////////////////////

// Constructor, destructor
////////////////////////////

meHTVRuleClosestProp::meHTVRuleClosestProp() : meHTVRule( ertClosestProp, 2 ){
	pSearchRadius = 1.0f;
	
	GetSlotAt( eosDirection ).SetIsInput( false );
	GetSlotAt( eosDistance ).SetIsInput( false );
	
	Reset();
}

meHTVRuleClosestProp::meHTVRuleClosestProp( const meHTVRuleClosestProp &rule ) :
meHTVRule( rule ),
pPropClass( rule.pPropClass ),
pSearchRadius( rule.pSearchRadius ),
pDistance( rule.pDistance ),
pDirection( rule.pDirection ),
pDirty( false ){
}

meHTVRuleClosestProp::~meHTVRuleClosestProp(){
}



// Management
///////////////

void meHTVRuleClosestProp::SetPropClass( const char *propClass ){
	if( ! propClass ) DETHROW( deeInvalidParam );
	
	pPropClass = propClass;
	
	Reset();
}

void meHTVRuleClosestProp::SetSearchRadius( float searchRadius ){
	pSearchRadius = searchRadius;
	
	Reset();
}

void meHTVRuleClosestProp::UpdateResult( meHTVEvaluationEnvironment &evalEnv ){
	if( pDirty ){
		const decDVector &ipos = evalEnv.GetPosition();
		int o, objectCount = evalEnv.GetObjectCount();
		meObject *object, *bestObject = NULL;
		decVector direction, bestDireciton;
		float distance, bestDistance = 0;
		
		for( o=0; o<objectCount; o++ ){
			object = evalEnv.GetObjectAt( o );
			
			if( pPropClass.Equals( object->GetClassName() ) ){
				direction = ( object->GetPosition() - ipos ).ToVector();
				distance = direction.Length();
				if( distance <= pSearchRadius ){
					if( ! bestObject || distance < bestDistance ){
						bestObject = object;
						bestDistance = distance;
						bestDireciton = direction;
					}
				}
			}
		}
		
		if( bestObject ){
			pDistance = bestDistance;
			if( bestDistance == 0.0f ){
				pDirection.Set( 0.0f, 1.0f, 0.0f );
				
			}else{
				pDirection = bestDireciton / bestDistance;
			}
			
		}else{
			pDistance = pSearchRadius;
			pDirection.Set( 0.0f, 1.0f, 0.0f );
		}
		
		// no more dirty
		pDirty = false;
	}
}



void meHTVRuleClosestProp::Reset(){
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

float meHTVRuleClosestProp::GetOutputSlotValueAt( int slot, meHTVEvaluationEnvironment &evalEnv ){
	if( slot < 0 || slot > 1 ) DETHROW( deeInvalidParam );
	
	UpdateResult( evalEnv );
	
	if( slot == eosDistance ){
		return pDistance;
		
	}else{ // slot == eosDirection // invalid usage. in this case we return the y coordinate
		return pDirection.y;
	}
}

decVector meHTVRuleClosestProp::GetOutputSlotVectorAt( int slot, meHTVEvaluationEnvironment &evalEnv ){
	if( slot < 0 || slot > 1 ) DETHROW( deeInvalidParam );
	
	UpdateResult( evalEnv );
	
	if( slot == eosDistance ){
		return decVector( pDistance, pDistance, pDistance );
		
	}else{ // slot == eosDirection
		return pDirection;
	}
}

meHTVRule *meHTVRuleClosestProp::Copy() const{
	return new meHTVRuleClosestProp( *this );
}
