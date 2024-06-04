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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deoglPVRRenderScale.h"
#include "../../deGraphicOpenGl.h"
#include "../../configuration/deoglConfiguration.h"

#include <dragengine/common/exceptions.h>



// Class deoglPVRRenderScale
//////////////////////////////

// Constructor, destructor
////////////////////////////

deoglPVRRenderScale::deoglPVRRenderScale( deGraphicOpenGl &ogl ) :
deoglParameter( ogl )
{
	SetName( "vrRenderScale" );
	SetDescription( "Percentage scale of VR Rendering relative to size requested by VR System."
		" Down scaling improves performance at the cost of quality."
		" You can adjust scaling in VR Systems (like SteamVR)."
		" This parameter allows to dynamically adjust render scale.");
	SetType( eptRanged );
	SetCategory( ecBasic );
	SetDisplayName( "VR Render Scale" );
	SetMinimumValue( 50.0f ); // 50%
	SetMaximumValue( 100.0f ); // 100% (beyond not possible with VR systems)
	SetValueStepSize( 5.0f ); // 5%
	SetDefaultValue( "100" );
}

deoglPVRRenderScale::~deoglPVRRenderScale(){
}



// Parameter Value
////////////////////

decString deoglPVRRenderScale::GetParameterValue(){
	decString value;
	value.Format( "%.0f", pOgl.GetConfiguration().GetVRRenderScale() * 100.0f + 0.01f );
	return value;
}

void deoglPVRRenderScale::SetParameterValue( const char *value ){
	pOgl.GetConfiguration().SetVRRenderScale( 0.01f * ( float )( decString( value ).ToInt() ) );
}
