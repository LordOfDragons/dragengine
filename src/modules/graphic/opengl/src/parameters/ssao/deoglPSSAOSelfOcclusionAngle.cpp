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

#include "deoglPSSAOSelfOcclusionAngle.h"
#include "../../deGraphicOpenGl.h"
#include "../../configuration/deoglConfiguration.h"

#include <dragengine/common/exceptions.h>



// Class deoglPSSAOSelfOcclusionAngle
///////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglPSSAOSelfOcclusionAngle::deoglPSSAOSelfOcclusionAngle(deGraphicOpenGl &ogl) :
deoglParameterFloat(ogl)
{
	SetName("ssaoSelfOcclusionAngle");
	SetDescription("Sets the self occlusion angle for screen space ambient occlusion."
		" Due to limited accuracy of the rendered depth image self occlusion can happen"
		" on flat or nearly flat geometry. To avoid this effect geometry is not taken"
		" into account for nearby occlusion if the angle is less than an threshold."
		" The threshold is defined as angle in degrees relative to the geometry."
		" Typical values are in the range from 10 to 30 degrees."
		" Small values reduce the risk of missing nearby geometry at shallow angles but"
		" runs the risk of false self occlusion in curved geometry."
		" Large values reduce the chance of false self occlusion but run the risk of"
		" missing geometry at shallow angles."
		" Shallow angles can happen near corners and affect mostly small cravices."
		" The default value is 10."
		" Increase if self occlusion becomes a problem.");
	SetCategory(ecExpert);
	SetDisplayName("SSAO Self Occlusion Angle");
	SetDefaultValue("10");
}

deoglPSSAOSelfOcclusionAngle::~deoglPSSAOSelfOcclusionAngle(){
}



// Parameter Value
////////////////////

float deoglPSSAOSelfOcclusionAngle::GetParameterFloat(){
	return pOgl.GetConfiguration().GetSSAOSelfOcclusionAngle();
}

void deoglPSSAOSelfOcclusionAngle::SetParameterFloat(float value){
	pOgl.GetConfiguration().SetSSAOSelfOcclusionAngle(value);
}
