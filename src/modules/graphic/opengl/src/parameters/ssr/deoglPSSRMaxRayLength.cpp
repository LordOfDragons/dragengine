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

#include "deoglPSSRMaxRayLength.h"
#include "../../deGraphicOpenGl.h"
#include "../../configuration/deoglConfiguration.h"

#include <dragengine/common/exceptions.h>



// Class deoglPSSRMaxRayLength
////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglPSSRMaxRayLength::deoglPSSRMaxRayLength( deGraphicOpenGl &ogl ) :
deoglParameterFloat( ogl )
{
	SetName( "ssrMaxRayLength" );
	SetDescription( "Sets the maximum length of reflection rays in screen space reflections"
		" as percentage of the screen size. The largest screen size (width or height) is"
		" multiplied with this factor to determine the maximum ray length in pixels."
		" Limiting the maximum ray length may improve the ray sampling quality without"
		" impacting rendering speed. A value of 0.5 limits the ray length to at most 50%"
		" of the screen thus sampling step wide is smaller compared to the ray at 100%"
		" length with the same ssrStepCount. As downside this reduces though the distance"
		" of local reflections. This is more an esthetic tuning value to balance local"
		" reflections versus global reflections. For rendering speed improvements better"
		" use the ssrStepCount parameter." );
	SetCategory( ecExpert );
	SetDisplayName( "SSR Max Ray Length" );
	SetDefaultValue( "1" );
}

deoglPSSRMaxRayLength::~deoglPSSRMaxRayLength(){
}



// Parameter Value
////////////////////

float deoglPSSRMaxRayLength::GetParameterFloat(){
	return pOgl.GetConfiguration().GetSSRMaxRayLength();
}

void deoglPSSRMaxRayLength::SetParameterFloat( float value ){
	pOgl.GetConfiguration().SetSSRMaxRayLength( value );
}
