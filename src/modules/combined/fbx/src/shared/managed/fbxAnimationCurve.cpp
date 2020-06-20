/* 
 * FBX Modules
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
#include <inttypes.h>

#include "fbxAnimationCurve.h"
#include "../fbxNode.h"
#include "../fbxProperty.h"

#include <dragengine/common/exceptions.h>


// Class fbxAnimationCurve
////////////////////////////

// Constructor, destructor
////////////////////////////

fbxAnimationCurve::fbxAnimationCurve( fbxNode &nodeCurve ) :
pNodeCurve( nodeCurve ),
pPropertyTime( NULL ),
pPropertyValue( NULL ){
	const fbxNode &nodeTime = *nodeCurve.FirstNodeNamed( "KeyTime" );
	pPropertyTime = nodeTime.GetPropertyAt( 0 );
	
	const fbxNode * const nodeValueTime = nodeCurve.FirstNodeNamed( "KeyValueFloat" );
	if( nodeValueTime ){
		pPropertyValue = nodeValueTime->GetPropertyAt( 0 );
	}
	
	if( ! pPropertyValue ){
		DETHROW_INFO( deeInvalidParam, "AnimationCurve missing KeyValueFloat" );
	}
	
	pDefaultValue.x = nodeCurve.GetPropertyFloat( "d|X", 0.0f );
	pDefaultValue.y = nodeCurve.GetPropertyFloat( "d|Y", 0.0f );
	pDefaultValue.z = nodeCurve.GetPropertyFloat( "d|Z", 0.0f );
}

fbxAnimationCurve::~fbxAnimationCurve(){
}



// Management
///////////////
