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

#include "deoglPSSAOTapCount.h"
#include "../../deGraphicOpenGl.h"
#include "../../configuration/deoglConfiguration.h"

#include <dragengine/common/exceptions.h>



// Class deoglPSSAOTapCount
/////////////////////////////

// Constructor, destructor
////////////////////////////

deoglPSSAOTapCount::deoglPSSAOTapCount( deGraphicOpenGl &ogl ) :
deoglParameterInt( ogl )
{
	SetName( "ssaoTapCount" );
	SetDescription( "Sets the number of samples to take for screen space ambient occlusion."
		" This determines the overall quality of the screen space ambient occlusion versus"
		" rendering speed. With lower tap counts the calculated ambient occlusion becomes"
		" more noisy. A value of 9 or lower should be used on weaker systems where rendering"
		" speed is more important than visual quality. A value between 9 to 18 gives good"
		" quality. Values above 18 do not improve quality considerably.  18 is the"
		" recommended value for modern graphic cards." );
	SetCategory( ecExpert );
	SetDisplayName( "SSAO Tap Count" );
	SetDefaultValue( "18" );
}

deoglPSSAOTapCount::~deoglPSSAOTapCount(){
}



// Parameter Value
////////////////////

int deoglPSSAOTapCount::GetParameterInt(){
	return pOgl.GetConfiguration().GetSSAOTapCount();
}

void deoglPSSAOTapCount::SetParameterInt( int value ){
	pOgl.GetConfiguration().SetSSAOTapCount( value );
}
