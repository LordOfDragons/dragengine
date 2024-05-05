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

#include "deoglPAOSelfShadowSmoothAngle.h"
#include "../../deGraphicOpenGl.h"
#include "../../configuration/deoglConfiguration.h"

#include <dragengine/common/exceptions.h>



// Class deoglPAOSelfShadowSmoothAngle
////////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglPAOSelfShadowSmoothAngle::deoglPAOSelfShadowSmoothAngle( deGraphicOpenGl &ogl ) :
deoglParameterFloat( ogl )
{
	SetName( "aoSelfShadowSmoothAngle" );
	SetDescription( "Sets the ambient occlusion self shadowing smoothing angle."
		" The self shadowing algorithm is by nature an on-off test."
		" The smoothing angle avoids the harsh step from shadows to unshadows areas by"
		" defining an angle range in degrees over which the shadow fades."
		" Typical values are in the range from 5 to 20 degrees."
		" The default value is 6 degrees."
		" The minimal value is 0.1 and the maximal value 90."
		" Values outside the range from 5 to 20 are though not recommended.\n"
		"\n"
		" 5 works best for small features like texture ambient occlusion on flat surfaces."
		" 15 works best for large features like curved and normal mapped geometry with"
		" SSAO but likes to fail due to not-oriented cone."
		" thus for SSAO self-shadowing is disabled using only texture ambient occlusion."
		" 6 seems to be a good value for this case." );
	SetCategory( ecExpert );
	SetDisplayName( "AO Self Shadow Smooth Angle" );
	SetDefaultValue( "10" );
}

deoglPAOSelfShadowSmoothAngle::~deoglPAOSelfShadowSmoothAngle(){
}



// Parameter Value
////////////////////

float deoglPAOSelfShadowSmoothAngle::GetParameterFloat(){
	return pOgl.GetConfiguration().GetAOSelfShadowSmoothAngle();
}

void deoglPAOSelfShadowSmoothAngle::SetParameterFloat( float value ){
	pOgl.GetConfiguration().SetAOSelfShadowSmoothAngle( value );
}
