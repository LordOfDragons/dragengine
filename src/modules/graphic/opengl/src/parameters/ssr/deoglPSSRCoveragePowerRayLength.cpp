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

#include "deoglPSSRCoveragePowerRayLength.h"
#include "../../deGraphicOpenGl.h"
#include "../../configuration/deoglConfiguration.h"

#include <dragengine/common/exceptions.h>



// Class deoglPSSRCoveragePowerRayLength
//////////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglPSSRCoveragePowerRayLength::deoglPSSRCoveragePowerRayLength( deGraphicOpenGl &ogl ) :
deoglParameterFloat( ogl )
{
	SetName( "ssrCoveragePowerRayLength" );
	SetDescription( "Sets the power for ray length coverage calculation in screen space reflection."
		" Many reflections can not be calculated properly in screen space."
		" Using ray length coverage the found results are faded out gradually the longer a ray is until it hits geometry."
		" This parameter sets the power to which the ray length coverage is raised."
		" A value of 1 results in a linear fading from no coverage to full coverage."
		" A value of 2 results in a quadratic fading from no coverage to full coverage."
		" Values larger than 2 result in fading function using x-raisedto-N keeping the coverage longer"
		" near full coverage before falling down to no coverage."
		" The default value is 8 which results fading out results rather close to the maximum ray length."
		" This can be used to adjust the fading function to your personal liking." );
	SetCategory( ecExpert );
	SetDisplayName( "SSR Coverage Power Ray Length" );
	SetDefaultValue( "8" );
}

deoglPSSRCoveragePowerRayLength::~deoglPSSRCoveragePowerRayLength(){
}



// Parameter Value
////////////////////

float deoglPSSRCoveragePowerRayLength::GetParameterFloat(){
	return pOgl.GetConfiguration().GetSSRCoveragePowerRayLength();
}

void deoglPSSRCoveragePowerRayLength::SetParameterFloat( float value ){
	pOgl.GetConfiguration().SetSSRCoveragePowerRayLength( value );
}
