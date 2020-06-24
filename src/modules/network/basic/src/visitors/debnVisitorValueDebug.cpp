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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define __STDC_FORMAT_MACROS
#include <inttypes.h>

#include "debnVisitorValueDebug.h"
#include "../deNetworkBasic.h"

#include <dragengine/resources/network/value/deNetworkValue.h>
#include <dragengine/resources/network/value/deNetworkValueInteger.h>
#include <dragengine/resources/network/value/deNetworkValueFloat.h>
#include <dragengine/resources/network/value/deNetworkValueString.h>
#include <dragengine/resources/network/value/deNetworkValueData.h>
#include <dragengine/resources/network/value/deNetworkValuePoint2.h>
#include <dragengine/resources/network/value/deNetworkValuePoint3.h>
#include <dragengine/resources/network/value/deNetworkValueVector2.h>
#include <dragengine/resources/network/value/deNetworkValueVector3.h>
#include <dragengine/resources/network/value/deNetworkValueQuaternion.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/string/decString.h>



// Class debnVisitorValueDebug
////////////////////////////////

// Constructor, destructor
////////////////////////////

debnVisitorValueDebug::debnVisitorValueDebug( deNetworkBasic *module ){
	pModule = module;
}

debnVisitorValueDebug::~debnVisitorValueDebug(){
}



// Management
///////////////



// Visiting
/////////////

void debnVisitorValueDebug::VisitValue( deNetworkValue* ){
	DETHROW( deeInvalidParam );
}


void debnVisitorValueDebug::VisitInteger( deNetworkValueInteger *value ){
#if defined OS_W32 && ! defined PRId16
	// mingw bug: PRId64 not defined
	#define PRId16 "I64u"
#endif
	pModule->LogInfoFormat( "- Integer: %" PRId64, value->GetInt() );
}

void debnVisitorValueDebug::VisitFloat( deNetworkValueFloat *value ){
	pModule->LogInfoFormat( "- Float: %g", value->GetFloat() );
}

void debnVisitorValueDebug::VisitString( deNetworkValueString *value ){
	pModule->LogInfoFormat( "- String: '%s'", value->GetString().GetString() );
}

void debnVisitorValueDebug::VisitData( deNetworkValueData *value ){
	int i, length = value->GetLength();
	uint8_t *data = value->GetData();
	decString text;
	
	text.AppendFormat( "- Data: %i bytes:", length );
	for( i=0; i<20; i++ ){
		if( i == length ) break;
		text.AppendFormat( " 0x%X(%c)", ( unsigned char )data[ i ], data[ i ] );
	}
	
	pModule->LogInfo( text.GetString() );
}

void debnVisitorValueDebug::VisitPoint2( deNetworkValuePoint2 *value ){
	const decPoint &point = value->GetPoint();
	
	pModule->LogInfoFormat( "- Point2: %i, %i", point.x, point.y );
}

void debnVisitorValueDebug::VisitPoint3( deNetworkValuePoint3 *value ){
	const decPoint3 &point = value->GetPoint();
	
	pModule->LogInfoFormat( "- Point3: %i, %i, %i", point.x, point.y, point.z );
}

void debnVisitorValueDebug::VisitVector2( deNetworkValueVector2 *value ){
	const decVector2 &vector = value->GetVector();
	
	pModule->LogInfoFormat( "- Vector2: %g, %g", vector.x, vector.y );
}

void debnVisitorValueDebug::VisitVector3( deNetworkValueVector3 *value ){
	const decDVector &vector = value->GetVector();
	
	pModule->LogInfoFormat( "- Vector3: %g, %g, %g", vector.x, vector.y, vector.z );
}

void debnVisitorValueDebug::VisitQuaternion( deNetworkValueQuaternion *value ){
	const decQuaternion &quaternion = value->GetQuaternion();
	
	pModule->LogInfoFormat( "- Quaternion: %g, %g, %g, %g", quaternion.x, quaternion.y, quaternion.z, quaternion.w );
}
