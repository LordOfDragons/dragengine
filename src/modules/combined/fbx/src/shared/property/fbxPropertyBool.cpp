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

#include "fbxPropertyBool.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/systems/modules/deBaseModule.h>



// Class fbxPropertyBool
//////////////////////////

// Constructor, destructor
////////////////////////////

fbxPropertyBool::fbxPropertyBool() :
fbxProperty(etBoolean),
pValue(false){
}

fbxPropertyBool::fbxPropertyBool(decBaseFileReader &reader) :
fbxProperty(etBoolean),
pValue(reader.ReadChar() != 0){
}

fbxPropertyBool::~fbxPropertyBool(){
}



// Loading and Saving
///////////////////////

void fbxPropertyBool::SetValue(bool value){
	pValue = value;
}

fbxPropertyBool &fbxPropertyBool::CastBool(){
	return *this;
}

bool fbxPropertyBool::GetValueAsBool() const{
	return pValue;
}

int fbxPropertyBool::GetValueAsInt() const{
	return pValue;
}

int64_t fbxPropertyBool::GetValueAsLong() const{
	return pValue;
}

float fbxPropertyBool::GetValueAsFloat() const{
	return pValue;
}

double fbxPropertyBool::GetValueAsDouble() const{
	return pValue;
}



void fbxPropertyBool::Save(decBaseFileWriter &writer){
}

void fbxPropertyBool::DebugPrintStructure(deBaseModule &module, const decString &prefix) const{
	module.LogInfoFormat("%sProperty Bool: %s", prefix.GetString(), pValue ? "true" : "false");
}
