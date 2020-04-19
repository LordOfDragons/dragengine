/* 
 * Drag[en]gine Game Engine
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

#include "deNetworkValueVector3.h"
#include "deNetworkValueVisitor.h"
#include "../../../common/exceptions.h"



// Class deNetworkValueVector3
////////////////////////////////

// Constructor, destructor
////////////////////////////

deNetworkValueVector3::deNetworkValueVector3( eValueFormats format, const decDVector &value ) :
pFormat( format ),
pValue( value ),
pPrecision( 0.001f )
{
	switch( format ){
	case evfFloat16:
	case evfFloat32:
	case evfFloat64:
		break;
		
	default:
		DETHROW( deeInvalidParam );
	}
}

deNetworkValueVector3::~deNetworkValueVector3(){
}



// Management
///////////////

void deNetworkValueVector3::SetVector( const decDVector &value ){
	pValue = value;
}

void deNetworkValueVector3::SetPrecision( double precision ){
	pPrecision = decMath::max( precision, DOUBLE_SAFE_EPSILON );
}



// Visiting
/////////////

void deNetworkValueVector3::Visit( deNetworkValueVisitor &visitor ){
	visitor.VisitVector3( this );
}
