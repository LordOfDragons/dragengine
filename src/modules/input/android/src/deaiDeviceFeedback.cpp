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

#include <stdlib.h>

#include "deaiDeviceFeedback.h"

#include <dragengine/common/exceptions.h>



// Class deaiDeviceFeedback
//////////////////////////////

// Constructor, destructor
////////////////////////////

deaiDeviceFeedback::deaiDeviceFeedback() :
pIndex( -1 ),
pType( deInputDeviceFeedback::eftGeneric ),
pMaximum( 1 ),
pValue( 0.0f ),
pEvdevCode( 0 ){
}

deaiDeviceFeedback::~deaiDeviceFeedback(){
}



// Management
///////////////

void deaiDeviceFeedback::SetIndex( int index ){
	pIndex = index;
}

void deaiDeviceFeedback::SetID( const char *id ){
	pID = id;
}

void deaiDeviceFeedback::SetName( const char *name ){
	pName = name;
}

void deaiDeviceFeedback::SetType( deInputDeviceFeedback::eFeedbackTypes type ){
	pType = type;
}



void deaiDeviceFeedback::SetMaximum( int maximum ){
	pMaximum = maximum;
}



void deaiDeviceFeedback::SetValue( float value ){
	pValue = value;
}



void deaiDeviceFeedback::SetEvdevCode( int code ){
	pEvdevCode = code;
}



void deaiDeviceFeedback::GetInfo( deInputDeviceFeedback &info ) const{
	info.SetID( pID );
	info.SetName( pName );
	info.SetType( pType );
}
