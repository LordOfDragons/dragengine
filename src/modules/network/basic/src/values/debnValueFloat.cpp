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

#include "debnValueFloat.h"
#include "../deNetworkBasic.h"
#include "../half/half.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/resources/network/deNetworkMessage.h>
#include <dragengine/resources/network/value/deNetworkValueFloat.h>



// Class debnValueFloat
/////////////////////////

// Constructor, destructor
////////////////////////////

debnValueFloat::debnValueFloat( deNetworkValueFloat &valueFloat ) :
pValueFloat( valueFloat ),
pLastValue( valueFloat.GetFloat() )
{
	switch( valueFloat.GetFormat() ){
	case deNetworkValue::evfFloat16:
		SetDataType( evtFloat16 );
		break;
		
	case deNetworkValue::evfFloat32:
		SetDataType( evtFloat32 );
		break;
		
	case deNetworkValue::evfFloat64:
		SetDataType( evtFloat64 );
		break;
		
	default:
		DETHROW( deeInvalidParam );
	}
}

debnValueFloat::~debnValueFloat(){
}



// Management
///////////////

bool debnValueFloat::UpdateValue( bool force ){
	if( ! force && fabs( pValueFloat.GetFloat() - pLastValue ) <= pValueFloat.GetPrecision() ){
		return false;
	}
	
	pLastValue = pValueFloat.GetFloat();
	return true;
}

void debnValueFloat::ReadValue( decBaseFileReader &reader ){
	switch( GetDataType() ){
	case evtFloat16:
		pLastValue = halfToFloat( reader.ReadUShort() );
		break;
		
	case evtFloat32:
		pLastValue = reader.ReadFloat();
		break;
		
	case evtFloat64:
		pLastValue = reader.ReadDouble();
		break;
		
	default:
		return;
	}
	
	pValueFloat.SetFloat( pLastValue );
}

void debnValueFloat::WriteValue( decBaseFileWriter &writer ){
	switch( GetDataType() ){
	case evtFloat16:
		writer.WriteUShort( floatToHalf( ( float )pValueFloat.GetFloat() ) );
		break;
		
	case evtFloat32:
		writer.WriteFloat( ( float )pValueFloat.GetFloat() );
		break;
		
	case evtFloat64:
		writer.WriteDouble( pValueFloat.GetFloat() );
		break;
		
	default:
		break;
	}
}
