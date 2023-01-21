/* 
 * Drag[en]gine OpenGL Graphic Module
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

#include "debnConfiguration.h"

#include <dragengine/dragengine_configuration.h>

#include <dragengine/common/exceptions.h>
#include <dragengine/common/math/decMath.h>



// Class debnConfiguration
////////////////////////////

// Constructor, destructor
////////////////////////////

debnConfiguration::debnConfiguration() :
pLogLevel( ellWarning ),

pConnectResendInterval( 1.0f ),
pConnectTimeout( 5.0f ),
pReliableResendInterval( 0.5f ),
pReliableTimeout( 3.0f ){
}

debnConfiguration::~debnConfiguration(){
}



// Management
///////////////

void debnConfiguration::SetLogLevel( eLogLevels level ){
	pLogLevel = level;
}

void debnConfiguration::SetConnectResendInterval( float interval ){
	pConnectResendInterval = decMath::max( interval, 0.01f );
}

void debnConfiguration::SetConnectTimeout( float timeout ){
	pConnectTimeout = decMath::max( timeout, 0.01f );
}

void debnConfiguration::SetReliableResendInterval( float interval ){
	pReliableResendInterval = decMath::max( interval, 0.01f );
}

void debnConfiguration::SetReliableTimeout( float timeout ){
	pReliableTimeout = decMath::max( timeout, 0.01f );
}
