/* 
 * FBX Model Module
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

#include "fbxConnection.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/systems/modules/deBaseModule.h>


// Class fbxConnection
////////////////////////

// Constructor, destructor
////////////////////////////

fbxConnection::fbxConnection( int64_t source, int64_t target, const char *property ) :
pSource( source ),
pTarget( target ),
pProperty( property ){
}

fbxConnection::~fbxConnection(){
}



// Management
///////////////

int64_t fbxConnection::OtherID( int64_t id ) const{
	if( id == pSource ){
		return pTarget;
		
	}else if( id == pTarget ){
		return pSource;
		
	}else{
		DETHROW( deeInvalidParam );
	}
}



void fbxConnection::DebugPrintStructure( deBaseModule &module, const decString &prefix ) const{
#if defined OS_W32 && ! defined PRId64
	// mingw bug: PRId64 not defined
	#define PRId64 "I64u"
#endif
	if( pProperty.IsEmpty() ){
		module.LogInfoFormat( "%sConnection: %" PRId64 " -> %" PRId64,
			prefix.GetString(), pSource, pTarget );
		
	}else{
		module.LogInfoFormat( "%sConnection: %" PRId64 " -> %" PRId64 " (%s)",
			prefix.GetString(), pSource, pTarget, pProperty.GetString() );
	}
}
