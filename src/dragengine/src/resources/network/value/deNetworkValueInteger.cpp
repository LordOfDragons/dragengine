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

#include "deNetworkValueInteger.h"
#include "deNetworkValueVisitor.h"
#include "../../../common/exceptions.h"



// Class deNetworkValueInteger
////////////////////////////////

// Constructor, destructor
////////////////////////////

deNetworkValueInteger::deNetworkValueInteger( eValueFormats format, int64_t value ) :
pFormat( format ),
pValue( value )
{
	switch( format ){
	case evfSInt8:
	case evfSInt16:
	case evfSInt32:
	case evfSInt64:
	case evfUInt8:
	case evfUInt16:
	case evfUInt32:
	case evfUInt64:
		break;
		
	default:
		DETHROW( deeInvalidParam );
	}
}

deNetworkValueInteger::~deNetworkValueInteger(){
}



// Management
///////////////

void deNetworkValueInteger::SetInt( int64_t value ){
	pValue = value;
}



// Visiting
/////////////

void deNetworkValueInteger::Visit( deNetworkValueVisitor &visitor ){
	visitor.VisitInteger( this );
}
