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

#include "deoglPSSAORadiusLimit.h"
#include "../../deGraphicOpenGl.h"
#include "../../configuration/deoglConfiguration.h"

#include <dragengine/common/exceptions.h>



// Class deoglPSSAORadiusLimit
////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglPSSAORadiusLimit::deoglPSSAORadiusLimit(deGraphicOpenGl &ogl) : deoglParameterFloat(ogl){
	SetName("ssaoRadiusLimit");
	SetDescription("Sets the screen space radius limit for screen space ambient occlusion."
		" Screen space ambient occlusion is a distance based algorithm sampling in a certain"
		" radius around geometry. The projection of the sampling radius as defined by"
		" ssaoRadius onto the screen depends on the distance to the camera. With decreasing"
		" distance the screen space radius quickly grows resulting in a sub-optimal size."
		" This parameter limits the radius for close geometry to a percentage of the screen"
		" size to avoid problems. The default value of 0.5 (50% of the screen size) provides"
		" a reasonable limit. This is an advanced parameter and should be left at the default.");
	SetCategory(ecExpert);
	SetDisplayName("SSAO Radius Limit");
	SetDefaultValue("0.5");
}

deoglPSSAORadiusLimit::~deoglPSSAORadiusLimit(){
}



// Parameter Value
////////////////////

float deoglPSSAORadiusLimit::GetParameterFloat(){
	return pOgl.GetConfiguration().GetSSAORadiusLimit();
}

void deoglPSSAORadiusLimit::SetParameterFloat(float value){
	pOgl.GetConfiguration().SetSSAORadiusLimit(value);
}
