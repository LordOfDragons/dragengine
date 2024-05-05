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

#include "meHTVRule.h"
#include "meHTVRLink.h"
#include "meHTVRSlot.h"

#include <dragengine/common/exceptions.h>



// Class meHTVRule
////////////////////

// Constructor, destructor
////////////////////////////

meHTVRule::meHTVRule( int type, int slotCount ){
	if( slotCount < 0 || type < 0 || type >= ERT_COUNT ){
		DETHROW( deeInvalidParam );
	}
	
	pType = type;
	pShowParameters = true;
	
	pSlotCount = 0;
	pSlots = NULL;
	
	try{
		if( slotCount > 0 ){
			pSlots = new meHTVRSlot[ slotCount ];
			pSlotCount = slotCount;
		}
		
	}catch( const deException & ){
		if( pSlots ){
			delete [] pSlots;
		}
		throw;
	}
}

meHTVRule::meHTVRule( const meHTVRule &rule ) :
pName( rule.pName ),
pType( rule.pType ),
pPosition( rule.pPosition ),
pShowParameters( rule.pShowParameters ),
pSlotCount( 0 ),
pSlots( NULL )
{
	if( rule.pSlotCount == 0 ){
		return;
	}
	
	try{
		pSlots = new meHTVRSlot[ rule.pSlotCount ];
		for( pSlotCount=0; pSlotCount<rule.pSlotCount; pSlotCount++ ){
			pSlots[ pSlotCount ].SetIsInput( rule.pSlots[ pSlotCount ].GetIsInput() );
		}
		
	}catch( const deException & ){
		if( pSlots ){
			delete [] pSlots;
		}
		throw;
	}
}

meHTVRule::~meHTVRule(){
	if( pSlots ){
		delete [] pSlots;
	}
}



// Management
///////////////

void meHTVRule::SetName( const char *name ){
	if( ! name ) DETHROW( deeInvalidParam );
	
	pName = name;
}

void meHTVRule::SetPosition( const decVector2 &position ){
	pPosition = position;
}

void meHTVRule::SetShowParameters( bool showParameters ){
	pShowParameters = showParameters;
}

meHTVRSlot &meHTVRule::GetSlotAt( int slot ) const{
	if( slot < 0 || slot >= pSlotCount ){
		DETHROW( deeInvalidParam );
	}
	return pSlots[ slot ];
}

bool meHTVRule::DependsOn( meHTVRule *rule ) const{
	if( ! rule ) DETHROW( deeInvalidParam );
	
	// by definition we depend on ourself
	if( rule == this ) return true;
	
	// otherwise test all input slot nodes for dependance
	meHTVRule *linkRule;
	int s, l, linkCount;
	
	for( s=0; s<pSlotCount; s++ ){
		const meHTVRSlot &slot = pSlots[ s ];
		if( ! slot.GetIsInput() ){
			continue;
		}
		
		linkCount = slot.GetLinkCount();
		
		for( l=0; l<linkCount; l++ ){
			const meHTVRLink &link = *slot.GetLinkAt( l );
			linkRule = link.GetSourceRule();
			
			if( linkRule->DependsOn( rule ) ){
				return true;
			}
		}
	}
	
	// not dependent on the given rule
	return false;
}



void meHTVRule::Reset(){
}

void meHTVRule::Evaluate( meHTVEvaluationEnvironment &evalEnv ){
}

float meHTVRule::GetOutputSlotValueAt( int slot, meHTVEvaluationEnvironment &evalEnv ){
	DETHROW( deeInvalidParam );
}

decVector meHTVRule::GetOutputSlotVectorAt( int slot, meHTVEvaluationEnvironment &evalEnv ){
	DETHROW( deeInvalidParam );
}
