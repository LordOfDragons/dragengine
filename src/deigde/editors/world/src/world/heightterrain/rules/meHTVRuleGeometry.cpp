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

#include "meHTVRuleGeometry.h"
#include "meHTVRLink.h"
#include "meHTVRSlot.h"
#include "../meHTVEvaluationEnvironment.h"
#include "../../terrain/meHeightTerrainTexture.h"

#include <dragengine/common/exceptions.h>



// Class meHTVRuleGeometry
////////////////////////////

// Constructor, destructor
////////////////////////////

meHTVRuleGeometry::meHTVRuleGeometry() : meHTVRule( ertGeometry, 3 ){
	GetSlotAt( eosHeight ).SetIsInput( false );
	GetSlotAt( eosNormal ).SetIsInput( false );
	GetSlotAt( eosTerrainType ).SetIsInput( false );
}

meHTVRuleGeometry::meHTVRuleGeometry( const meHTVRuleGeometry &rule ) :
meHTVRule( rule ){
}

meHTVRuleGeometry::~meHTVRuleGeometry(){
}



// Management
///////////////

float meHTVRuleGeometry::GetOutputSlotValueAt( int slot, meHTVEvaluationEnvironment &evalEnv ){
	if( slot < 0 || slot > 2 ) DETHROW( deeInvalidParam );
	
	if( slot == eosHeight ){
		return ( float )evalEnv.GetPosition().y;
		
	}else if( slot == eosTerrainType ){
		meHeightTerrainTexture *texture = evalEnv.GetHTDominantTexture();
		
		if( texture ){
			return ( float )texture->GetTypeNumber();
		}
	}
	
	return 0.0f;
}

decVector meHTVRuleGeometry::GetOutputSlotVectorAt( int slot, meHTVEvaluationEnvironment &evalEnv ){
	if( slot < 0 || slot > 2 ) DETHROW( deeInvalidParam );
	
	if( slot == eosHeight ){
		float height = evalEnv.GetPosition().y;
		
		return decVector( height, height, height );
		
	}else if( slot == eosNormal ){
		return evalEnv.GetNormal();
		
	}else if( slot == eosTerrainType ){
		meHeightTerrainTexture *texture = evalEnv.GetHTDominantTexture();
		
		if( texture ){
			float typeNumber = ( float )texture->GetTypeNumber();
			
			return decVector( typeNumber, typeNumber, typeNumber );
		}
	}
	
	return decVector();
}

meHTVRule *meHTVRuleGeometry::Copy() const{
	return new meHTVRuleGeometry( *this );
}
