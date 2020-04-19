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

#include "meHTVRulePropCount.h"
#include "meHTVRLink.h"
#include "meHTVRSlot.h"
#include "../../meWorld.h"
#include "../../object/meObject.h"
#include "../meHTVEvaluationEnvironment.h"

#include <dragengine/common/exceptions.h>



// Class meHTVRulePropCount
///////////////////////////////

// Constructor, destructor
////////////////////////////

meHTVRulePropCount::meHTVRulePropCount() : meHTVRule( ertPropCount, 1 ){
	pSearchRadius = 1.0f;
	
	GetSlotAt( eosCount ).SetIsInput( false );
	
	Reset();
}

meHTVRulePropCount::meHTVRulePropCount( const meHTVRulePropCount &rule ) :
meHTVRule( rule ),
pPropClass( rule.pPropClass ),
pSearchRadius( rule.pSearchRadius ),
pCount( rule.pCount ),
pDirty( false ){
}

meHTVRulePropCount::~meHTVRulePropCount(){
}



// Management
///////////////

void meHTVRulePropCount::SetPropClass( const char *propClass ){
	if( ! propClass ) DETHROW( deeInvalidParam );
	
	pPropClass = propClass;
	
	Reset();
}

void meHTVRulePropCount::SetSearchRadius( float searchRadius ){
	pSearchRadius = searchRadius;
	
	Reset();
}

void meHTVRulePropCount::UpdateResult( meHTVEvaluationEnvironment &evalEnv ){
	if( pDirty ){
		const decDVector &ipos = evalEnv.GetPosition();
		int o, objectCount = evalEnv.GetObjectCount();
		meObject *object;
		int count = 0;
		
		for( o=0; o<objectCount; o++ ){
			object = evalEnv.GetObjectAt( o );
			
			if( pPropClass.Equals( object->GetClassName() ) ){
				if( ( float )( object->GetPosition() - ipos ).Length() <= pSearchRadius ){
					count++;
				}
			}
		}
		
		pCount = ( float )count;
		
		// no more dirty
		pDirty = false;
	}
}



void meHTVRulePropCount::Reset(){
	pCount = 0.0f;
	pDirty = true;
}

float meHTVRulePropCount::GetOutputSlotValueAt( int slot, meHTVEvaluationEnvironment &evalEnv ){
	if( slot != 0 ) DETHROW( deeInvalidParam );
	
	UpdateResult( evalEnv );
	
	return pCount;
}

decVector meHTVRulePropCount::GetOutputSlotVectorAt( int slot, meHTVEvaluationEnvironment &evalEnv ){
	if( slot != 0 ) DETHROW( deeInvalidParam );
	
	UpdateResult( evalEnv );
	
	return decVector( pCount, pCount, pCount );
}

meHTVRule *meHTVRulePropCount::Copy() const{
	return new meHTVRulePropCount( *this );
}
