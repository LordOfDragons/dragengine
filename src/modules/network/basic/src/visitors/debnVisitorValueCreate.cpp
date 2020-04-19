/* 
 * Drag[en]gine Basic Network Module
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

// includes
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "debnVisitorValueCreate.h"
#include "../deNetworkBasic.h"
#include "../values/debnValue.h"
#include "../values/debnValueInteger.h"
#include "../values/debnValueFloat.h"
#include "../values/debnValueString.h"
#include "../values/debnValueData.h"
#include "../values/debnValuePoint2.h"
#include "../values/debnValuePoint3.h"
#include "../values/debnValueVector2.h"
#include "../values/debnValueVector3.h"
#include "../values/debnValueQuaternion.h"
#include "dragengine/resources/network/value/deNetworkValue.h"
#include "dragengine/resources/network/value/deNetworkValueInteger.h"
#include "dragengine/resources/network/value/deNetworkValueFloat.h"
#include "dragengine/resources/network/value/deNetworkValueString.h"
#include "dragengine/resources/network/value/deNetworkValueData.h"
#include "dragengine/resources/network/value/deNetworkValuePoint2.h"
#include "dragengine/resources/network/value/deNetworkValuePoint3.h"
#include "dragengine/resources/network/value/deNetworkValueVector2.h"
#include "dragengine/resources/network/value/deNetworkValueVector3.h"
#include "dragengine/resources/network/value/deNetworkValueQuaternion.h"
#include "dragengine/common/math/decMath.h"
#include "dragengine/common/exceptions.h"



// Class debnVisitorValueCreate
///////////////////////////////

// Constructor, destructor
////////////////////////////

debnVisitorValueCreate::debnVisitorValueCreate(){
	pValue = NULL;
}

debnVisitorValueCreate::~debnVisitorValueCreate(){
}



// Management
///////////////

void debnVisitorValueCreate::Reset(){
	pValue = NULL;
}



// Visiting
/////////////

void debnVisitorValueCreate::VisitValue( deNetworkValue* ){
	DETHROW( deeInvalidParam );
}

void debnVisitorValueCreate::VisitInteger( deNetworkValueInteger *value ){
	pValue = new debnValueInteger( *value );
}

void debnVisitorValueCreate::VisitFloat( deNetworkValueFloat *value ){
	pValue = new debnValueFloat( *value );
}

void debnVisitorValueCreate::VisitString( deNetworkValueString *value ){
	pValue = new debnValueString( *value );
}

void debnVisitorValueCreate::VisitData( deNetworkValueData *value ){
	pValue = new debnValueData( *value );
}

void debnVisitorValueCreate::VisitPoint2( deNetworkValuePoint2 *value ){
	pValue = new debnValuePoint2( *value );
}

void debnVisitorValueCreate::VisitPoint3( deNetworkValuePoint3 *value ){
	pValue = new debnValuePoint3( *value );
}

void debnVisitorValueCreate::VisitVector2( deNetworkValueVector2 *value ){
	pValue = new debnValueVector2( *value );
}

void debnVisitorValueCreate::VisitVector3( deNetworkValueVector3 *value ){
	pValue = new debnValueVector3( *value );
}

void debnVisitorValueCreate::VisitQuaternion( deNetworkValueQuaternion *value ){
	pValue = new debnValueQuaternion( *value );
}
