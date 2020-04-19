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

#include "debnValueVector2.h"
#include "../deNetworkBasic.h"
#include "../half/half.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/resources/network/deNetworkMessage.h>
#include <dragengine/resources/network/value/deNetworkValueVector2.h>



// Class debnValueVector2
///////////////////////////

// Constructor, destructor
////////////////////////////

debnValueVector2::debnValueVector2( deNetworkValueVector2 &valueVector2 ) :
pValueVector2( valueVector2 ),
pLastValue( valueVector2.GetVector() )
{
	switch( valueVector2.GetFormat() ){
	case deNetworkValue::evfFloat16:
		SetDataType( evtVector2F16 );
		break;
		
	case deNetworkValue::evfFloat32:
		SetDataType( evtVector2F32 );
		break;
		
	case deNetworkValue::evfFloat64:
		SetDataType( evtVector2F64 );
		break;
		
	default:
		DETHROW( deeInvalidParam );
	}
}

debnValueVector2::~debnValueVector2(){
}



// Management
///////////////

bool debnValueVector2::UpdateValue( bool force ){
	if( ! force && pValueVector2.GetVector().IsEqualTo( pLastValue, pValueVector2.GetPrecision() ) ){
		return false;
	}
	
	pLastValue = pValueVector2.GetVector();
	return true;
}

void debnValueVector2::ReadValue( decBaseFileReader &reader ){
	switch( GetDataType() ){
	case evtVector2F16:
		pLastValue.x = halfToFloat( reader.ReadUShort() );
		pLastValue.y = halfToFloat( reader.ReadUShort() );
		break;
		
	case evtVector2F32:
		pLastValue.x = reader.ReadFloat();
		pLastValue.y = reader.ReadFloat();
		break;
		
	case evtVector2F64:
		pLastValue.x = reader.ReadDouble();
		pLastValue.y = reader.ReadDouble();
		break;
		
	default:
		return;
	}
	
	pValueVector2.SetVector( pLastValue );
}

void debnValueVector2::WriteValue( decBaseFileWriter &writer ){
	const decVector2 &vector = pValueVector2.GetVector();
	
	switch( GetDataType() ){
	case evtVector2F16:
		writer.WriteUShort( floatToHalf( ( float )vector.x ) );
		writer.WriteUShort( floatToHalf( ( float )vector.y ) );
		break;
		
	case evtVector2F32:
		writer.WriteFloat( ( float )vector.x );
		writer.WriteFloat( ( float )vector.y );
		break;
		
	case evtVector2F64:
		writer.WriteDouble( vector.x );
		writer.WriteDouble( vector.y );
		break;
		
	default:
		break;
	}
}
