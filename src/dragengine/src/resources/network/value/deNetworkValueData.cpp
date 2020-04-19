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
#include <string.h>

#include "deNetworkValueData.h"
#include "deNetworkValueVisitor.h"
#include "../../../common/exceptions.h"



// Class deNetworkValueData
/////////////////////////////

// Constructor, destructor
////////////////////////////

deNetworkValueData::deNetworkValueData( int length ) :
pData( NULL ),
pLength( 0 )
{
	SetLength( length );
}

deNetworkValueData::~deNetworkValueData(){
	if( pData ){
		delete [] pData;
	}
}



// Management
///////////////

void deNetworkValueData::SetLength( int length ){
	if( length < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	if( length == pLength ){
		return;
	}
	
	if( pData ){
		delete [] pData;
		pData = NULL;
		pLength = 0;
	}
	
	if( length > 0 ){
		pData = new uint8_t[ length ];
		pLength = length;
	}
}



// Visiting
/////////////

void deNetworkValueData::Visit( deNetworkValueVisitor &visitor ){
	visitor.VisitData( this );
}
