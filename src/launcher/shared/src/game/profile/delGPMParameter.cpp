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

#include "delGPMParameter.h"

#include <dragengine/common/exceptions.h>



// Class delGPMParameter
///////////////////////////

// Constructors and Destructors
/////////////////////////////////

delGPMParameter::delGPMParameter( const char *name, const char *value ) :
pName( name ),
pValue( value ){
}

delGPMParameter::delGPMParameter( const delGPMParameter &parameter ) :
pName( parameter.pName ),
pValue( parameter.pValue ){
}

delGPMParameter::~delGPMParameter(){
}



// Management
///////////////

void delGPMParameter::SetName( const char *name ){
	pName = name;
}

void delGPMParameter::SetValue( const char *value ){
	pValue = value;
}



// Operators
//////////////

delGPMParameter &delGPMParameter::operator=( const delGPMParameter& parameter ){
	pName = parameter.pName;
	pValue = parameter.pValue;
	return *this;
}
