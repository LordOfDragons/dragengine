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
#include <string.h>

#include "debnValueData.h"
#include "../deNetworkBasic.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/resources/network/deNetworkMessage.h>
#include <dragengine/resources/network/value/deNetworkValueData.h>



// Class debnValueData
////////////////////////

// Constructor, destructor
////////////////////////////

debnValueData::debnValueData(deNetworkValueData &valueData) :
pValueData(valueData),
pLastValue(NULL),
pLastValueLen(0)
{
	SetDataType(evtData);
	
	if(valueData.GetLength() > 0){
		pLastValue = new uint8_t[valueData.GetLength()];
		memcpy(pLastValue, valueData.GetData(), valueData.GetLength());
		pLastValueLen = valueData.GetLength();
	}
}

debnValueData::~debnValueData(){
	if(pLastValue){
		delete [] pLastValue;
	}
}



// Management
///////////////

bool debnValueData::LastValueEqualsNetworkValue() const{
	if(pLastValueLen != pValueData.GetLength()){
		return false;
	}
	
	if(pLastValue){
		return pValueData.GetData() && memcmp(pLastValue, pValueData.GetData(), pLastValueLen) == 0;
		
	}else{
		return !pValueData.GetData();
	}
}

void debnValueData::SetLastValueFromNetworkValue(){
	if(pValueData.GetLength() > pLastValueLen){
		if(pLastValue){
			delete [] pLastValue;
			pLastValue = NULL;
			pLastValueLen = 0;
		}
		pLastValue = new uint8_t[pValueData.GetLength()];
	}
	
	pLastValueLen = pValueData.GetLength();
	memcpy(pLastValue, pValueData.GetData(), pLastValueLen);
}

bool debnValueData::UpdateValue(bool force){
	if(!force && LastValueEqualsNetworkValue()){
		return false;
	}
	
	SetLastValueFromNetworkValue();
	return true;
}

void debnValueData::ReadValue(decBaseFileReader &reader){
	pValueData.SetLength(reader.ReadUShort());
	reader.Read(pValueData.GetData(), pValueData.GetLength());
	SetLastValueFromNetworkValue();
}

void debnValueData::WriteValue(decBaseFileWriter &writer){
	writer.WriteUShort((uint16_t)pValueData.GetLength());
	writer.Write(pValueData.GetData(), pValueData.GetLength());
}
