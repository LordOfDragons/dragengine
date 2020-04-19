/* 
 * Drag[en]gine Animator Module
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

#include "dearControllerStates.h"

#include <dragengine/resources/animator/deAnimator.h>
#include <dragengine/resources/animator/deAnimatorLink.h>
#include <dragengine/resources/animator/controller/deAnimatorController.h>
#include <dragengine/resources/animator/controller/deAnimatorControllerTarget.h>
#include <dragengine/common/exceptions.h>



// Class dearControllerStates
///////////////////////////////

// Constructor, destructor
////////////////////////////

dearControllerStates::dearControllerStates(){
	pStates = NULL;
	pStateCount = 0;
}

dearControllerStates::~dearControllerStates(){
	if( pStates ){
		delete [] pStates;
	}
}



// Management
///////////////

void dearControllerStates::SetStateCount( int count ){
	if( count != pStateCount ){
		if( pStates ){
			delete [] pStates;
			pStates = NULL;
		}
		pStateCount = 0;
		
		if( count > 0 ){
			pStates = new sState[ count ];
			if( ! pStates ) DETHROW( deeOutOfMemory );
		}
		pStateCount = count;
	}
}



float dearControllerStates::GetValueAt( int state ) const{
	if( state < 0 || state >= pStateCount ) DETHROW( deeInvalidParam );
	
	return pStates[ state ].value;
}

const decVector &dearControllerStates::GetVectorAt( int state ) const{
	if( state < 0 || state >= pStateCount ) DETHROW( deeInvalidParam );
	
	return pStates[ state ].vector;
}

void dearControllerStates::SetValueAt( int state, float value ){
	if( state < 0 || state >= pStateCount ) DETHROW( deeInvalidParam );
	
	pStates[ state ].value = value;
}

void dearControllerStates::SetVectorAt( int state, const decVector &vector ){
	if( state < 0 || state >= pStateCount ) DETHROW( deeInvalidParam );
	
	pStates[ state ].vector = vector;
}

void dearControllerStates::SetStateAt( int state, float value, const decVector &vector ){
	if( state < 0 || state >= pStateCount ) DETHROW( deeInvalidParam );
	
	pStates[ state ].value = value;
	pStates[ state ].vector = vector;
}

void dearControllerStates::ResetStateAt( int state ){
	if( state < 0 || state >= pStateCount ) DETHROW( deeInvalidParam );
	
	pStates[ state ].value = 0.0f;
	pStates[ state ].vector.SetZero();
}

void dearControllerStates::SetStateFrom( int destState, const dearControllerStates &sourceStates, int sourceState ){
	if( sourceState < 0 || sourceState >= sourceStates.pStateCount ){
		DETHROW( deeInvalidParam );
	}
	if( destState < 0 || destState >= pStateCount ){
		DETHROW( deeInvalidParam );
	}
	
	pStates[ destState ].value = sourceStates.pStates[ sourceState ].value;
	pStates[ destState ].vector = sourceStates.pStates[ sourceState ].vector;
}
