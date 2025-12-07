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

#include "deoglPNorRougCorrStrength.h"
#include "../../deGraphicOpenGl.h"
#include "../../configuration/deoglConfiguration.h"

#include <dragengine/common/exceptions.h>



// Class deoglPNorRougCorrStrength
////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglPNorRougCorrStrength::deoglPNorRougCorrStrength(deGraphicOpenGl &ogl) :
deoglParameterFloat(ogl)
{
	SetName("normalRoughnessCorrectionStrength");
	SetDescription("Sets the roughness correction based on normal variance."
		" Mip mapping averages normals at the distance turning smooth but bumpy surfaces"
		" into near-like mirrors."
		" Furthermore this results in shimmering artifacts while moving the camera."
		" The variance lost in the mip map process has be applied to the roughness to"
		" compensate for the lost bumpiness."
		" This works since roughness is a micro-scale bumpiness of a surface while normals"
		" are macro-scale bumpiness."
		" Values are in the range from 0 to 1 where 0 indicating the percentage of lost"
		" normal variance applied to the roughness."
		" Too low values cause shimmering on bumpy but otherwise smooth surfaces."
		" Too high values cause surfaces to turn dull too strong."
		" The default value is 0.75."
		" This applies roughly 75% of normal variance to the roughness."
		" This accounts for interreflections happening reducing the correction at the distance a bit."
		" Values between 50-100% are reasonable.");
	SetCategory(ecExpert);
	SetDisplayName("Normal Roughness Correction");
	SetDefaultValue("0.75");
}

deoglPNorRougCorrStrength::~deoglPNorRougCorrStrength(){
}



// Parameter Value
////////////////////

float deoglPNorRougCorrStrength::GetParameterFloat(){
	return pOgl.GetConfiguration().GetNormalRoughnessCorrectionStrength();
}

void deoglPNorRougCorrStrength::SetParameterFloat(float value){
	pOgl.GetConfiguration().SetNormalRoughnessCorrectionStrength(value);
}
