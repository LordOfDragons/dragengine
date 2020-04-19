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

#include "debnValueInteger.h"
#include "../deNetworkBasic.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/resources/network/deNetworkMessage.h>
#include <dragengine/resources/network/value/deNetworkValueInteger.h>



// Class debnValueInteger
///////////////////////////

// Constructor, destructor
////////////////////////////

debnValueInteger::debnValueInteger( deNetworkValueInteger &valueInt ) :
pValueInt( valueInt ),
pLastValue( valueInt.GetInt() )
{
	switch( valueInt.GetFormat() ){
	case deNetworkValue::evfSInt8:
		SetDataType( evtSInt8 );
		break;
		
	case deNetworkValue::evfUInt8:
		SetDataType( evtUInt8 );
		break;
		
	case deNetworkValue::evfSInt16:
		SetDataType( evtSInt16 );
		break;
		
	case deNetworkValue::evfUInt16:
		SetDataType( evtUInt16 );
		break;
		
	case deNetworkValue::evfSInt32:
		SetDataType( evtSInt32 );
		break;
		
	case deNetworkValue::evfUInt32:
		SetDataType( evtUInt32 );
		break;
		
	case deNetworkValue::evfSInt64:
		SetDataType( evtSInt64 );
		break;
		
	case deNetworkValue::evfUInt64:
		SetDataType( evtUInt64 );
		break;
		
	default:
		DETHROW( deeInvalidParam );
	}
}

debnValueInteger::~debnValueInteger(){
}



// Management
///////////////

bool debnValueInteger::UpdateValue( bool force ){
	if( ! force && pValueInt.GetInt() == pLastValue ){
		return false;
	}
	
	pLastValue = pValueInt.GetInt();
	return true;
}

void debnValueInteger::ReadValue( decBaseFileReader &reader ){
	switch( GetDataType() ){
	case evtSInt8:
		pLastValue = reader.ReadChar();
		break;
		
	case evtUInt8:
		pLastValue = reader.ReadByte();
		break;
		
	case evtSInt16:
		pLastValue = reader.ReadShort();
		break;
		
	case evtUInt16:
		pLastValue = reader.ReadUShort();
		break;
		
	case evtSInt32:
		pLastValue = reader.ReadInt();
		break;
		
	case evtUInt32:
		pLastValue = reader.ReadUInt();
		break;
		
	case evtSInt64:
		pLastValue = reader.ReadLong();
		break;
		
	case evtUInt64:
		pLastValue = reader.ReadULong();
		break;
		
	default:
		return;
	}
	
	pValueInt.SetInt( pLastValue );
}

void debnValueInteger::WriteValue( decBaseFileWriter &writer ){
	switch( GetDataType() ){
	case evtSInt8:
		writer.WriteChar( ( int8_t )pValueInt.GetInt() );
		break;
		
	case evtUInt8:
		writer.WriteByte( ( uint8_t )pValueInt.GetInt() );
		break;
		
	case evtSInt16:
		writer.WriteShort( ( int16_t )pValueInt.GetInt() );
		break;
		
	case evtUInt16:
		writer.WriteUShort( ( uint16_t )pValueInt.GetInt() );
		break;
		
	case evtSInt32:
		writer.WriteInt( ( int32_t )pValueInt.GetInt() );
		break;
		
	case evtUInt32:
		writer.WriteUInt( ( uint32_t )pValueInt.GetInt() );
		break;
		
	case evtSInt64:
		writer.WriteLong( ( int64_t )pValueInt.GetInt() );
		break;
		
	case evtUInt64:
		writer.WriteULong( ( uint64_t )pValueInt.GetInt() );
		break;
		
	default:
		break;
	}
}
