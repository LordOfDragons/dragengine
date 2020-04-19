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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "debnValueString.h"
#include "../deNetworkBasic.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/resources/network/deNetworkMessage.h>
#include <dragengine/resources/network/value/deNetworkValueString.h>



// Class debnValueString
//////////////////////////

// Constructor, destructor
////////////////////////////

debnValueString::debnValueString( deNetworkValueString &valueString ) :
pValueString( valueString ),
pLastValue( valueString.GetString() )
{
	SetDataType( evtString );
}

debnValueString::~debnValueString(){
}



// Management
///////////////

bool debnValueString::UpdateValue( bool force ){
	if( ! force && pValueString.GetString() == pLastValue ){
		return false;
	}
	
	pLastValue = pValueString.GetString();
	return true;
}

void debnValueString::ReadValue( decBaseFileReader &reader ){
	pLastValue = reader.ReadString16();
	pValueString.SetString( pLastValue );
}

void debnValueString::WriteValue( decBaseFileWriter &writer ){
	writer.WriteString16( pValueString.GetString() );
}
