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

#include "deNetworkValuePoint2.h"
#include "deNetworkValueVisitor.h"
#include "../../../common/exceptions.h"



// Class deNetworkValuePoint2
///////////////////////////////

// Constructor, destructor
////////////////////////////

deNetworkValuePoint2::deNetworkValuePoint2( eValueFormats format, const decPoint &value ) :
pFormat( format ),
pValue( value )
{
	switch( format ){
	case evfSInt8:
	case evfSInt16:
	case evfSInt32:
	case evfUInt8:
	case evfUInt16:
	case evfUInt32:
		break;
		
	default:
		DETHROW( deeInvalidParam );
	}
}

deNetworkValuePoint2::~deNetworkValuePoint2(){
}



// Management
///////////////

void deNetworkValuePoint2::SetPoint( const decPoint &value ){
	pValue = value;
}



// Visiting
/////////////

void deNetworkValuePoint2::Visit( deNetworkValueVisitor &visitor ){
	visitor.VisitPoint2( this );
}
