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

#include "debnValuePoint3.h"
#include "../deNetworkBasic.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/resources/network/deNetworkMessage.h>
#include <dragengine/resources/network/value/deNetworkValuePoint3.h>



// Class debnValuePoint3
//////////////////////////

// Constructor, destructor
////////////////////////////

debnValuePoint3::debnValuePoint3(deNetworkValuePoint3 &valuePoint3) :
pValuePoint3(valuePoint3),
pLastValue(valuePoint3.GetPoint())
{
	switch(valuePoint3.GetFormat()){
	case deNetworkValue::evfSInt8:
		SetDataType(evtPoint3S8);
		break;
		
	case deNetworkValue::evfUInt8:
		SetDataType(evtPoint3U8);
		break;
		
	case deNetworkValue::evfSInt16:
		SetDataType(evtPoint3S16);
		break;
		
	case deNetworkValue::evfUInt16:
		SetDataType(evtPoint3U16);
		break;
		
	case deNetworkValue::evfSInt32:
		SetDataType(evtPoint3S32);
		break;
		
	case deNetworkValue::evfUInt32:
		SetDataType(evtPoint3U32);
		break;
		
	case deNetworkValue::evfSInt64:
		SetDataType(evtPoint3S64);
		break;
		
	case deNetworkValue::evfUInt64:
		SetDataType(evtPoint3U64);
		break;
		
	default:
		DETHROW(deeInvalidParam);
	}
}

debnValuePoint3::~debnValuePoint3(){
}



// Management
///////////////

bool debnValuePoint3::UpdateValue(bool force){
	if(!force && pValuePoint3.GetPoint() == pLastValue){
		return false;
	}
	
	pLastValue = pValuePoint3.GetPoint();
	return true;
}

void debnValuePoint3::ReadValue(decBaseFileReader &reader){
	switch(GetDataType()){
	case evtPoint3S8:
		pLastValue.x = reader.ReadChar();
		pLastValue.y = reader.ReadChar();
		pLastValue.z = reader.ReadChar();
		break;
		
	case evtPoint3U8:
		pLastValue.x = reader.ReadByte();
		pLastValue.y = reader.ReadByte();
		pLastValue.z = reader.ReadByte();
		break;
		
	case evtPoint3S16:
		pLastValue.x = reader.ReadShort();
		pLastValue.y = reader.ReadShort();
		pLastValue.z = reader.ReadShort();
		break;
		
	case evtPoint3U16:
		pLastValue.x = reader.ReadUShort();
		pLastValue.y = reader.ReadUShort();
		pLastValue.z = reader.ReadUShort();
		break;
		
	case evtPoint3S32:
		pLastValue.x = reader.ReadInt();
		pLastValue.y = reader.ReadInt();
		pLastValue.z = reader.ReadInt();
		break;
		
	case evtPoint3U32:
		pLastValue.x = reader.ReadUInt();
		pLastValue.y = reader.ReadUInt();
		pLastValue.z = reader.ReadUInt();
		break;
		
	case evtPoint3S64:
		pLastValue.x = (int)reader.ReadLong();
		pLastValue.y = (int)reader.ReadLong();
		pLastValue.z = (int)reader.ReadLong();
		break;
		
	case evtPoint3U64:
		pLastValue.x = (int)reader.ReadULong();
		pLastValue.y = (int)reader.ReadULong();
		pLastValue.z = (int)reader.ReadULong();
		break;
		
	default:
		return;
	}
	
	pValuePoint3.SetPoint(pLastValue);
}

void debnValuePoint3::WriteValue(decBaseFileWriter &writer){
	const decPoint3 &point = pValuePoint3.GetPoint();
	
	switch(GetDataType()){
	case evtPoint3S8:
		writer.WriteChar((int8_t)point.x);
		writer.WriteChar((int8_t)point.y);
		writer.WriteChar((int8_t)point.z);
		break;
		
	case evtPoint3U8:
		writer.WriteByte((uint8_t)point.x);
		writer.WriteByte((uint8_t)point.y);
		writer.WriteByte((uint8_t)point.z);
		break;
		
	case evtPoint3S16:
		writer.WriteShort((int16_t)point.x);
		writer.WriteShort((int16_t)point.y);
		writer.WriteShort((int16_t)point.z);
		break;
		
	case evtPoint3U16:
		writer.WriteUShort((uint16_t)point.x);
		writer.WriteUShort((uint16_t)point.y);
		writer.WriteUShort((uint16_t)point.z);
		break;
		
	case evtPoint3S32:
		writer.WriteInt((int32_t)point.x);
		writer.WriteInt((int32_t)point.y);
		writer.WriteInt((int32_t)point.z);
		break;
		
	case evtPoint3U32:
		writer.WriteUInt((uint32_t)point.x);
		writer.WriteUInt((uint32_t)point.y);
		writer.WriteUInt((uint32_t)point.z);
		break;
		
	case evtPoint3S64:
		writer.WriteLong(point.x);
		writer.WriteLong(point.y);
		writer.WriteLong(point.z);
		break;
		
	case evtPoint3U64:
		writer.WriteULong(point.x);
		writer.WriteULong(point.y);
		writer.WriteULong(point.z);
		break;
		
	default:
		break;
	}
}
