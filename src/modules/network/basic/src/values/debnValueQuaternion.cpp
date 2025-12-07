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

debnValueQuaternion::debnValueQuaternion(deNetworkValueQuaternion &valueQuat) :
pValueQuat(valueQuat),
pLastValue(valueQuat.GetQuaternion())
{
	switch(valueQuat.GetFormat()){
	case deNetworkValue::evfFloat16:
		SetDataType(evtQuaternionF16);
		break;
		
	case deNetworkValue::evfFloat32:
		SetDataType(evtQuaternionF32);
		break;
		
	case deNetworkValue::evfFloat64:
		SetDataType(evtQuaternionF64);
		break;
		
	default:
		DETHROW(deeInvalidParam);
	}
}

debnValueQuaternion::~debnValueQuaternion(){
}



// Management
///////////////

bool debnValueQuaternion::UpdateValue(bool force){
	if(! force && pValueQuat.GetQuaternion().IsEqualTo(pLastValue, (float)pValueQuat.GetPrecision())){
		return false;
	}
	
	pLastValue = pValueQuat.GetQuaternion();
	return true;
}

void debnValueQuaternion::ReadValue(decBaseFileReader &reader){
	switch(GetDataType()){
	case evtQuaternionF16:
		pLastValue.x = halfToFloat(reader.ReadUShort());
		pLastValue.y = halfToFloat(reader.ReadUShort());
		pLastValue.z = halfToFloat(reader.ReadUShort());
		pLastValue.w = halfToFloat(reader.ReadUShort());
		break;
		
	case evtQuaternionF32:
		pLastValue.x = reader.ReadFloat();
		pLastValue.y = reader.ReadFloat();
		pLastValue.z = reader.ReadFloat();
		pLastValue.w = reader.ReadFloat();
		break;
		
	case evtQuaternionF64:
		pLastValue.x = (float)reader.ReadDouble();
		pLastValue.y = (float)reader.ReadDouble();
		pLastValue.z = (float)reader.ReadDouble();
		pLastValue.w = (float)reader.ReadDouble();
		break;
		
	default:
		return;
	}
	
	pValueQuat.SetQuaternion(pLastValue);
}

void debnValueQuaternion::WriteValue(decBaseFileWriter &writer){
	const decQuaternion &quaternion = pValueQuat.GetQuaternion();
	
	switch(GetDataType()){
	case evtQuaternionF16:
		writer.WriteUShort(floatToHalf((float)quaternion.x));
		writer.WriteUShort(floatToHalf((float)quaternion.y));
		writer.WriteUShort(floatToHalf((float)quaternion.z));
		writer.WriteUShort(floatToHalf((float)quaternion.w));
		break;
		
	case evtQuaternionF32:
		writer.WriteFloat((float)quaternion.x);
		writer.WriteFloat((float)quaternion.y);
		writer.WriteFloat((float)quaternion.z);
		writer.WriteFloat((float)quaternion.w);
		break;
		
	case evtQuaternionF64:
		writer.WriteDouble(quaternion.x);
		writer.WriteDouble(quaternion.y);
		writer.WriteDouble(quaternion.z);
		writer.WriteDouble(quaternion.w);
		break;
		
	default:
		break;
	}
}
