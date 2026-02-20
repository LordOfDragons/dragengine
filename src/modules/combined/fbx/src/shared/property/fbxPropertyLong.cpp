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
#include <inttypes.h>

#include "fbxPropertyLong.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/systems/modules/deBaseModule.h>



// Class fbxPropertyLong
//////////////////////////

// Constructor, destructor
////////////////////////////

fbxPropertyLong::fbxPropertyLong() :
fbxProperty(etLong),
pValue(0){
}

fbxPropertyLong::fbxPropertyLong(decBaseFileReader &reader) :
fbxProperty(etLong),
pValue(reader.ReadLong()){
}

fbxPropertyLong::~fbxPropertyLong(){
}



// Loading and Saving
///////////////////////

void fbxPropertyLong::SetValue(int64_t value){
	pValue = value;
}

fbxPropertyLong &fbxPropertyLong::CastLong(){
	return *this;
}

bool fbxPropertyLong::GetValueAsBool() const{
	return pValue;
}

int fbxPropertyLong::GetValueAsInt() const{
	return (int)pValue;
}

int64_t fbxPropertyLong::GetValueAsLong() const{
	return pValue;
}

float fbxPropertyLong::GetValueAsFloat() const{
	return (float)pValue;
}

double fbxPropertyLong::GetValueAsDouble() const{
	return (double)pValue;
}



void fbxPropertyLong::Save(decBaseFileWriter &writer){
}

void fbxPropertyLong::DebugPrintStructure(deBaseModule &module, const decString &prefix) const{
#if defined __MINGW32__ || defined __MINGW64__
	#ifdef PRId64
		#undef PRId64
	#endif
	#define PRId64 "I64u"
#endif
	module.LogInfoFormat("%sProperty Long: %" PRId64, prefix.GetString(), pValue);
}
