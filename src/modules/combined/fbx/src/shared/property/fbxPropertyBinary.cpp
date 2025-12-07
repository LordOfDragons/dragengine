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

#include "fbxPropertyBinary.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/systems/modules/deBaseModule.h>



// Class fbxPropertyBinary
//////////////////////////

// Constructor, destructor
////////////////////////////

fbxPropertyBinary::fbxPropertyBinary() :
fbxProperty(etBinary),
pValue(NULL),
pLength(0){
}

fbxPropertyBinary::fbxPropertyBinary(decBaseFileReader &reader) :
fbxProperty(etBinary),
pValue(NULL),
pLength(0)
{
	const int length = reader.ReadUInt();
	if(length > 0){
		pValue = new uint8_t[length];
		reader.Read(pValue, length);
	}
}

fbxPropertyBinary::~fbxPropertyBinary(){
	if(pValue){
		delete [] pValue;
	}
}



// Loading and Saving
///////////////////////

void fbxPropertyBinary::SetValue(const uint8_t *value, int length){
	if(length < 0 || (length > 0 && ! value)){
		DETHROW(deeInvalidParam);
	}
	
	if(pValue){
		delete [] pValue;
		pValue = NULL;
		pLength = 0;
	}
	
	if(length > 0){
		pValue = new uint8_t[length];
		memcpy(pValue, value, length);
		pLength = length;
	}
}

fbxPropertyBinary &fbxPropertyBinary::CastBinary(){
	return *this;
}



void fbxPropertyBinary::Save(decBaseFileWriter &writer){
}

void fbxPropertyBinary::DebugPrintStructure(deBaseModule &module, const decString &prefix) const{
	module.LogInfoFormat("%sProperty Binary: length %d", prefix.GetString(), pLength);
}
