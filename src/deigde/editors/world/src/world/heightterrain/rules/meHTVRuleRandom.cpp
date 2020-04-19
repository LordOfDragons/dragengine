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

#include "meHTVRuleRandom.h"
#include "meHTVRLink.h"
#include "meHTVRSlot.h"
#include "../meHTVEvaluationEnvironment.h"

#include <dragengine/common/exceptions.h>


#ifdef OS_W32
#define random rand
#endif


// Class meHTVRuleRandom
///////////////////////////

// Constructor, destructor
////////////////////////////

meHTVRuleRandom::meHTVRuleRandom() : meHTVRule( ertRandom, 1 ){
	GetSlotAt( eosRandom ).SetIsInput( false );
}

meHTVRuleRandom::meHTVRuleRandom( const meHTVRuleRandom &rule ) :
meHTVRule( rule ),
pRandom( rule.pRandom ){
}

meHTVRuleRandom::~meHTVRuleRandom(){
}



// Management
///////////////

void meHTVRuleRandom::Reset(){
	pRandom = ( float )random() / ( float )RAND_MAX;
}

float meHTVRuleRandom::GetOutputSlotValueAt( int slot, meHTVEvaluationEnvironment &evalEnv ){
	if( slot != 0 ) DETHROW( deeInvalidParam );
	
	return pRandom;
}

decVector meHTVRuleRandom::GetOutputSlotVectorAt( int slot, meHTVEvaluationEnvironment &evalEnv ){
	if( slot != 0 ) DETHROW( deeInvalidParam );
	
	return decVector( pRandom, pRandom, pRandom );
}

meHTVRule *meHTVRuleRandom::Copy() const{
	return new meHTVRuleRandom( *this );
}
