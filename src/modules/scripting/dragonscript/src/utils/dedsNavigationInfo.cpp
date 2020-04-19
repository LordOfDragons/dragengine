/* 
 * Drag[en]gine DragonScript Script Module
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

#include <libdscript/libdscript.h>

#include "dedsNavigationInfo.h"
#include "../deScriptingDragonScript.h"

#include <dragengine/deEngine.h>
#include <libdscript/exceptions.h>



// Class dedsNavigationInfo
/////////////////////////////

// Constructor, destructor
////////////////////////////

dedsNavigationInfo::dedsNavigationInfo( deScriptingDragonScript &ds ) :
pDS( ds ),

pCostType( 0 ),

pPathIndex( -1 ),
pPathFactor( 0.0f ){
}

dedsNavigationInfo::dedsNavigationInfo( const dedsNavigationInfo &info ) :
pDS( info.pDS ),

pPosition( info.pPosition ),
pCostType( info.pCostType ),

pPathIndex( info.pPathIndex ),
pPathFactor( info.pPathFactor ){
}

dedsNavigationInfo::~dedsNavigationInfo(){
}



// Management
///////////////

void dedsNavigationInfo::SetPosition( const decDVector &position ){
	pPosition = position;
}

void dedsNavigationInfo::SetCostType( int costType ){
	pCostType = costType;
}



void dedsNavigationInfo::SetPathIndex( int index ){
	if( index < -1 ){
		DSTHROW( dueInvalidParam );
	}
	
	pPathIndex = index;
}

void dedsNavigationInfo::SetPathFactor( float factor ){
	pPathFactor = decMath::clamp( factor, 0.0f, 1.0f );
}
