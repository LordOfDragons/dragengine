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

#include "deoglPFrameRateLimit.h"
#include "../../deGraphicOpenGl.h"
#include "../../configuration/deoglConfiguration.h"

#include <dragengine/common/exceptions.h>



// Class deoglPFrameRateLimit
///////////////////////////////

// Constructor, destructor
////////////////////////////

deoglPFrameRateLimit::deoglPFrameRateLimit( deGraphicOpenGl &ogl ) : deoglParameterInt( ogl ){
	SetName( "frameRateLimit" );
	SetDescription( "Set frame rate limit of render thread. Effects only asynchronous rendering. "
		"The game frame rate is not affected.\n"
		"\n"
		"By default the parameter is set to 0 indicating to use the refresh rate of the display as "
		"frame limit for rendering. The game frame rate is decoupled and usually higher.\n"
		"\n"
		"Values larger 0 override the frame limiter. A value between 30 and 60 Hz is recommended. "
		"Smaller values (like 30) reduce GPU load allowing the graphic module to do optimizations "
		"in the spare time. Higher values rarely improve visual perception. Values beyond the "
		"display refresh rate are pointless since they are not displayed anymore swandering only "
		"valuable GPU time and heating it up for no gain." );
	SetCategory( ecExpert );
	SetDisplayName( "Frame Rate Limit" );
	SetDefaultValue( "0" );
}

deoglPFrameRateLimit::~deoglPFrameRateLimit(){
}



// Parameter Value
////////////////////

int deoglPFrameRateLimit::GetParameterInt(){
	return pOgl.GetConfiguration().GetFrameRateLimit();
}

void deoglPFrameRateLimit::SetParameterInt( int value ){
	pOgl.GetConfiguration().SetFrameRateLimit( value );
}
