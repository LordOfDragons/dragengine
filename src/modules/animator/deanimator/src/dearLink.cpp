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

#include <math.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "dearLink.h"
#include "dearControllerStates.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/collection/decIntList.h>
#include <dragengine/resources/animator/deAnimatorLink.h>



// Class dearLink
///////////////////

// Constructors and Destructors
/////////////////////////////////

dearLink::dearLink( const deAnimatorLink &link, const decIntList &controllerMapping ) :
pLink( link ),
pEvaluator( link.GetCurve() )
{
	const int controller = pLink.GetController();
	if( controller == -1 ){
		return;
	}
	
	if( controller < controllerMapping.GetCount() ){
		pLink.SetController( controllerMapping.GetAt( controller ) );
		
	}else{
		pLink.SetController( -1 );
	}
}

dearLink::~dearLink(){
}



// Management
///////////////

bool dearLink::HasController() const{
	return pLink.HasController();
}

int dearLink::GetController() const{
	return pLink.GetController();
}



float dearLink::GetValue( const dearControllerStates &controllerStates, float defaultValue ) const{
	float value = defaultValue;
	
	if( pLink.HasController() ){
		float controllerValue = controllerStates.GetValueAt( pLink.GetController() );
		
		const int repeat = pLink.GetRepeat();
		if( repeat > 1 ){
			controllerValue *= ( float )repeat;
			controllerValue -= floorf( controllerValue );
		}
		
		if( pEvaluator.GetCurve().GetPointCount() == 0 ){
			value = controllerValue;
			
		}else{
			value = pEvaluator.EvaluateAt( controllerValue );
		}
	}
	
	return value;
}

void dearLink::GetVector( const dearControllerStates &controllerStates, decVector &vector ) const{
	if( pLink.HasController() ){
		vector = controllerStates.GetVectorAt( pLink.GetController() );
	}
}

void dearLink::GetQuaternion( const dearControllerStates &controllerStates, decQuaternion &quaternion ) const{
	if( pLink.HasController() ){
		quaternion.SetFromEuler( controllerStates.GetVectorAt( pLink.GetController() ) );
	}
}
