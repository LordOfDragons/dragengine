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

#include "deoglPSSAORadius.h"
#include "../../deGraphicOpenGl.h"
#include "../../configuration/deoglConfiguration.h"

#include <dragengine/common/exceptions.h>



// Class deoglPSSAORadius
///////////////////////////

// Constructor, destructor
////////////////////////////

deoglPSSAORadius::deoglPSSAORadius(deGraphicOpenGl &ogl) : deoglParameterFloat(ogl){
	SetName("ssaoRadius");
	SetDescription("Sets the world space radius for screen space ambient occlusion."
		" Screen space ambient occlusion is a distance based algorithm sampling in a"
		" certain radius around geometry."
		" The radius is set in meters and determines the distance after which a test ray"
		" is considered not occluded."
		" Typical values are in the range from 0.1m (1dm) to 1m."
		" Smaller values enhance small cravices missing though larger scale occlusion."
		" Large values provide a more correct larger scale ambient occlusion but have"
		" troubles with smaller cravices."
		" The risk of flickering, halos and edge undersampling rises with larger values."
		" The default value of 0.5 (5dm) provides a reasonable balance between the two.");
	SetCategory(ecExpert);
	SetDisplayName("SSAO Radius");
	SetDefaultValue("0.5");
}

deoglPSSAORadius::~deoglPSSAORadius(){
}



// Parameter Value
////////////////////

float deoglPSSAORadius::GetParameterFloat(){
	return pOgl.GetConfiguration().GetSSAORadius();
}

void deoglPSSAORadius::SetParameterFloat(float value){
	pOgl.GetConfiguration().SetSSAORadius(value);
}
