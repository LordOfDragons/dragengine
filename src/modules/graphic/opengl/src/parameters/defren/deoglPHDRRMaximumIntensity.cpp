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
#include "deoglPHDRRMaximumIntensity.h"
#include "../../deGraphicOpenGl.h"
#include "../../configuration/deoglConfiguration.h"



// Class deoglPHDRRMaximumIntensity
/////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglPHDRRMaximumIntensity::deoglPHDRRMaximumIntensity(deGraphicOpenGl &ogl) :
deoglParameterFloat(ogl)
{
	SetName("hdrrMaximumIntensity");
	SetDescription("Sets the maximum intensity for the HDRR tone mapper. "
		"This parameter defines the maximum intensity of a color before becomg white. "
		"The default value is 2 which is double the intensity of a white pixel with "
		"full ambient and no other lights.");
	SetCategory(ecExpert);
	SetDisplayName("HDRR Maximum Intensity");
	SetDefaultValue("1.5");
}

deoglPHDRRMaximumIntensity::~deoglPHDRRMaximumIntensity(){
}



// Parameter Value
////////////////////

float deoglPHDRRMaximumIntensity::GetParameterFloat(){
	return pOgl.GetConfiguration().GetHDRRMaximumIntensity();
}

void deoglPHDRRMaximumIntensity::SetParameterFloat(float value){
	pOgl.GetConfiguration().SetHDRRMaximumIntensity(value);
}
