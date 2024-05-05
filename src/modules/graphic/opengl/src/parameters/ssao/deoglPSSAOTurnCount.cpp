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

#include "deoglPSSAOTurnCount.h"
#include "../../deGraphicOpenGl.h"
#include "../../configuration/deoglConfiguration.h"

#include <dragengine/common/exceptions.h>



// Class deoglPSSAOTurnCount
//////////////////////////////

// Constructor, destructor
////////////////////////////

deoglPSSAOTurnCount::deoglPSSAOTurnCount( deGraphicOpenGl &ogl ) :
deoglParameterFloat( ogl )
{
	SetName( "ssaoTurnCount" );
	SetDescription( "Sets the number of spiral turns to take for screen space ambient occlusion."
		" Screen space ambient occlusion is a stochastic algorithm introducing noise into the result "
		" due to taking less samples than required to cover the entire sampling range for spead reason."
		" This requires distributing samples across the sampling range in an optimal pattern."
		" The algorithm uses a sort of spiral pattern to distribute the sampling points."
		" This parameter determines the number of turns to take for this spiral."
		" Values should be prime numbers to avoid visible patterns."
		" This is an advanced parameter that is best left to the default value of 7 turns." );
	SetCategory( ecExpert );
	SetDisplayName( "SSAO Turn Count" );
	SetDefaultValue( "7" );
}

deoglPSSAOTurnCount::~deoglPSSAOTurnCount(){
}



// Parameter Value
////////////////////

float deoglPSSAOTurnCount::GetParameterFloat(){
	return pOgl.GetConfiguration().GetSSAOTurnCount();
}

void deoglPSSAOTurnCount::SetParameterFloat( float value ){
	pOgl.GetConfiguration().SetSSAOTurnCount( value );
}
