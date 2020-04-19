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

#include "debnValuePoint2.h"
#include "../deNetworkBasic.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/resources/network/deNetworkMessage.h>
#include <dragengine/resources/network/value/deNetworkValuePoint2.h>



// Class debnValuePoint2
//////////////////////////

// Constructor, destructor
////////////////////////////

debnValuePoint2::debnValuePoint2( deNetworkValuePoint2 &valuePoint2 ) :
pValuePoint2( valuePoint2 ),
pLastValue( valuePoint2.GetPoint() )
{
	switch( valuePoint2.GetFormat() ){
	case deNetworkValue::evfSInt8:
		SetDataType( evtPoint2S8 );
		break;
		
	case deNetworkValue::evfUInt8:
		SetDataType( evtPoint2U8 );
		break;
		
	case deNetworkValue::evfSInt16:
		SetDataType( evtPoint2S16 );
		break;
		
	case deNetworkValue::evfUInt16:
		SetDataType( evtPoint2U16 );
		break;
		
	case deNetworkValue::evfSInt32:
		SetDataType( evtPoint2S32 );
		break;
		
	case deNetworkValue::evfUInt32:
		SetDataType( evtPoint2U32 );
		break;
		
	case deNetworkValue::evfSInt64:
		SetDataType( evtPoint2S64 );
		break;
		
	case deNetworkValue::evfUInt64:
		SetDataType( evtPoint2U64 );
		break;
		
	default:
		DETHROW( deeInvalidParam );
	}
}

debnValuePoint2::~debnValuePoint2(){
}



// Management
///////////////

bool debnValuePoint2::UpdateValue( bool force ){
	if( ! force && pValuePoint2.GetPoint() == pLastValue ){
		return false;
	}
	
	pLastValue = pValuePoint2.GetPoint();
	return true;
}

void debnValuePoint2::ReadValue( decBaseFileReader &reader ){
	switch( GetDataType() ){
	case evtPoint2S8:
		pLastValue.x = reader.ReadChar();
		pLastValue.y = reader.ReadChar();
		break;
		
	case evtPoint2U8:
		pLastValue.x = reader.ReadByte();
		pLastValue.y = reader.ReadByte();
		break;
		
	case evtPoint2S16:
		pLastValue.x = reader.ReadShort();
		pLastValue.y = reader.ReadShort();
		break;
		
	case evtPoint2U16:
		pLastValue.x = reader.ReadUShort();
		pLastValue.y = reader.ReadUShort();
		break;
		
	case evtPoint2S32:
		pLastValue.x = reader.ReadInt();
		pLastValue.y = reader.ReadInt();
		break;
		
	case evtPoint2U32:
		pLastValue.x = reader.ReadUInt();
		pLastValue.y = reader.ReadUInt();
		break;
		
	case evtPoint2S64:
		pLastValue.x = ( int )reader.ReadLong();
		pLastValue.y = ( int )reader.ReadLong();
		break;
		
	case evtPoint2U64:
		pLastValue.x = ( int )reader.ReadULong();
		pLastValue.y = ( int )reader.ReadULong();
		break;
		
	default:
		return;
	}
	
	pValuePoint2.SetPoint( pLastValue );
}

void debnValuePoint2::WriteValue( decBaseFileWriter &writer ){
	const decPoint &point = pValuePoint2.GetPoint();
	
	switch( GetDataType() ){
	case evtPoint2S8:
		writer.WriteChar( ( int8_t )point.x );
		writer.WriteChar( ( int8_t )point.y );
		break;
		
	case evtPoint2U8:
		writer.WriteByte( ( uint8_t )point.x );
		writer.WriteByte( ( uint8_t )point.y );
		break;
		
	case evtPoint2S16:
		writer.WriteShort( ( int16_t )point.x );
		writer.WriteShort( ( int16_t )point.y );
		break;
		
	case evtPoint2U16:
		writer.WriteUShort( ( uint16_t )point.x );
		writer.WriteUShort( ( uint16_t )point.y );
		break;
		
	case evtPoint2S32:
		writer.WriteInt( ( int32_t )point.x );
		writer.WriteInt( ( int32_t )point.y );
		break;
		
	case evtPoint2U32:
		writer.WriteUInt( ( uint32_t )point.x );
		writer.WriteUInt( ( uint32_t )point.y );
		break;
		
	case evtPoint2S64:
		writer.WriteLong( point.x );
		writer.WriteLong( point.y );
		break;
		
	case evtPoint2U64:
		writer.WriteULong( point.x );
		writer.WriteULong( point.y );
		break;
		
	default:
		break;
	}
}
