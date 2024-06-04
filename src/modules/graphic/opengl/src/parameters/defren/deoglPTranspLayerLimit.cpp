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

#include "deoglPTranspLayerLimit.h"
#include "../../deGraphicOpenGl.h"
#include "../../configuration/deoglConfiguration.h"

#include <dragengine/common/exceptions.h>



// Class deoglPTranspLayerLimit
/////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglPTranspLayerLimit::deoglPTranspLayerLimit( deGraphicOpenGl &ogl ) : deoglParameterInt( ogl ){
	SetName( "transpLayerLimit" );
	SetDescription( "Limits transparency rendering to N layers. Lower values increase performance"
		" but can introduce visual errors. Higher values are more accurate but more expensive."
		" Values ranges from 1 to 15 with good values between 4 to 12. Default is 8." );
	SetType( deModuleParameter::eptRanged );
	SetMinimumValue( 1.0f );
	SetMaximumValue( 15.0f );
	SetValueStepSize( 1.0f );
	SetCategory( ecBasic );
	SetDisplayName( "Transparency Layer Limit" );
	SetDefaultValue( "8" );
}

deoglPTranspLayerLimit::~deoglPTranspLayerLimit(){
}



// Management
///////////////

int deoglPTranspLayerLimit::GetParameterInt(){
	return pOgl.GetConfiguration().GetTranspLayerLimit();
}

void deoglPTranspLayerLimit::SetParameterInt( int value ){
	pOgl.GetConfiguration().SetTranspLayerLimit( value );
}
