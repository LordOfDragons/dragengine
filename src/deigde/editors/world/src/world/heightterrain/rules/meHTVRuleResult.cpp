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
