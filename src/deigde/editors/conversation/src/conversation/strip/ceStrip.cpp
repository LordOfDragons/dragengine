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

#include "ceStrip.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/math/decMath.h>



// Class ceStrip
//////////////////

// Constructor, destructor
////////////////////////////

ceStrip::ceStrip(){
	pPause = 0.0f,
	pDuration = 1.0f;
}

ceStrip::ceStrip( const char *id, float duration, float pause ){
	if( ! id ){
		DETHROW( deeInvalidParam );
	}
	
	pID = id;
	pPause = pause,
	pDuration = duration;
}

ceStrip::ceStrip( const ceStrip& strip ){
	pID = strip.GetID();
	pPause = strip.GetPause();
	pDuration = strip.GetDuration();
}

ceStrip::~ceStrip(){
}



// Management
///////////////

void ceStrip::SetID( const char *id ){
	if( ! id ){
		DETHROW( deeInvalidParam );
	}
	pID = id;
}

void ceStrip::SetPause( float pause ){
	pPause = pause;
}

void ceStrip::SetDuration( float duration ){
	pDuration = duration;
}



// Operators
//////////////

ceStrip &ceStrip::operator=( const ceStrip &strip ){
	pID = strip.GetID();
	pPause = strip.GetPause();
	pDuration = strip.GetDuration();
	return *this;
}

bool ceStrip::operator==( const ceStrip &strip ) const{
	return pID.Equals( strip.GetID() )
		&& fabsf( pPause - strip.GetPause() ) < FLOAT_SAFE_EPSILON
		&& fabsf( pDuration - strip.GetDuration() ) < FLOAT_SAFE_EPSILON;
}

bool ceStrip::operator!=( const ceStrip &strip ) const{
	return ! ( *this == strip );
}
