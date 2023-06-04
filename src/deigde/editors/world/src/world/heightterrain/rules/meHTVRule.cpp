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
