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

#include "deoglPSSAOMipMapBase.h"
#include "../../deGraphicOpenGl.h"
#include "../../configuration/deoglConfiguration.h"

#include <dragengine/common/exceptions.h>



// Class deoglPSSAOMipMapBase
///////////////////////////////

// Constructor, destructor
////////////////////////////

deoglPSSAOMipMapBase::deoglPSSAOMipMapBase( deGraphicOpenGl &ogl ) : deoglParameterFloat( ogl ){
	SetName( "ssaoMipMapBase" );
	SetDescription( "Sets the mip map base for screen space ambient occlusion."
		" Screen space ambient occlusion is a distance based algorithm sampling in a"
		" certain radius around geometry. The projection of the sampling radius as"
		" defined by ssaoRadius onto the screen depends on the distance to the camera."
		" To enable using the same sampling count for all radi a downsampled version of"
		" the depth buffer is used. The choice of the mip map level to use determines"
		" the amount of noise, flickering and quality. The typical values are in the"
		" range from 4 to 16 pixels (2 raised to the power of 2-4). Smaller values"
		" cause higher mip map levels to be chosen which reduce noise but raises flickering."
		" Larger values cause lower mip map levels to be chosen which reduce flickering"
		" but raise noise. Higher noise can be compensated by increasing ssaoTapCount"
		" at the expense of higher runtime cost. The default value of 8 provides a"
		" reasonable balance between the two." );
	SetCategory( ecExpert );
	SetDisplayName( "SSAO MipMap Base" );
	SetDefaultValue( "8" );
}

deoglPSSAOMipMapBase::~deoglPSSAOMipMapBase(){
}



// Parameter Value
////////////////////

float deoglPSSAOMipMapBase::GetParameterFloat(){
	return pOgl.GetConfiguration().GetSSAOMipMapBase();
}

void deoglPSSAOMipMapBase::SetParameterFloat( float value ){
	pOgl.GetConfiguration().SetSSAOMipMapBase( value );
}
