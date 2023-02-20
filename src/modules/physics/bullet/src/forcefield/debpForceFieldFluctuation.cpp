/* 
 * Drag[en]gine Bullet Physics Module
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
#include <stdlib.h>

#include "debpForceFieldFluctuation.h"

#include <dragengine/common/exceptions.h>

#ifdef OS_W32
#define random rand
#endif // OS_W32


// Definitions
////////////////

static const float vRandomFactor = 1.0f / ( float )RAND_MAX;



// Class debpForceFieldFluctuation
////////////////////////////////////

// Constructor, destructor
////////////////////////////

debpForceFieldFluctuation::debpForceFieldFluctuation() :
pDirection( 0.0f ),
pStrength( 0.0f ),
pTimer( 0.0f ){
}

debpForceFieldFluctuation::~debpForceFieldFluctuation(){
}



// Management
///////////////

void debpForceFieldFluctuation::Reset(){
	pDirection = 0.0f;
	pStrength = 0.0f;
	pTimer = 0.0f;
}

void debpForceFieldFluctuation::Update( float elapsed ){
	pTimer += elapsed;
	
	while( pTimer > 0.04f ){
		pTimer -= 0.04f;
		pDirection = decMath::clamp( pDirection + ( ( float )random() * vRandomFactor * 2.0f - 1.0f ) * 0.4f, -1.0f, 1.0f );
		pStrength = decMath::clamp( pStrength + ( ( float )random() * vRandomFactor * 2.0f - 1.0f ) * 0.4f, -1.0f, 1.0f );
	}
}
