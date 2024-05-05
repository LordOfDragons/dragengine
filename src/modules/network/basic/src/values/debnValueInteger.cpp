/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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
