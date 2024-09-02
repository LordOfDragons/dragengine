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
		float height = ( float )evalEnv.GetPosition().y;
		
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
