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

#include "deoglPSSAOEdgeBlurThreshold.h"
#include "../../deGraphicOpenGl.h"
#include "../../configuration/deoglConfiguration.h"

#include <dragengine/common/exceptions.h>



// Class deoglPSSAOEdgeBlurThreshold
//////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglPSSAOEdgeBlurThreshold::deoglPSSAOEdgeBlurThreshold( deGraphicOpenGl &ogl ) :
deoglParameterFloat( ogl )
{
	SetName( "ssaoEdgeBlurThreshold" );
	SetDescription( "Sets the depth threshold for the blur pass of the screen space ambient occlusion."
		" Screen space ambient occlusion is a stochastic algorithm introducing noise into the result "
		" due to taking less samples than required to cover the entire sampling range for spead reason."
		" To hide the noise a blur pass is applied to the calculated ambient occlusion."
		" This blur is an edge aware blur using depth differences to avoid bluring across edges."
		" This parameter defines the depth threshold between neighbor pixels to fade out to blur towards."
		" Smaller values increase the blur hiding noise better but wash out edges in the distance"
		" which can cause light bleeding."
		" Larger values reduce the blur on the noise resulting in a sharper image but with more"
		" noise especially in the distance."
		" The default value is 5 which equals a depth difference of 0.2m at 1m distance from the camera." );
	SetCategory( ecExpert );
	SetDisplayName( "SSAO Edge Blur Threshold" );
	SetDefaultValue( "5" );
}

deoglPSSAOEdgeBlurThreshold::~deoglPSSAOEdgeBlurThreshold(){
}



// Parameter Value
////////////////////

float deoglPSSAOEdgeBlurThreshold::GetParameterFloat(){
	return pOgl.GetConfiguration().GetSSAOEdgeBlurThreshold();
}

void deoglPSSAOEdgeBlurThreshold::SetParameterFloat( float value ){
	pOgl.GetConfiguration().SetSSAOEdgeBlurThreshold( value );
}
