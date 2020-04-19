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

#include "meHTVRuleResult.h"
#include "meHTVRLink.h"
#include "meHTVRSlot.h"
#include "../meHTVEvaluationEnvironment.h"
#include "../meHTVegetationLayer.h"



// Class meHTVRuleResult
//////////////////////////

// Constructor, destructor
////////////////////////////

meHTVRuleResult::meHTVRuleResult() :
meHTVRule( ertResult, 2 ),
pProbability( 0.0f ),
pVariation( 0 )
{
	GetSlotAt( eisProbability ).SetIsInput( true );
	GetSlotAt( eisVariation ).SetIsInput( true );
}

meHTVRuleResult::meHTVRuleResult( const meHTVRuleResult &rule ) :
meHTVRule( rule ),
pProbability( rule.pProbability ),
pVariation( rule.pVariation ){
}

meHTVRuleResult::~meHTVRuleResult(){
}



// Management
///////////////

void meHTVRuleResult::SetProbability( float probability ){
	pProbability = probability;
}

void meHTVRuleResult::SetVariation( int variation ){
	pVariation = variation;
}



void meHTVRuleResult::Evaluate( meHTVEvaluationEnvironment &evalEnv ){
	meHTVRSlot &slotPropbability = GetSlotAt( eisProbability );
	meHTVRSlot &slotVariation = GetSlotAt( eisVariation );
	
	// probability
	int linkCount = slotPropbability.GetLinkCount();
	float value;
	
	if( linkCount > 0 ){
		float probability = 1.0f;
		int l;
		
		for( l=0; l<linkCount; l++ ){
			meHTVRLink &link = *slotPropbability.GetLinkAt( l );
			
			value = link.GetSourceRule()->GetOutputSlotValueAt( link.GetSourceSlot(), evalEnv );
				
			if( value < 0.0f ){
				probability = 0.0f;
				
			}else if( value < 1.0f ){
				probability *= value;
			}
		}
		
		evalEnv.SetProbability( probability );
		
	}else{
		evalEnv.SetProbability( pProbability );
	}
	
	// variation
	if( slotVariation.GetLinkCount() > 0 ){
		meHTVRLink &link = *slotVariation.GetLinkAt( 0 );
		meHTVegetationLayer *vlayer = evalEnv.GetVLayer();
		int variationCount = 0;
		
		if( vlayer ){
			variationCount = vlayer->GetVariationCount();
		}
		
		value = link.GetSourceRule()->GetOutputSlotValueAt( link.GetSourceSlot(), evalEnv );
		
		evalEnv.SetVariation( ( int )floorf( value * ( float )variationCount + 0.5f ) );
		
	}else{
		evalEnv.SetVariation( pVariation );
	}
}

meHTVRule *meHTVRuleResult::Copy() const{
	return new meHTVRuleResult( *this );
}
