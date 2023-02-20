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

#include "debnValueQuaternion.h"
#include "../deNetworkBasic.h"
#include "../half/half.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/resources/network/deNetworkMessage.h>
#include <dragengine/resources/network/value/deNetworkValueQuaternion.h>



// Class debnValueQuaternion
//////////////////////////////

// Constructor, destructor
////////////////////////////

debnValueQuaternion::debnValueQuaternion( deNetworkValueQuaternion &valueQuat ) :
pValueQuat( valueQuat ),
pLastValue( valueQuat.GetQuaternion() )
{
	switch( valueQuat.GetFormat() ){
	case deNetworkValue::evfFloat16:
		SetDataType( evtQuaternionF16 );
		break;
		
	case deNetworkValue::evfFloat32:
		SetDataType( evtQuaternionF32 );
		break;
		
	case deNetworkValue::evfFloat64:
		SetDataType( evtQuaternionF64 );
		break;
		
	default:
		DETHROW( deeInvalidParam );
	}
}

debnValueQuaternion::~debnValueQuaternion(){
}



// Management
///////////////

bool debnValueQuaternion::UpdateValue( bool force ){
	if( ! force && pValueQuat.GetQuaternion().IsEqualTo( pLastValue, ( float )pValueQuat.GetPrecision() ) ){
		return false;
	}
	
	pLastValue = pValueQuat.GetQuaternion();
	return true;
}

void debnValueQuaternion::ReadValue( decBaseFileReader &reader ){
	switch( GetDataType() ){
	case evtQuaternionF16:
		pLastValue.x = halfToFloat( reader.ReadUShort() );
		pLastValue.y = halfToFloat( reader.ReadUShort() );
		pLastValue.z = halfToFloat( reader.ReadUShort() );
		pLastValue.w = halfToFloat( reader.ReadUShort() );
		break;
		
	case evtQuaternionF32:
		pLastValue.x = reader.ReadFloat();
		pLastValue.y = reader.ReadFloat();
		pLastValue.z = reader.ReadFloat();
		pLastValue.w = reader.ReadFloat();
		break;
		
	case evtQuaternionF64:
		pLastValue.x = ( float )reader.ReadDouble();
		pLastValue.y = ( float )reader.ReadDouble();
		pLastValue.z = ( float )reader.ReadDouble();
		pLastValue.w = ( float )reader.ReadDouble();
		break;
		
	default:
		return;
	}
	
	pValueQuat.SetQuaternion( pLastValue );
}

void debnValueQuaternion::WriteValue( decBaseFileWriter &writer ){
	const decQuaternion &quaternion = pValueQuat.GetQuaternion();
	
	switch( GetDataType() ){
	case evtQuaternionF16:
		writer.WriteUShort( floatToHalf( ( float )quaternion.x ) );
		writer.WriteUShort( floatToHalf( ( float )quaternion.y ) );
		writer.WriteUShort( floatToHalf( ( float )quaternion.z ) );
		writer.WriteUShort( floatToHalf( ( float )quaternion.w ) );
		break;
		
	case evtQuaternionF32:
		writer.WriteFloat( ( float )quaternion.x );
		writer.WriteFloat( ( float )quaternion.y );
		writer.WriteFloat( ( float )quaternion.z );
		writer.WriteFloat( ( float )quaternion.w );
		break;
		
	case evtQuaternionF64:
		writer.WriteDouble( quaternion.x );
		writer.WriteDouble( quaternion.y );
		writer.WriteDouble( quaternion.z );
		writer.WriteDouble( quaternion.w );
		break;
		
	default:
		break;
	}
}
