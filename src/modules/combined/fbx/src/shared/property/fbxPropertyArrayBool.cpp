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

#include "fbxPropertyArrayBool.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/systems/modules/deBaseModule.h>



// Class fbxPropertyArrayBool
///////////////////////////////

// Constructor, destructor
////////////////////////////

fbxPropertyArrayBool::fbxPropertyArrayBool() :
fbxProperty(etArrayBoolean){
}

fbxPropertyArrayBool::fbxPropertyArrayBool(decBaseFileReader &reader) :
fbxProperty(etArrayBoolean)
{
	const int count = reader.ReadUInt();
	
	decBaseFileReader::Ref valueReader;
	pGetValueReader(reader, valueReader);
	
	if(count == 0){
		return;
	}
	
	pValues.EnlargeCapacity(count);
	int i;
	for(i=0; i<count; i++){
		pValues.Add(valueReader->ReadByte() != 0);
	}
}

fbxPropertyArrayBool::~fbxPropertyArrayBool(){
}



// Loading and Saving
///////////////////////

fbxPropertyArrayBool &fbxPropertyArrayBool::CastArrayBool(){
	return *this;
}

int fbxPropertyArrayBool::GetValueCount() const{
	return pValues.GetCount();
}

bool fbxPropertyArrayBool::GetValueAtAsBool(int index) const{
	return pValues[index];
}

int fbxPropertyArrayBool::GetValueAtAsInt(int index) const{
	return pValues[index];
}

int64_t fbxPropertyArrayBool::GetValueAtAsLong(int index) const{
	return pValues[index];
}

float fbxPropertyArrayBool::GetValueAtAsFloat(int index) const{
	return pValues[index];
}

double fbxPropertyArrayBool::GetValueAtAsDouble(int index) const{
	return pValues[index];
}



void fbxPropertyArrayBool::Save(decBaseFileWriter &writer){
}

void fbxPropertyArrayBool::DebugPrintStructure(deBaseModule &module, const decString &prefix) const{
	module.LogInfoFormat("%sProperty Bool[%d]", prefix.GetString(), pValues.GetCount());
}
