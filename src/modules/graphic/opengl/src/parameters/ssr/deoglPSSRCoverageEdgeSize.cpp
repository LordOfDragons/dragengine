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

#include "deoglPSSRCoverageEdgeSize.h"
#include "../../deGraphicOpenGl.h"
#include "../../configuration/deoglConfiguration.h"

#include <dragengine/common/exceptions.h>



// Class deoglPSSRCoverageEdgeSize
////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglPSSRCoverageEdgeSize::deoglPSSRCoverageEdgeSize( deGraphicOpenGl &ogl ) :
deoglParameterFloat( ogl )
{
	SetName( "ssrCoverageEdgeSize" );
	SetDescription( "Sets the edge size for edge coverage calculation in screen space reflection."
		" Many reflections can not be calculated properly in screen space."
		" Especially near the edge the calculation fails with higher probability."
		" Using edge coverage the found results near the screen edge are gradually faded out"
		" to avoid sharp differences between reflections just inside or outside the screen boundaries."
		" This parameter sets the fading distance from the edge as percentage of half the screen dimension."
		" The default value is 0.1 (10%) which equals to 5% edge of the screen dimension."
		" Higher values enlarge the fading distance which might be more visually more pleasant."
		" Speed wise this has no impact." );
	SetCategory( ecExpert );
	SetDisplayName( "SSR Coverage Edge Size" );
	SetDefaultValue( "0.1" );
}

deoglPSSRCoverageEdgeSize::~deoglPSSRCoverageEdgeSize(){
}



// Parameter Value
////////////////////

float deoglPSSRCoverageEdgeSize::GetParameterFloat(){
	return pOgl.GetConfiguration().GetSSRCoverageEdgeSize();
}

void deoglPSSRCoverageEdgeSize::SetParameterFloat( float value ){
	pOgl.GetConfiguration().SetSSRCoverageEdgeSize( value );
}
