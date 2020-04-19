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

#include "debnValueVector3.h"
#include "../deNetworkBasic.h"
#include "../half/half.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/resources/network/deNetworkMessage.h>
#include <dragengine/resources/network/value/deNetworkValueVector3.h>



// Class debnValueVector3
///////////////////////////

// Constructor, destructor
////////////////////////////

debnValueVector3::debnValueVector3( deNetworkValueVector3 &valueVector3 ) :
pValueVector3( valueVector3 ),
pLastValue( valueVector3.GetVector() )
{
	switch( valueVector3.GetFormat() ){
	case deNetworkValue::evfFloat16:
		SetDataType( evtVector3F16 );
		break;
		
	case deNetworkValue::evfFloat32:
		SetDataType( evtVector3F32 );
		break;
		
	case deNetworkValue::evfFloat64:
		SetDataType( evtVector3F64 );
		break;
		
	default:
		DETHROW( deeInvalidParam );
	}
}

debnValueVector3::~debnValueVector3(){
}



// Management
///////////////

bool debnValueVector3::UpdateValue( bool force ){
	if( ! force && pValueVector3.GetVector().IsEqualTo( pLastValue, pValueVector3.GetPrecision() ) ){
		return false;
	}
	
	pLastValue = pValueVector3.GetVector();
	return true;
}

void debnValueVector3::ReadValue( decBaseFileReader &reader ){
	switch( GetDataType() ){
	case evtVector3F16:
		pLastValue.x = halfToFloat( reader.ReadUShort() );
		pLastValue.y = halfToFloat( reader.ReadUShort() );
		pLastValue.z = halfToFloat( reader.ReadUShort() );
		break;
		
	case evtVector3F32:
		pLastValue.x = reader.ReadFloat();
		pLastValue.y = reader.ReadFloat();
		pLastValue.z = reader.ReadFloat();
		break;
		
	case evtVector3F64:
		pLastValue.x = reader.ReadDouble();
		pLastValue.y = reader.ReadDouble();
		pLastValue.z = reader.ReadDouble();
		break;
		
	default:
		return;
	}
	
	pValueVector3.SetVector( pLastValue );
}

void debnValueVector3::WriteValue( decBaseFileWriter &writer ){
	const decDVector &vector = pValueVector3.GetVector();
	
	switch( GetDataType() ){
	case evtVector3F16:
		writer.WriteUShort( floatToHalf( ( float )vector.x ) );
		writer.WriteUShort( floatToHalf( ( float )vector.y ) );
		writer.WriteUShort( floatToHalf( ( float )vector.z ) );
		break;
		
	case evtVector3F32:
		writer.WriteFloat( ( float )vector.x );
		writer.WriteFloat( ( float )vector.y );
		writer.WriteFloat( ( float )vector.z );
		break;
		
	case evtVector3F64:
		writer.WriteDouble( vector.x );
		writer.WriteDouble( vector.y );
		writer.WriteDouble( vector.z );
		break;
		
	default:
		break;
	}
}
