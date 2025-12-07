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

#include "deoglPSSRCoveragePowerEdge.h"
#include "../../deGraphicOpenGl.h"
#include "../../configuration/deoglConfiguration.h"

#include <dragengine/common/exceptions.h>



// Class deoglPSSRCoveragePowerEdge
/////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglPSSRCoveragePowerEdge::deoglPSSRCoveragePowerEdge(deGraphicOpenGl &ogl) :
deoglParameterFloat(ogl)
{
	SetName("ssrCoveragePowerEdge");
	SetDescription("Sets the power for edge coverage calculation in screen space reflection."
		" Many reflections can not be calculated properly in screen space."
		" Especially near the edge the calculation fails with higher probability."
		" Using edge coverage the found results near the screen edge are gradually faded out"
		" to avoid sharp differences between reflections just inside or outside the screen boundaries."
		" This parameter sets the power to which the edge coverage is raised."
		" The default is 2 which results in a quadratic fading from no coverage to full coverage."
		" A value of 1 results in a linear fading from no coverage to full coverage."
		" Values larger than 2 result in fading function using x-raisedto-N keeping the coverage longer"
		" near full coverage before falling down to no coverage."
		" This can be used to adjust the fading function to your personal liking.");
	SetCategory(ecExpert);
	SetDisplayName("SSR Coverage Edge Power");
	SetDefaultValue("1");
}

deoglPSSRCoveragePowerEdge::~deoglPSSRCoveragePowerEdge(){
}



// Parameter Value
////////////////////

float deoglPSSRCoveragePowerEdge::GetParameterFloat(){
	return pOgl.GetConfiguration().GetSSRCoveragePowerEdge();
}

void deoglPSSRCoveragePowerEdge::SetParameterFloat(float value){
	pOgl.GetConfiguration().SetSSRCoveragePowerEdge(value);
}
