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

#include "fbxPropertyInteger.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/systems/modules/deBaseModule.h>



// Class fbxPropertyInteger
//////////////////////////

// Constructor, destructor
////////////////////////////

fbxPropertyInteger::fbxPropertyInteger() :
fbxProperty(etInteger),
pValue(0){
}

fbxPropertyInteger::fbxPropertyInteger(decBaseFileReader &reader) :
fbxProperty(etInteger),
pValue(reader.ReadInt()){
}

fbxPropertyInteger::~fbxPropertyInteger(){
}



// Loading and Saving
///////////////////////

void fbxPropertyInteger::SetValue(int value){
	pValue = value;
}

fbxPropertyInteger &fbxPropertyInteger::CastInteger(){
	return *this;
}

bool fbxPropertyInteger::GetValueAsBool() const{
	return pValue;
}

int fbxPropertyInteger::GetValueAsInt() const{
	return pValue;
}

int64_t fbxPropertyInteger::GetValueAsLong() const{
	return pValue;
}

float fbxPropertyInteger::GetValueAsFloat() const{
	return (float)pValue;
}

double fbxPropertyInteger::GetValueAsDouble() const{
	return pValue;
}



void fbxPropertyInteger::Save(decBaseFileWriter &writer){
}

void fbxPropertyInteger::DebugPrintStructure(deBaseModule &module, const decString &prefix) const{
	module.LogInfoFormat("%sProperty Integer: %d", prefix.GetString(), pValue);
}
